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
#include "romhustler.h"
#include "../urlhandling.h"
#include "mapping.h"
#include "../results.h"
#include "../../../common/utils.h"
#include "../../helper/regex.h"

#define SHORTNAME "HSL"
#define FULLNAME "https://romhustler.org"
#define URL_TEMPLATE "https://romhustler.org/roms/search/page:%page%?q=%query%&console_id%5B9%5D=%system%"
#define URL_PREFIX "https://romhustler.org"
#define URL_DOWNLOAD_LINK "https://romhustler.org/link/"
#define URL_FAVICON "https://romhustler.org/favicon.ico"

static result_t *search(system_t *system, char *searchString);

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response);

static char *fetchId(char *response);

static char *fetchDownloadLink(char *response);

static hoster_t *hoster = NULL;

hoster_t *romhustler_getHoster(cache_t *cacheHandler) {
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
        memcpy(hoster->favicon->binary, faviconResponse->data, sizeof(char) * faviconResponse->size);
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
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romhustler_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(url, NULL, GET, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);
        curl_freeResponse(response);
        free(url);

        resultCount = ll_count(resultList);
        page++;
    } while (resultCount != ll_count(resultList) && resultCount % 20 == 0);

    return resultList;
}

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData) {
    if (item == NULL) {
        return;
    }
    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *id = fetchId(detailPageResponse->data);

    csafestring_t *linkUrl = safe_create(URL_DOWNLOAD_LINK);
    safe_strcat(linkUrl, id);
    curl_response_t *linkJsonResponse = curlling_fetch(linkUrl->data, NULL, GET, 1L);

    char *downloadLink = fetchDownloadLink(linkJsonResponse->data);
    char *decodedDownloadLink = str_quoteDecode(downloadLink);

    char *filename = str_concat(item->title, file_suffix(decodedDownloadLink));
    downloadCallbackFunction(appData, item->system, item->title, decodedDownloadLink, NULL, filename, GET);

    free(filename);
    curl_freeResponse(linkJsonResponse);
    free(id);
    free(downloadLink);
    free(decodedDownloadLink);
    curl_freeResponse(detailPageResponse);
    safe_destroy(linkUrl);
}

static result_t *fetchingResultItems(system_t *system, result_t *resultList, char *response) {
    char *regexString = "<div class=\"title\"><a href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        result_t *item = result_newItem(system, hoster);

        char *url = str_concat(URL_PREFIX, ptr->groups[0]);
        result_setUrl(item, url);
        free(url);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = ll_append(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}

static char *fetchId(char *response) {
    char *regexString = "var download_id = ([^;]+);";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *id = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return id;
}

static char *fetchDownloadLink(char *response) {
    char *regexString = "\"direct\":\"([^\"]+)\"";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}
