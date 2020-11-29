/*
 * Copyright 2020 Maximilian Voss (maximilian@voss.rocks)
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
#include <time.h>
#include <helper/domparsing.h>
#include "wowroms.h"
#include "mapping.h"
#include "../urlhandling.h"
#include "../../../common/utils.h"
#include "../../helper/regex.h"
#include "../../helper/md5.h"
#include "../results.h"


#define SHORTNAME "WOW"
#define FULLNAME "https://www.wowroms.com"
#define URL_TEMPLATE "https://wowroms.com/en/roms/list/%system%?search=%query%&page=%page%"
#define URL_PREFIX "https://wowroms.com"
#define URL_FAVICON "https://www.wowroms.com/assets/images/favicon.jpg"

static result_t *search(system_t *system, char *searchString);

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadServlet(char *response);

static char *fetchHiddenField(char *text, char *fieldname);

static char *fetchDownloadLink(char *response);

static hoster_t *hoster = NULL;

hoster_t *wowroms_getHoster(cache_t *cacheHandler) {
    if (hoster == NULL) {
        hoster = calloc(1, sizeof(hoster_t));
        hoster->name = SHORTNAME;
        hoster->fullname = FULLNAME;
        hoster->active = 1;
        hoster->search = search;
        hoster->download = download;
        hoster->cacheHandler = cacheHandler;

        curl_response_t *faviconResponse = curlling_fetch(URL_FAVICON, NULL, GET, 0L);
        hoster->favicon = calloc(1, sizeof(memimage_t));
        hoster->favicon->binary = calloc(faviconResponse->size, sizeof(char));
        memcpy(hoster->favicon->binary, faviconResponse->data, faviconResponse->size);
        hoster->favicon->size = faviconResponse->size;
        curl_freeResponse(faviconResponse);
    }
    return hoster;
}


// FIXME: fix the page count as it won't search on all pages
static result_t *search(system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    result_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &wowroms_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(url, NULL, GET, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);
        curl_freeResponse(response);
        free(url);

        resultCount = ll_count(resultList);
        page++;
    } while (resultCount != ll_count(resultList) && resultCount % 30 != 0);

    return resultList;
}

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData) {
    if (item == NULL) {
        return;
    }

    char timestamp[20];
    sprintf(timestamp, "%u", (unsigned) time(NULL));
    char *timestampMD5 = md5_str2md5(timestamp);
    char timeToken[255];
    snprintf(timeToken, 255, "?k=%s&t=%s", timestamp, timestampMD5);

    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *linkDownloadPageRelative = fetchDownloadPageLink(detailPageResponse->data);
    char *linkDownloadPage = str_concat(URL_PREFIX, linkDownloadPageRelative);

    curl_response_t *downloadPageResponse = curlling_fetch(linkDownloadPage, NULL, GET, 1L);
    char *downloadServletRel = fetchDownloadServlet(downloadPageResponse->data);
    char *downloadServlet = str_concat(URL_PREFIX, downloadServletRel);

    char *filename = fetchHiddenField(downloadPageResponse->data, "file");
    char *id = fetchHiddenField(downloadPageResponse->data, "id");
    char *emuid = fetchHiddenField(downloadPageResponse->data, "emuid");
    char *downloadServletUrl = str_concat(downloadServlet, timeToken);

    curl_response_t *downloadServletResponse = curlling_fetch(downloadServletUrl, "", POST, 1L);
    char *downloadLink = fetchDownloadLink(downloadServletResponse->data);
    char *decodedDownloadLink = str_quoteDecode(downloadLink);

    csafestring_t *payload = safe_create("emuid=");
    safe_strcat(payload, emuid);
    safe_strcat(payload, "&id=");
    safe_strcat(payload, id);
    safe_strcat(payload, "&file=");
    safe_strcat(payload, filename);

    char *decodedFilename = str_urlDecode(filename);

    char *downloadFilename = str_concat(item->title, file_suffix(fetchHiddenField(downloadPageResponse->data, "file")));
    downloadCallbackFunction(appData, item->system, item->title, decodedDownloadLink, payload->data, downloadFilename,
                             POST);
    free(downloadFilename);

    free(decodedFilename);
    free(decodedDownloadLink);
    free(downloadServletUrl);
    curl_freeResponse(downloadServletResponse);
    free(filename);
    free(emuid);
    free(id);
    free(timestampMD5);
    free(downloadServletRel);
    curl_freeResponse(detailPageResponse);
    free(linkDownloadPageRelative);
    free(linkDownloadPage);
    curl_freeResponse(downloadPageResponse);
    free(downloadServlet);
    free(downloadLink);
    safe_destroy(payload);
}

static char *fetchHiddenField(char *response, char *fieldname) {
    char *regexStringTmpl = "<input type=\"hidden\" name=\"%fieldname%\" value=\"([^\"]+)\" />";
    char *regexString = str_replace(regexStringTmpl, "%fieldname%", fieldname);

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    free(regexString);
    return link;
}

static char *fetchDownloadServlet(char *response) {
    char *regexString = "var ajaxLinkUrl = \"([^\"]+)\";";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static char *fetchDownloadLink(char *response) {
    char *regexString = "\"link\":\"([^\"]+)\"";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static char *fetchDownloadPageLink(char *response) {
    char *regexString = "<a class=\"[^\"]+\" style=\"[^\"]+\" href=\"([^\"]+)\">Download [^<]+</a>";

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
    lxb_dom_collection_t *gamesCollection = domparsing_getElementsCollectionByClassName(response, &document,
                                                                                        "group_info");
    lxb_dom_collection_t *gameElementCollection = domparsing_createCollection(document);

    for (size_t i = 0; i < lxb_dom_collection_length(gamesCollection); i++) {
        lxb_dom_element_t *gameParent = lxb_dom_collection_element(gamesCollection, i);
        result_t *item = result_newItem(system, hoster);

        domparsing_findChildElementsByTagName(gameElementCollection, gameParent, "A", 1);

        lxb_dom_element_t *element;
        element = lxb_dom_collection_element(gameElementCollection, 0);
        result_setTitle(item, domparsing_getText(element));

        char *url = str_concat(URL_PREFIX, domparsing_getAttributeValue(element, "href"));
        result_setUrl(item, url);
        free(url);

        element = lxb_dom_collection_element(gameElementCollection, 3);
        char *fileSize = domparsing_getText(element);
        fileSize = str_replace(fileSize, "File Size : ", "");
        result_setFileSize(item, fileSize);

        element = lxb_dom_collection_element(gameElementCollection, 4);
        char *downloads = domparsing_getText(element);
        downloads = str_replace(downloads, "Downlaod : ", "");
        result_setDownloads(item, downloads);

        element = lxb_dom_collection_element(gameElementCollection, 5);
        char *rating = domparsing_getText(element);
        rating = str_replace(rating, "Rating : ", "");
        result_setRating(item, rating, 5);

        lxb_dom_collection_clean(gameElementCollection);
        resultList = ll_append(resultList, item);
    }
    lxb_dom_collection_destroy(gameElementCollection, true);
    lxb_dom_collection_destroy(gamesCollection, true);
    lxb_html_document_destroy(document);

    return resultList;
}
