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
#include "../curlling.h"
#include "../../helper/regex.h"
#include "../results.h"
#include "../enginehandler.h"
#include "../../helper/utils.h"
#include "../../helper/path.h"

#define URL_TEMPLATE "https://romhustler.org/roms/search/page:%page%?q=%query%&console_id%5B9%5D=%system%"
#define URL_PREFIX "https://romhustler.org"
#define URL_DOWNLOAD_LINK "https://romhustler.org/link/"

static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response);

static char *fetchId(char *response);

static char *fetchFilename(char *response);

static char *fetchDownloadLink(char *response);

searchresult_t *romhustler_search(void *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romhustler_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        char *response = curlling_fetchURL(url);
        resultList = fetchingResultItems(app, system, resultList, response);
        free(response);
        free(url);

        resultCount = linkedlist_getElementCount(resultList);
        page++;
    } while (resultCount != linkedlist_getElementCount(resultList) && resultCount % 20 == 0);

    return resultList;
}

void romhustler_download(void *app, searchresult_t *item, void (*callback)(void *app)) {
    if (item == NULL) {
        return;
    }
    char *detailPageResponse = curlling_fetchURL(item->url);
    char *id = fetchId(detailPageResponse);
    char *filename = fetchFilename(detailPageResponse);

    csafestring_t *linkUrl = safe_create(URL_DOWNLOAD_LINK);
    safe_strcat(linkUrl, id);
    char *linkJsonResponse = curlling_fetchURL(linkUrl->data);

    char *downloadLink = fetchDownloadLink(linkJsonResponse);
    char *decodedDownloadLink = str_quoteDecode(downloadLink);

    csafestring_t *downloadPath = path_downloadTarget(item->system, filename);
    curlling_downloadURL(app, decodedDownloadLink, downloadPath->data);

    free(linkJsonResponse);
    free(id);
    free(filename);
    free(downloadLink);
    free(decodedDownloadLink);
    free(detailPageResponse);
    safe_destroy(linkUrl);
    safe_destroy(downloadPath);

    callback(app);
}

char *romhustler_shortname() {
    return "HSL";
}

static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<div class=\"title\"><a href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = result_newItem(system, enginehandler_findEngine(app, romhustler_shortname()));

        char *url = str_concat(URL_PREFIX, ptr->groups[0]);
        result_setUrl(item, url);
        free(url);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = linkedlist_appendElement(resultList, item);
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

static char *fetchFilename(char *response) {
    char *regexString = "data-filename=\"([^\"]+)\"";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *filename = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return filename;
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