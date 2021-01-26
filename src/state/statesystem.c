/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
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

#include "statesystem.h"


window_t staterl_systemarget(app_t *app, uint8_t isSelectButton) {
    return window_search;
}

void statesystem_persist(app_t *app) {
    app->systems.active = app->list.active;
}

void statesystem_init(app_t *app) {
    app->win = window_system;
    app->list.active = app->systems.active;
    app->list.all = app->systems.all;
    app->list.cursor = app->systems.active;
    app->list.multi = 0;
    app->list.checkbox = 0;
    app->list.filterActive = 1;
}