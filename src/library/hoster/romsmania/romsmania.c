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
#include "romsmania.h"
#include "mapping.h"
#include "../urlhandling.h"
#include "../../../common/utils.h"
#include "../../helper/regex.h"
#include "../results.h"

#define SHORTNAME "RCC"
#define FULLNAME "https://www.romsmania.cc"
#define URL_TEMPLATE "https://romsmania.cc/roms/%system%/search?name=%query%&genre=&region=&orderBy=name&orderAsc=1&page=%page%"
#define URL_FAVICON "https://romsmania.cc/favicon.ico"

static result_t *search(system_t *system, char *searchString);

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadLink(char *response);

static hoster_t *hoster = NULL;

hoster_t *romsmania_getHoster(cache_t *cacheHandler) {
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

static result_t *search(system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    result_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romsmania_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(url, NULL, GET, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);
        curl_freeResponse(response);
        free(url);

        resultCount = ll_count(resultList);
        page++;
    } while (resultCount != ll_count(resultList));

    return resultList;
}

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData) {
    if (item == NULL) {
        return;
    }
    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse->data);

    curl_response_t *downloadPageResponse = curlling_fetch(linkDownloadPage, NULL, GET, 1L);
    char *linkDownload = fetchDownloadLink(downloadPageResponse->data);

    char *filename = str_concat(item->title, file_suffix(linkDownload));
    downloadCallbackFunction(appData, item->system, item->title, linkDownload, NULL, filename, GET);
    free(filename);

    curl_freeResponse(downloadPageResponse);
    free(linkDownloadPage);
    curl_freeResponse(detailPageResponse);
    free(linkDownload);
}

static char *fetchDownloadLink(char *response) {
    char *regexString = "<a class=\"wait__link\" href=\"([^\"]+)\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static char *fetchDownloadPageLink(char *response) {
    char *regexString = "<a href=\"([^\"]+)\" rel=\"nofollow\" id=\"download_link\" class=\"btn is-with-ico\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response) {
    char *regexString = "<a href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        result_t *item = result_newItem(system, hoster);
        result_setUrl(item, ptr->groups[0]);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = ll_append(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}