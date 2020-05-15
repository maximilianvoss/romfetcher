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

searchresult_t *enginehandler_search(app_t *app, system_t *system, char *searchString) {
    searchresult_t *result = NULL;
    app->engine.active = app->engine.enabled;

    while (app->engine.active != NULL) {
        searchresult_t *tmp = app->engine.active->search(app, system, searchString);
        result = linkedlist_appendElement(result, tmp);
        app->engine.active = app->engine.active->next;
    }
    return linkedlist_sort(result);
}

void enginehandler_download(app_t *app, searchresult_t *item, void (*callback)(void *app)) {
    ((engine_t *) item->engine)->download(app, item, callback);
}

void enginehandler_doMapping(engine_t *ptr) {
    while (ptr != NULL) {
        if (!strcmp(ptr->name, "RCC")) {
            ptr->search = &romsmania_search;
            ptr->download = &romsmania_download;
        } else if (!strcmp(ptr->name, "MOD")) {
            ptr->search = &romsmode_search;
            ptr->download = &romsmode_download;
        } else if (!strcmp(ptr->name, "WOW")) {
            ptr->search = &wowroms_search;
            ptr->download = &wowroms_download;
        } else if (!strcmp(ptr->name, "RDC")) {
            ptr->search = &romsdownload_search;
            ptr->download = &romsdownload_download;
        } else if (!strcmp(ptr->name, "REN")) {
            ptr->search = &romsemulator_search;
            ptr->download = &romsemulator_download;
        } else {
            SDL_Log("Mapping was not found for %s(%s)\n", ptr->fullname, ptr->name);
        }

        ptr = ptr->next;
    }
}
