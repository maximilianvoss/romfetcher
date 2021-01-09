/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <csafestring.h>
#include <helper/domparsing.h>
#include "romsdownload.h"
#include "mapping.h"
#include "../urlhandling.h"
#include "../../helper/regex.h"
#include "../results.h"
#include "../../../common/utils.h"
#include "icon.h"

#define SHORTNAME "RDC"
#define FULLNAME "https://www.roms-download.com"
#define URL_TEMPLATE "https://roms-download.com/ajax.php?m=roms_j"
#define DATA_TEMPLATE "sort=file_name%24ASC&page=%page%&search=%query%&rom_concole=%system%"
#define URL_PREFIX "https://roms-download.com"

static result_t *search(system_t *system, char *searchString);

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response);

static char *fetchDownloadLink(char *response);

static uint32_t recalcPageCount(char *response);

static hoster_t *hoster = NULL;

hoster_t *romsdownload_getHoster(cache_t *cacheHandler) {
    if (hoster == NULL) {
        hoster = calloc(1, sizeof(hoster_t));
        hoster->search = search;
        hoster->download = download;
        hoster->name = SHORTNAME;
        hoster->active = 1;
        hoster->fullname = FULLNAME;
        hoster->favicon = calloc(1, sizeof(memimage_t));
        hoster->favicon->binary = romdownload_icon_data;
        hoster->favicon->size = romdownload_icon_size;
    }
    return hoster;
}

static result_t *search(system_t *system, char *searchString) {
    uint32_t page = 1;
    uint32_t pageCount = 1;

    result_t *resultList = NULL;
    while (page <= pageCount) {
        char *data = urlhandling_substitudeVariables(DATA_TEMPLATE, system, &romsdownload_deviceMapping, searchString,
                                                     page);
        if (data == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(URL_TEMPLATE, data, POST, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);

        if (pageCount == 1) {
            pageCount = recalcPageCount(response->data);
        }

        curl_freeResponse(response);
        free(data);

        page++;
    }
    return resultList;
}

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData) {
    if (item == NULL) {
        return;
    }
    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *linkDownload = fetchDownloadLink(detailPageResponse->data);

    char *filename = str_concat(item->title, file_suffix(linkDownload));

    downloadCallbackFunction(appData, item->system, item->title, linkDownload, NULL, filename, GET);

    free(filename);
    curl_freeResponse(detailPageResponse);
    free(linkDownload);
}

static char *fetchDownloadLink(char *response) {
    char *regexString = "<a href=\"([^\"]+)\" rel=\"nofollow\" itemprop=\"downloadUrl\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response) {
    lxb_html_document_t *document;
    lxb_dom_collection_t *wrapperCollection = domparsing_getElementsCollectionByTagName(response, &document, "TBODY");
    lxb_dom_collection_t *gamesCollection = domparsing_createCollection(document);
    lxb_dom_collection_t *gameElementCollection = domparsing_createCollection(document);

    domparsing_findChildElementsByTagName(gamesCollection, lxb_dom_collection_element(wrapperCollection, 0), "TR", 1);

    for (size_t i = 0; i < lxb_dom_collection_length(gamesCollection); i++) {
        lxb_dom_element_t *gameParent = lxb_dom_collection_element(gamesCollection, i);
        result_t *item = result_newItem(system, hoster);

        domparsing_findChildElementsByTagName(gameElementCollection, gameParent, "TD", 1);

        lxb_dom_element_t *element;
        element = lxb_dom_collection_element(gameElementCollection, 0);
        result_setTitle(item, domparsing_getText(element));

        element = domparser_findFirstChildElementByTagName(element, "A", 1);
        char *url = str_concat(URL_PREFIX, domparsing_getAttributeValue(element, "href"));
        result_setUrl(item, url);
        free(url);

        element = lxb_dom_collection_element(gameElementCollection, 2);
        char *rating = domparsing_getText(element);
        rating = str_replace(rating, ",", ".");
        result_setRating(item, rating, 5);

        element = lxb_dom_collection_element(gameElementCollection, 3);
        char *downloads = domparsing_getText(element);
        downloads = str_replace(downloads, ",", "");
        result_setDownloads(item, downloads);

        element = lxb_dom_collection_element(gameElementCollection, 4);
        char *fileSize = domparsing_getText(element);
        result_setFileSize(item, fileSize);

        lxb_dom_collection_clean(gameElementCollection);
        resultList = ll_append(resultList, item);
    }
    lxb_dom_collection_destroy(gameElementCollection, true);
    lxb_dom_collection_destroy(gamesCollection, true);
    lxb_dom_collection_destroy(wrapperCollection, true);
    lxb_html_document_destroy(document);

    return resultList;
}

static uint32_t recalcPageCount(char *response) {
    lxb_html_document_t *document;
    lxb_dom_collection_t *navContainer = domparsing_getElementsCollectionByClassName(response, &document, "pagination");
    lxb_dom_collection_t *navItems = domparsing_createCollection(document);

    if (!lxb_dom_collection_length(navContainer)) {
        return 0;
    }

    lxb_dom_element_t *navContainerElement = lxb_dom_collection_element(navContainer, 0);
    domparsing_findChildElementsByTagName(navItems, navContainerElement, "LI", 1);

    lxb_dom_element_t *navItemElement = lxb_dom_collection_element(navItems, lxb_dom_collection_length(navItems) - 2);
    char *pages = domparsing_getText(navItemElement);
    int retVal = atoi(pages);

    lxb_dom_collection_destroy(navContainer, true);
    lxb_dom_collection_destroy(navItems, true);
    lxb_html_document_destroy(document);

    return retVal;
}