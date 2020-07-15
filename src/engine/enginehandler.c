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
#include "freeroms/freeroms.h"
#include "../database/linkedlist.h"
#include "../config.h"

static void *executeThread(void *searchPtr);

typedef struct {
    app_t *app;
    engine_t *engine;
    system_t *system;
    char *searchString;
    pthread_t thread;
    searchresult_t *result;
} search_t;

void enginehandler_init(app_t *app) {
    app->engine.all = NULL;
    app->engine.all = linkedlist_appendElement(app->engine.all, freeroms_getEngine());
    app->engine.all = linkedlist_appendElement(app->engine.all, romhustler_getEngine());
    app->engine.all = linkedlist_appendElement(app->engine.all, romsdownload_getEngine());
    app->engine.all = linkedlist_appendElement(app->engine.all, romsemulator_getEngine());
    app->engine.all = linkedlist_appendElement(app->engine.all, romsmania_getEngine());
    app->engine.all = linkedlist_appendElement(app->engine.all, romsmode_getEngine());
    app->engine.all = linkedlist_appendElement(app->engine.all, wowroms_getEngine());

    databaselinkedlist_loadActivities(app->database.db, DATABASE_TABLE_ENGINES, (linkedlist_t *) app->engine.all);
}

searchresult_t *enginehandler_search(app_t *app, system_t *system, char *searchString) {
    searchresult_t *result = NULL;
    engine_t *ptr = app->engine.all;

    uint32_t count = linkedlist_getElementCount(ptr);
    search_t *searches = (search_t *) calloc(count, sizeof(search_t));

    int activeNumber = 0;
    while (ptr != NULL) {
        if (ptr->active) {
            searches[activeNumber].app = app;
            searches[activeNumber].engine = ptr;
            searches[activeNumber].system = system;
            searches[activeNumber].searchString = searchString;
            searches[activeNumber].result = NULL;
            pthread_create(&searches[activeNumber].thread, NULL, executeThread, &searches[activeNumber]);
            activeNumber++;
        }
        ptr = ptr->next;
    }

    for (int i = 0; i < activeNumber; i++) {
        pthread_join(searches[i].thread, NULL);
        result = linkedlist_appendElement(result, searches[i].result);
    }

    return linkedlist_sort(result);
}

void enginehandler_download(app_t *app, searchresult_t *item) {
    ((engine_t *) item->engine)->download(app, item);
}

void enginehandler_destroy(app_t *app) {
    linkedlist_freeList(app->engine.all, NULL);
}

static void *executeThread(void *searchPtr) {
    search_t *search = (search_t *) searchPtr;
    search->result = search->engine->search(search->app, search->system, search->searchString);
    return NULL;
}
