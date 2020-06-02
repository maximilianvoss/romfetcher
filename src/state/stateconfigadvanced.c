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

#include "stateconfigadvanced.h"
#include "../database/config.h"
#include "../ui/core.h"

window_t stateconfigadvanced_target(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return window_config_advanced;
    }
    database_configPersist(app);
    ui_destroy(app);
    ui_init(app);
    return window_config;
}

void stateconfigadvanced_persist(app_t *app) {
}

void stateconfigadvanced_init(app_t *app) {
    if (app->win != window_config_advanced) {
        app->win = window_config_advanced;

        app->list.cursor = (linkedlist_t *) app->config.advanced.all;
        app->list.all = (linkedlist_t *) app->config.advanced.all;
        app->list.multi = 1;
        app->list.checkbox = 1;
        app->list.filterActive = 0;
    }
}
