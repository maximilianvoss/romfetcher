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
#include "enginehandler.h"
#include "romsmania/romsmania.h"
#include "romsmode/romsmode.h"
#include "wowroms/wowroms.h"
#include "romsdownload/romsdownload.h"
#include "romsemulator/romsemulator.h"
#include "romhustler/romhustler.h"

static void *executeThread(void *searchPtr);

typedef struct {
    app_t *app;
    engine_t *engine;
    system_t *system;
    char *searchString;
    pthread_t thread;
    searchresult_t *result;
} search_t;

searchresult_t *enginehandler_search(app_t *app, system_t *system, char *searchString) {
    searchresult_t *result = NULL;
    engine_t *ptr = app->engine.enabled;

    uint32_t count = linkedlist_getElementCount(ptr);
    search_t *searches = (search_t *) calloc(count, sizeof(search_t));

    for (int i = 0; ptr != NULL; i++, ptr = ptr->next) {
        searches[i].app = app;
        searches[i].engine = ptr;
        searches[i].system = system;
        searches[i].searchString = searchString;
        searches[i].thread = NULL;
        searches[i].result = NULL;
        pthread_create(&searches[i].thread, NULL, executeThread, &searches[i]);
    }

    for (int i = 0; i < count; i++) {
        pthread_join(searches[i].thread, NULL);
        result = linkedlist_appendElement(result, searches[i].result);
    }

    return linkedlist_sort(result);
}

void enginehandler_download(app_t *app, searchresult_t *item, void (*callback)(void *app)) {
    ((engine_t *) item->engine)->download(app, item, callback);
}

void enginehandler_doMapping(engine_t *ptr) {
    while (ptr != NULL) {
        if (!strcmp(ptr->name, romsmania_shortname())) {
            ptr->search = &romsmania_search;
            ptr->download = &romsmania_download;
            ptr->shortname = &romsmania_shortname;
        } else if (!strcmp(ptr->name, romsmode_shortname())) {
            ptr->search = &romsmode_search;
            ptr->download = &romsmode_download;
            ptr->shortname = &romsmode_shortname;
        } else if (!strcmp(ptr->name, wowroms_shortname())) {
            ptr->search = &wowroms_search;
            ptr->download = &wowroms_download;
            ptr->shortname = &wowroms_shortname;
        } else if (!strcmp(ptr->name, romsdownload_shortname())) {
            ptr->search = &romsdownload_search;
            ptr->download = &romsdownload_download;
            ptr->shortname = &romsdownload_shortname;
        } else if (!strcmp(ptr->name, romsemulator_shortname())) {
            ptr->search = &romsemulator_search;
            ptr->download = &romsemulator_download;
            ptr->shortname = &romsemulator_shortname;
        } else if (!strcmp(ptr->name, romhustler_shortname())) {
            ptr->search = &romhustler_search;
            ptr->download = &romhustler_download;
            ptr->shortname = &romhustler_shortname;
        } else {
            SDL_Log("Mapping was not found for %s(%s)\n", ptr->fullname, ptr->name);
        }

        ptr = ptr->next;
    }
}

engine_t *enginehandler_findEngine(app_t *app, char *name) {
    engine_t *ptr = app->engine.all;
    while (ptr != NULL) {
        if (!strcmp(name, ptr->shortname())) {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

static void *executeThread(void *searchPtr) {
    search_t *search = (search_t *) searchPtr;
    search->result = search->engine->search(search->app, search->system, search->searchString);
    return NULL;
}