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
#include "../../download/curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../urlhandling.h"
#include "../../helper/regex.h"
#include "../../download/downloader.h"
#include "../../ui/rendering.h"

#define SHORTNAME "RCC"
#define URL_TEMPLATE "https://romsmania.cc/roms/%system%/search?name=%query%&genre=&region=&orderBy=name&orderAsc=1&page=%page%"

static searchresult_t *search(void *app, system_t *system, char *searchString);

static void download(void *app, searchresult_t *item);

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadLink(char *response);

static SDL_Texture *loadIcon(void *app);

static engine_t *engine = NULL;

static SDL_Texture *icon = NULL;

engine_t *romsmania_getEngine() {
    if (engine == NULL) {
        engine = calloc(1, sizeof(engine_t));
        engine->search = search;
        engine->download = download;
        engine->loadIcon = loadIcon;
        engine->name = SHORTNAME;
        engine->active = 0;
        engine->fullname = "https://www.romsmania.cc";
    }
    return engine;
}

static searchresult_t *search(void *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romsmania_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(url, NULL, GET, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);
        curl_freeResponse(response);
        free(url);

        resultCount = linkedlist_getElementCount(resultList);
        page++;
    } while (resultCount != linkedlist_getElementCount(resultList));

    return resultList;
}

static void download(void *app, searchresult_t *item) {
    if (item == NULL) {
        return;
    }
    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse->data);

    curl_response_t *downloadPageResponse = curlling_fetch(linkDownloadPage, NULL, GET, 1L);
    char *linkDownload = fetchDownloadLink(downloadPageResponse->data);

    char *filename = str_concat(item->title, file_suffix(linkDownload));
    downloader_addToQueue(app, item->system, item->title, linkDownload, NULL, filename, GET);
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

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<a href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = result_newItem(system, romsmania_getEngine());
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

static SDL_Texture *loadIcon(void *app) {
    if (icon == NULL) {
        curl_response_t *data = curlling_fetch("https://romsmania.cc/favicon.ico", NULL, GET, 0L);
        icon = rendering_memImage(app, data->data, data->size);
    }
    return icon;
}