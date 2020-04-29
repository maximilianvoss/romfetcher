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

#include "romsmode.h"
#include "mapping.h"
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../../download/utils.h"
#include "../../helper/regex.h"
#include "../urlhandling.h"

#define URL_TEMPLATE "https://romsmode.com/roms/%system%/%page%?name=%query%"

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadLink(char *response);

static void removeFastTag(char *link);

searchresult_t *romsmode_search(app_t *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romsmode_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        resultCount = result_getListCount(resultList);

        char *response = curlling_fetchURL(url);
        resultList = fetchingResultItems(system, resultList, response);
        free(response);
        free(url);

        page++;
    } while (resultCount != result_getListCount(resultList));

    return resultList;
}

void romsmode_download(app_t *app, searchresult_t *item, void (*callback)(app_t *app)) {
    if (item == NULL) {
        return;
    }
    char *detailPageResponse = curlling_fetchURL(item->url);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse);

    removeFastTag(linkDownloadPage);
    char *downloadPageResponse = curlling_fetchURL(linkDownloadPage);
    char *linkDownloadArtifact = fetchDownloadLink(downloadPageResponse);

    char *filename = file_name(linkDownloadArtifact);
    char *decodedFilename = str_urlDecode(filename);

    char *downloadPath = download_targetPath(item->system, decodedFilename);

    curlling_downloadURL(app, linkDownloadArtifact, downloadPath);

    free(downloadPageResponse);
    free(linkDownloadPage);
    free(decodedFilename);
    free(detailPageResponse);
    free(linkDownloadArtifact);
    free(downloadPath);

    callback(app);
}

static void removeFastTag(char *link) {
    char *ptr = link;
    while (strcmp(ptr, "?fast")) {
        ptr++;
    }
    *ptr = '\0';
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
    char *regexString = "<a href=\"([^\"]+)\" class=\"btn is-with-ico\" rel=\"nofollow\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}


static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<a class=\"link\" href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = result_newItem(system);
        item->system = system;
        result_setUrl(item, ptr->groups[0]);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = result_addItemToList(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}