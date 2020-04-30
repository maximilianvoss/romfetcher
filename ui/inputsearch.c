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

#include "inputsearch.h"
#include "../engine/results.h"
#include "../engine/enginehandler.h"

void inputsearch_processUp(app_t *app) {
    switch (app->search.position) {
        case searchactivity_config:
            break;
        case searchactivity_system:
            app->search.position = searchactivity_config;
            break;
        case searchactivity_field:
        case searchactivity_button:
            app->search.position = searchactivity_system;
            break;
        case searchactivity_results:
            if (app->search.resultHovered->prev == NULL) {
                app->search.position = searchactivity_field;
            } else {
                app->search.resultHovered = app->search.resultHovered->prev;
            }
            break;
    }
}

void inputsearch_processDown(app_t *app) {
    switch (app->search.position) {
        case searchactivity_config:
            app->search.position = searchactivity_system;
            break;
        case searchactivity_system:
            app->search.position = searchactivity_field;
            break;
        case searchactivity_field:
        case searchactivity_button:
            if (app->search.results != NULL) {
                app->search.resultHovered = app->search.results;
                app->search.position = searchactivity_results;
            }
            break;
        case searchactivity_results:
            if (app->search.resultHovered->next != NULL) {
                app->search.resultHovered = app->search.resultHovered->next;
            }
            break;
    }
}

void inputsearch_processLeft(app_t *app) {
    switch (app->search.position) {
        case searchactivity_config:
            app->search.position = searchactivity_system;
            break;
        case searchactivity_system:
            break;
        case searchactivity_field:
            app->search.position = searchactivity_system;
            break;
        case searchactivity_button:
            app->search.position = searchactivity_field;
            break;
        case searchactivity_results:
            if (app->search.resultHovered->prev == NULL) {
                app->search.position = searchactivity_button;
            } else {
                app->search.resultHovered = app->search.resultHovered->prev;
            }
            break;
    }
}

void inputsearch_processRight(app_t *app) {
    switch (app->search.position) {
        case searchactivity_config:
            break;
        case searchactivity_system:
            app->search.position = searchactivity_field;
            break;
        case searchactivity_field:
            app->search.position = searchactivity_button;
            break;
        case searchactivity_button:
            if (app->search.results != NULL) {
                app->search.resultHovered = app->search.results;
                app->search.position = searchactivity_results;
            }
            break;
        case searchactivity_results:
            if (app->search.resultHovered->next != NULL) {
                app->search.resultHovered = app->search.resultHovered->next;
            }
            break;
    }
}

void inputsearch_processSelect(app_t *app) {
    switch (app->search.position) {
        case searchactivity_config:
            app->win = window_config;
            break;
        case searchactivity_system:
            app->win = window_system;
            break;
        case searchactivity_field:
            app->win = window_keyboard;
            break;
        case searchactivity_button:
            if (strlen(app->search.searchText) > 2) {
                if (app->search.results != NULL) {
                    result_freeList(app->search.results);
                }
                app->search.results = enginehandler_search(app, app->search.systemActive, app->search.searchText);
                app->search.resultHovered = app->search.results;
            }
            break;
        case searchactivity_results:
            app->search.resultActive = app->search.resultHovered;
            app->win = window_download;
            break;
    }
}

void inputsearch_processBack(app_t *app) {
}

void inputsearch_processOtherButton(app_t *app, GameControllerState_t *state) {
}