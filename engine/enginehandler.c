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

#include "enginehandler.h"
#include "romsmania/romsmania.h"
#include "romsmode/romsmode.h"
#include "wowroms/wowroms.h"
#include "romsdownload/romsdownload.h"
#include "romsemulator/romsemulator.h"

engine_t *createEngine(char *title, searchresult_t *(*search)(void *app, system_t *system, char *searchString),
                       void (*download)(void *app, searchresult_t *item, void (*callback)(void *app)));

searchresult_t *enginehandler_search(app_t *app, system_t *system, char *searchString) {
    if (app->engine.active != NULL) {
        return app->engine.active->search(app, system, searchString);
    }
    return NULL;
}

void enginehandler_download(app_t *app, searchresult_t *item, void (*callback)(void *app)) {
    if (app->engine.active != NULL) {
        return app->engine.active->download(app, item, callback);
    }
}

void enginehandler_init(app_t *app) {
    app->engine.all = linkedlist_appendElement(app->engine.all,
                                               createEngine("https://www.romsmania.cc", &romsmania_search,
                                                            &romsmania_download));
    app->engine.all = linkedlist_appendElement(app->engine.all,
                                               createEngine("https://www.romsmode.com", &romsmode_search,
                                                            &romsmode_download));
    app->engine.all = linkedlist_appendElement(app->engine.all,
                                               createEngine("https://www.wowroms.com", &wowroms_search,
                                                            &wowroms_download));
    app->engine.all = linkedlist_appendElement(app->engine.all,
                                               createEngine("https://www.roms-download.com", &romsdownload_search,
                                                            &romsdownload_download));
    app->engine.all = linkedlist_appendElement(app->engine.all,
                                               createEngine("https://www.romsemulator.net", &romsemulator_search,
                                                            &romsemulator_download));
}

void enginehandler_destroy(app_t *app) {
    linkedlist_freeList(app->engine.all, NULL);
}

engine_t *createEngine(char *title, searchresult_t *(*search)(void *app, system_t *system, char *searchString),
                       void (*download)(void *app, searchresult_t *item, void (*callback)(void *app))) {
    engine_t *engine = calloc(1, sizeof(engine_t));
    engine->title = title;
    engine->search = search;
    engine->download = download;
    return engine;
}