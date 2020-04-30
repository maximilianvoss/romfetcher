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

#include "inputsystem.h"


void inputsystem_processUp(app_t *app) {
    if (app->search.systemHovered->prev != NULL) {
        app->search.systemHovered = app->search.systemHovered->prev;
    }
}

void inputsystem_processDown(app_t *app) {
    if (app->search.systemHovered->next != NULL) {
        app->search.systemHovered = app->search.systemHovered->next;
    }
}

void inputsystem_processLeft(app_t *app) {
    if (app->search.systemHovered->prev != NULL) {
        app->search.systemHovered = app->search.systemHovered->prev;
    }
}

void inputsystem_processRight(app_t *app) {
    if (app->search.systemHovered->next != NULL) {
        app->search.systemHovered = app->search.systemHovered->next;
    }
}

void inputsystem_processSelect(app_t *app) {
    app->search.systemActive = app->search.systemHovered;
    app->search.systemHovered = NULL;
    app->win = window_search;
}

void inputsystem_processBack(app_t *app) {
    app->win = window_search;
}

void inputsystem_processOtherButton(app_t *app, GameControllerState_t *state) {
}