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

#include <pthread.h>
#include <csafestring.h>
#include <helper/domparsing.h>
#include <hoster/results.h>
#include "freeroms.h"
#include "mapping.h"
#include "../../../common/utils.h"
#include "../urlhandling.h"

#define SHORTNAME "FRE"
#define FULLNAME "https://freeroms.com"
#define URL_TEMPLATE_NUM "https://www.freeroms.com/%system%_roms_NUM.htm"
#define URL_TEMPLATE_CHAR "https://www.freeroms.com/%system%_roms_%query%.htm"
#define URL_TEMPLATE_DOWNLOAD "https://www.freeroms.com/dl_roms/rom_download_tr.php?system=%system%&game_id=%id%"
#define URL_FAVICON "https://www.freeroms.com/favicon.ico"

static result_t *search(system_t *system, char *searchString);

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData);

static void fillCache(system_t *system);

static void *executeThread(void *ptr);

static void extractLink(system_t *system, char *response);

static char *generateDownloadLink(system_t *system, char *id);

struct s_download_filter {
    char start;
    char end;
    system_t *system;
};

static hoster_t *hoster = NULL;

hoster_t *freeroms_getHoster(cache_t *cacheHandler) {
    if (cacheHandler == NULL) {
        return NULL;
    }
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
    if (!hoster->cacheHandler->isValid(hoster, system, hoster->cacheHandler->appData)) {
        LOG_DEBUG("Cache is invalid and needs to be rebuilt");
        hoster->cacheHandler->clear(hoster, system, hoster->cacheHandler->appData);
        fillCache(system);
        hoster->cacheHandler->touch(hoster, system, hoster->cacheHandler->appData);
        LOG_DEBUG("Rebuilding complete");
    }
    LOG_DEBUG("Retrieve search result from Cache");
    return hoster->cacheHandler->get(hoster, system, searchString, hoster->cacheHandler->appData);
}

static void download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData) {
    if (item == NULL) {
        return;
    }

    char *filename = str_concat(item->title, ".zip");
    downloadCallbackFunction(appData, item->system, item->title, item->url, NULL, filename, GET);
    FREENOTNULL(filename);
}

static void fillCache(system_t *system) {
    pthread_t thread[HOSTER_FETCH_THREADS];
    struct s_download_filter filter[HOSTER_FETCH_THREADS];

    char chunks = ('Z' - '@') / HOSTER_FETCH_THREADS;
    for (int i = 0; i < HOSTER_FETCH_THREADS; i++) {
        filter[i].start = (char) '@' + i * chunks;
        filter[i].end = (char) '@' + (i + 1) * chunks - 1;
        filter[i].system = system;
    }
    filter[HOSTER_FETCH_THREADS - 1].end = 'Z';

    for (int i = 0; i < HOSTER_FETCH_THREADS; i++) {
        pthread_create(&thread[i], NULL, executeThread, &filter[i]);
    }
    for (int i = HOSTER_FETCH_THREADS - 1; i >= 0; i--) {
        pthread_join(thread[i], NULL);
    }
}


static void *executeThread(void *ptr) {
    struct s_download_filter *filter = (struct s_download_filter *) ptr;

    for (char chr = filter->start; chr <= filter->end; chr++) {
        char *url;
        curl_response_t *response;
        if (chr == '@') {
            url = urlhandling_substitudeVariables(URL_TEMPLATE_NUM, filter->system, &freeroms_deviceMapping, "", 0);
            if (url == NULL) {
                break;
            }
            response = curlling_fetch(url, NULL, GET, 1L);
            extractLink(filter->system, response->data);
        } else {
            char str[2] = {0, 0};
            str[0] = chr;
            url = urlhandling_substitudeVariables(URL_TEMPLATE_CHAR, filter->system, &freeroms_deviceMapping, str,
                                                  0);
            if (url == NULL) {
                break;
            }
            response = curlling_fetch(url, NULL, GET, 1L);
            extractLink(filter->system, response->data);
        }
        curl_freeResponse(response);
        FREENOTNULL(url);
    }

    return NULL;
}

static void extractLink(system_t *system, char *response) {
    lxb_html_document_t *document;
    lxb_dom_collection_t *gamesCollection = domparsing_getElementsCollectionByClassName(response, &document,
                                                                                        "system-rom-tr-wrap");
    lxb_dom_collection_t *gameElementCollection = domparsing_createCollection(document);

    for (size_t i = 0; i < lxb_dom_collection_length(gamesCollection); i++) {
        lxb_dom_element_t *gameParent = lxb_dom_collection_element(gamesCollection, i);
        result_t *item = result_newItem(system, hoster);

        domparsing_findChildElementsByTagName(gameElementCollection, gameParent, "DIV", 1);

        lxb_dom_element_t *element;
        element = lxb_dom_collection_element(gameElementCollection, 0);
        result_setTitle(item, domparsing_getText(element));

        element = lxb_dom_collection_element(gameElementCollection, 1);
        element = domparser_findFirstChildElementByTagName(element, "A", 1);
        char *gameId = (char *) domparsing_getAttributeValue(element, "onclick");
        gameId = str_replace(gameId, "window.open('/vote.php?game_id=", "");
        gameId = str_replace(gameId, "', 'votewindow', 'width=450, height=400'); return false;", "");
        result_setUrl(item, generateDownloadLink(system, gameId));

        element = lxb_dom_collection_element(gameElementCollection, 3);
        char *rating = domparsing_getText(element);
        rating = str_replace(rating, "/10", "");
        result_setRating(item, rating, 10);

        hoster->cacheHandler->add(hoster, system, item, hoster->cacheHandler->appData);
        result_freeList(item);

        lxb_dom_collection_clean(gameElementCollection);
    }
    lxb_dom_collection_destroy(gameElementCollection, true);
    lxb_dom_collection_destroy(gamesCollection, true);
    lxb_html_document_destroy(document);
}


static char *generateDownloadLink(system_t *system, char *id) {
    char *systemStr = freeroms_deviceMapping(system);
    if (systemStr == NULL) {
        LOG_INFO("Found no mapping for system: %s", system->fullname);
        return NULL;
    }

    char *result;
    char *tmp;

    result = str_replace(URL_TEMPLATE_DOWNLOAD, "%id%", id);

    tmp = result;
    result = str_replace(tmp, "%system%", systemStr);
    FREENOTNULL(tmp);

    return result;
}
