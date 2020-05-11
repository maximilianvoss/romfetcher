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

#include "statehandler.h"
#include "../database/sytems.h"
#include "../database/config.h"

static void handleSearch(app_t *app, window_t destiny);

static void handleSystem(app_t *app, window_t destiny);

static void handleKeyboard(app_t *app, window_t destiny);

static void handleDownload(app_t *app, window_t destiny);

static void handleConfig(app_t *app, window_t destiny);

static void handleConfigEngine(app_t *app, window_t destiny);

static void handleConfigSystems(app_t *app, window_t destiny);

void statehandler_change(app_t *app, window_t destiny) {
    switch (app->win) {
        case window_search:
            handleSearch(app, destiny);
            break;
        case window_system:
            handleSystem(app, destiny);
            break;
        case window_keyboard:
            handleKeyboard(app, destiny);
            break;
        case window_download:
            handleDownload(app, destiny);
            break;
        case window_config:
            handleConfig(app, destiny);
            break;
        case window_config_engine:
            handleConfigEngine(app, destiny);
            break;
        case window_config_systems:
            handleConfigSystems(app, destiny);
            break;
    }
    app->win = destiny;
}

static void handleSearch(app_t *app, window_t destiny) {
    switch (destiny) {
        case window_download:
            app->search.active = app->search.cursor;
            break;
        case window_keyboard:
            strcpy(app->keyboard.text, app->search.searchText);
            break;
        case window_system:
            app->list.active = (linkedlist_t *) app->systems.active;
            app->list.all = (linkedlist_t *) app->systems.all;
            app->list.cursor = (linkedlist_t *) app->systems.active;
            app->list.multi = 0;
            app->list.checkbox = 0;
            app->list.destiny = window_search;
        default:
            break;
    }
}

static void handleSystem(app_t *app, window_t destiny) {
    app->systems.active = (system_t *) app->list.active;
}

static void handleKeyboard(app_t *app, window_t destiny) {}

static void handleDownload(app_t *app, window_t destiny) {
    app->download.cursorPos = downloadActivity_cancel;
    app->download.started = 0;
    app->download.complete = 0;
}

static void handleConfig(app_t *app, window_t destiny) {
    switch (destiny) {
        case window_config_systems:
            app->list.cursor = (linkedlist_t *) app->systems.all;
            app->list.all = (linkedlist_t *) app->systems.all;
            app->list.multi = 1;
            app->list.checkbox = 1;
            app->list.destiny = window_config;
            break;
        case window_config_engine:
            if (app->engine.cursor == engine_notdefined) {
                app->engine.cursor = app->engine.active;
            }
            break;
        default:
            break;
    }
}

static void handleConfigEngine(app_t *app, window_t destiny) {
    app->engine.active = app->engine.cursor;
    database_configPersist(app);
    app->engine.cursor = engine_notdefined;
    app->win = window_config;
}

static void handleConfigSystems(app_t *app, window_t destiny) {
    database_systemStore(app->database.db, app->systems.all);

    database_systemsDestroy(app->systems.all);
    database_systemsDestroy(app->systems.enabled);

    app->systems.all = database_systemList(app, 0);
    app->systems.enabled = database_systemList(app, 1);

    app->systems.active = app->systems.enabled;
    app->systems.cursor = app->systems.enabled;
}