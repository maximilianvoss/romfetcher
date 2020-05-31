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

#include "stateconfig.h"

window_t stateconfig_target(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return ((config_t *) app->list.cursor)->destiny;
    }
    return window_search;
}

void stateconfig_persist(app_t *app) {

}

void stateconfig_init(app_t *app) {
    app->win = window_config;
    app->list.active = (linkedlist_t *) app->config.active;
    app->list.all = (linkedlist_t *) app->config.all;
    app->list.cursor = (linkedlist_t *) app->config.active;
    app->list.multi = 0;
    app->list.checkbox = 0;
    app->list.filterActive = 0;
}