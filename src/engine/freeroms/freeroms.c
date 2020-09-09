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
#include "freeroms.h"
#include "../../database/enginecache.h"
#include "../urlhandling.h"
#include "mapping.h"
#include "../../download/curlling.h"
#include "../../helper/regex.h"
#include "../../helper/utils.h"
#include "../../download/downloader.h"
#include "../../config.h"

#define SHORTNAME "FRE"
#define URL_TEMPLATE_NUM "https://www.freeroms.com/%system%_roms_NUM.htm"
#define URL_TEMPLATE_CHAR "https://www.freeroms.com/%system%_roms_%query%.htm"
#define URL_TEMPLATE_DOWNLOAD "https://www.freeroms.com/dl_roms/rom_download_tr.php?system=%system%&game_id=%id%"

static searchresult_t *search(void *app, system_t *system, char *searchString);

static void download(void *app, searchresult_t *item);

static void fillCache(app_t *app, system_t *system);

static void *executeThread(void *ptr);

static void extractLink(app_t *app, system_t *system, char *response);

static char *generateDownloadLink(system_t *system, char *id);

struct s_download_filter {
    char start;
    char end;
    app_t *app;
    system_t *system;
};

static engine_t *engine = NULL;

engine_t *freeroms_getEngine() {
    if (engine == NULL) {
        engine = calloc(1, sizeof(engine_t));
        engine->search = search;
        engine->download = download;
        engine->name = SHORTNAME;
        engine->active = 1;
        engine->fullname = "https://freeroms.com";
    }
    return engine;
}

static searchresult_t *search(void *app, system_t *system, char *searchString) {
    if (!enginecache_isCacheValid(app, SHORTNAME, system)) {
        enginecache_clear(app, SHORTNAME, system);
        fillCache(app, system);
        enginecache_updateTimestamp(app, SHORTNAME, system);
    }
    return enginecache_getSearchResults(app, freeroms_getEngine(), system, searchString);
}

static void download(void *app, searchresult_t *item) {
    if (item == NULL) {
        return;
    }

    char *filename = str_concat(item->title, ".zip");
    downloader_addToQueue(app, item->system, item->title, item->url, NULL, filename, GET);
    FREENOTNULL(filename);
}

static void fillCache(app_t *app, system_t *system) {
    pthread_t thread[ENGINE_FETCH_THREADS];
    struct s_download_filter filter[ENGINE_FETCH_THREADS];

    char chunks = ('Z' - '@') / ENGINE_FETCH_THREADS;
    for (int i = 0; i < ENGINE_FETCH_THREADS; i++) {
        filter[i].start = (char) '@' + i * chunks;
        filter[i].end = (char) '@' + (i + 1) * chunks - 1;
        filter[i].app = app;
        filter[i].system = system;
    }
    filter[ENGINE_FETCH_THREADS - 1].end = 'Z';

    for (int i = 0; i < ENGINE_FETCH_THREADS; i++) {
        pthread_create(&thread[i], NULL, executeThread, &filter[i]);
    }
    for (int i = ENGINE_FETCH_THREADS - 1; i >= 0; i--) {
        pthread_join(thread[i], NULL);
    }
}


static void *executeThread(void *ptr) {
    struct s_download_filter *filter = (struct s_download_filter *) ptr;

    for (char chr = filter->start; chr <= filter->end; chr++) {
        char *url;
        char *response;
        if (chr == '@') {
            url = urlhandling_substitudeVariables(URL_TEMPLATE_NUM, filter->system, &freeroms_deviceMapping, "", 0);
            if (url == NULL) {
                break;
            }
            response = curlling_fetch(url, NULL, GET);
            extractLink(filter->app, filter->system, response);
        } else {
            char str[2] = {0, 0};
            str[0] = chr;
            url = urlhandling_substitudeVariables(URL_TEMPLATE_CHAR, filter->system, &freeroms_deviceMapping, str,
                                                  0);
            if (url == NULL) {
                break;
            }
            response = curlling_fetch(url, NULL, GET);
            extractLink(filter->app, filter->system, response);
        }
        FREENOTNULL(response);
        FREENOTNULL(url);
    }

    return NULL;
}

static void extractLink(app_t *app, system_t *system, char *response) {
    char *regexString = "<span itemprop=\"name\">([^<]+)[^\']+\'\\/vote.php\\?game_id=([^\']+)";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        enginecache_addEntry(app, SHORTNAME, system, ptr->groups[0], generateDownloadLink(system, ptr->groups[1]));
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
}


static char *generateDownloadLink(system_t *system, char *id) {
    char *systemStr = freeroms_deviceMapping(system);
    if (systemStr == NULL) {
        fprintf(stderr, "Found no mapping for system: %s\n", system->fullname);
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