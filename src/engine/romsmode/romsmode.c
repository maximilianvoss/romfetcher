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
#include "romsmode.h"
#include "mapping.h"
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../../helper/regex.h"
#include "../urlhandling.h"
#include "../../helper/path.h"

#define SHORTNAME "MOD"
#define URL_TEMPLATE "https://romsmode.com/roms/%system%/%page%?name=%query%"

static searchresult_t *search(void *app, system_t *system, char *searchString);

static void download(void *app, searchresult_t *item, void (*callback)(void *app));

static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadLink(char *response);

static void removeFastTag(char *link);

static engine_t *engine = NULL;

engine_t *romsmode_getEngine() {
    if (engine == NULL) {
        engine = calloc(1, sizeof(engine_t));
        engine->search = search;
        engine->download = download;
        engine->name = SHORTNAME;
        engine->active = 0;
        engine->fullname = "https://www.romsmode.com";
    }
    return engine;
}

static searchresult_t *search(void *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romsmode_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        char *response = curlling_fetchURL(url);
        resultList = fetchingResultItems(app, system, resultList, response);
        free(response);
        free(url);

        resultCount = linkedlist_getElementCount(resultList);
        page++;
    } while (resultCount != linkedlist_getElementCount(resultList));

    return resultList;
}

static void download(void *app, searchresult_t *item, void (*callback)(void *app)) {
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

    csafestring_t *downloadPath = path_downloadTarget(item->system, decodedFilename);

    curlling_downloadURL(app, linkDownloadArtifact, downloadPath->data);

    free(downloadPageResponse);
    free(linkDownloadPage);
    free(decodedFilename);
    free(detailPageResponse);
    free(linkDownloadArtifact);
    safe_destroy(downloadPath);

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


static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<a class=\"link\" href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = result_newItem(system, romsmode_getEngine());
        result_setUrl(item, ptr->groups[0]);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = linkedlist_appendElement(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}