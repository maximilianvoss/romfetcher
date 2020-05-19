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
#include "romsdownload.h"
#include "mapping.h"
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../urlhandling.h"
#include "../../helper/regex.h"
#include "../../helper/path.h"
#include "../enginehandler.h"

#define URL_TEMPLATE "https://roms-download.com/ajax.php?m=roms_j"
#define DATA_TEMPLATE "sort=file_name%24ASC&page=%page%&search=%query%&rom_concole=%system%"
#define URL_PREFIX "https://roms-download.com"

static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadLink(char *response);

searchresult_t *romsdownload_search(void *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;

    searchresult_t *resultList = NULL;
    do {
        char *data = urlhandling_substitudeVariables(DATA_TEMPLATE, system, &romsdownload_deviceMapping, searchString,
                                                     page);
        if (data == NULL) {
            break;
        }

        char *response = curlling_fetchURLPost(URL_TEMPLATE, data);
        resultList = fetchingResultItems(app, system, resultList, response);
        free(response);
        free(data);

        resultCount = linkedlist_getElementCount(resultList);
        page++;
    } while (resultCount != linkedlist_getElementCount(resultList) && resultCount % 20 == 0);

    return resultList;
}

void romsdownload_download(void *app, searchresult_t *item, void (*callback)(void *app)) {
    if (item == NULL) {
        return;
    }
    char *detailPageResponse = curlling_fetchURL(item->url);
    char *linkDownload = fetchDownloadLink(detailPageResponse);

    char *filename = file_name(linkDownload);
    char *decodedFilename = str_urlDecode(filename);
    csafestring_t *downloadPath = path_downloadTarget(item->system, decodedFilename);

    curlling_downloadURL(app, linkDownload, downloadPath->data);

    free(detailPageResponse);
    free(linkDownload);
    free(decodedFilename);
    safe_destroy(downloadPath);

    callback(app);
}

char *romsdownload_shortname() {
    return "RDC";
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

static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<a href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = result_newItem(system, enginehandler_findEngine(app, romsdownload_shortname()));

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