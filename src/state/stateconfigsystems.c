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

#include "stateconfigsystems.h"
#include "../database/sytems.h"

static void persistSystems(app_t *app);

window_t stateconfigsystems_target(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return window_config_systems;
    }
    persistSystems(app);
    return window_config;
}

void stateconfigsystems_persist(app_t *app) {

}

void stateconfigsystems_init(app_t *app) {
    if (app->win != window_config_systems) {
        app->win = window_config_systems;
        app->list.cursor = (linkedlist_t *) app->systems.all;
        app->list.all = (linkedlist_t *) app->systems.all;
        app->list.multi = 1;
        app->list.checkbox = 1;
    }
}

static void persistSystems(app_t *app) {
    database_systemStore(app->database.db, app->systems.all);

    database_systemsDestroy(app->systems.all);
    database_systemsDestroy(app->systems.enabled);

    app->systems.all = database_systemList(app, 0);
    app->systems.enabled = database_systemList(app, 1);

    app->systems.active = app->systems.enabled;
    app->systems.cursor = app->systems.enabled;
}