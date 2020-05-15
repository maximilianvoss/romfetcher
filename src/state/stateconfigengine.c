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

#include "stateconfigengine.h"
#include "../database/engines.h"

static void persistEngines(app_t *app);

window_t stateconfigengine_target(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return window_config_engine;
    }
    persistEngines(app);
    return window_config;
}

void stateconfigengine_persist(app_t *app) {

}

void stateconfigengine_init(app_t *app) {
    if (app->win != window_config_engine) {
        app->win = window_config_engine;
        app->list.cursor = (linkedlist_t *) app->engine.all;
        app->list.all = (linkedlist_t *) app->engine.all;
        app->list.multi = 1;
        app->list.checkbox = 1;
    }
}

static void persistEngines(app_t *app) {
    database_enginesStore(app->database.db, app->engine.all);

    database_enginesDestroy(app->engine.all);
    database_enginesDestroy(app->engine.enabled);

    app->engine.all = database_engineList(app, 0);
    app->engine.enabled = database_engineList(app, 1);

    app->engine.active = app->engine.enabled;
    app->engine.cursor = app->engine.enabled;
}