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
#include "../structs.h"
#include "../engine/enginehandler.h"

void search_processUp(app_t *app) {
    switch (app->search.position) {
        case searchsystem:
            break;
        case searchfield:
        case searchbutton:
            app->search.position = searchsystem;
            break;
        case searchresults:
            if (app->search.resultHovered->prev == NULL) {
                app->search.position = searchfield;
            } else {
                app->search.resultHovered = app->search.resultHovered->prev;
            }
            break;
    }
}

void search_processDown(app_t *app) {
    switch (app->search.position) {
        case searchsystem:
            app->search.position = searchfield;
            break;
        case searchfield:
        case searchbutton:
            if (app->search.results != NULL) {
                app->search.resultHovered = app->search.results;
                app->search.position = searchresults;
            }
            break;
        case searchresults:
            if (app->search.resultHovered->next != NULL) {
                app->search.resultHovered = app->search.resultHovered->next;
            }
            break;
    }
}

void search_processLeft(app_t *app) {
    switch (app->search.position) {
        case searchsystem:
            break;
        case searchfield:
            app->search.position = searchsystem;
            break;
        case searchbutton:
            app->search.position = searchfield;
            break;
        case searchresults:
            if (app->search.resultHovered->prev == NULL) {
                app->search.position = searchbutton;
            } else {
                app->search.resultHovered = app->search.resultHovered->prev;
            }
            break;
    }
}

void search_processRight(app_t *app) {
    switch (app->search.position) {
        case searchsystem:
            app->search.position = searchfield;
            break;
        case searchfield:
            app->search.position = searchbutton;
            break;
        case searchbutton:
            if (app->search.results != NULL) {
                app->search.resultHovered = app->search.results;
                app->search.position = searchresults;
            }
            break;
        case searchresults:
            if (app->search.resultHovered->next != NULL) {
                app->search.resultHovered = app->search.resultHovered->next;
            }
            break;
    }
}

void search_processSelect(app_t *app) {
    switch (app->search.position) {
        case searchsystem:
            app->search.systemHovered = app->search.systemActive;
            app->win = systemselect;
            break;
        case searchfield:
            app->win = keyboard;
            break;
        case searchbutton:
            if (strlen(app->search.searchText) > 2) {
                if (app->search.results != NULL) {
                    freeResultList(app->search.results);
                }
                app->search.results = searchhandler_search(app, app->search.systemActive, app->search.searchText);
                app->search.resultHovered = app->search.results;
            }
            break;
        case searchresults:
            app->search.resultActive = app->search.resultHovered;
            app->win = download;
            break;
    }
}

void search_processBack(app_t *app) {
}

void search_processOtherButton(app_t *app, GameControllerState_t *state) {
}