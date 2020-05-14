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

#include "statesearch.h"
#include "../engine/results.h"
#include "../engine/enginehandler.h"

window_t statesearch_target(app_t *app, uint8_t isSelectButton) {
    switch (app->search.position) {
        case searchactivity_config:
            return window_config;
        case searchactivity_system:
            return window_system;
        case searchactivity_field:
            return window_keyboard;
        case searchactivity_results:
            return window_download;
        default:
            return window_search;
    }
}

void statesearch_persist(app_t *app) {
    if (app->search.position == searchactivity_button) {
        if (strlen(app->search.searchText) > 2) {
            if (app->search.all != NULL) {
                result_freeList(app->search.all);
            }
            app->search.all = enginehandler_search(app, app->systems.active, app->search.searchText);
        }
    }
}

void statesearch_init(app_t *app) {
    app->win = window_search;
}