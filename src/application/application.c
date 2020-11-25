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

#include <zconf.h>
#include "application.h"
#include "ui/uihandler.h"
#include "input/inputhandler.h"
#include "ui/core.h"
#include "database/init.h"
#include "database/config.h"
#include "themes/loading.h"
#include "config/init.h"
#include "ui/fonts.h"
#include "download/downloader.h"
#include "database/download.h"
#include "path.h"
#include "database/linkedlist.h"
#include "config.h"
#include "database/enginecache.h"

void static initApp(app_t *app);

void static destroyApp(app_t *app);

cache_t cacheHandler;

int main() {
    app_t app;
    initApp(&app);

    while (!app.quit) {
        inputhandler_processInputs(&app);
        uihandler_render(&app);
        SDL_Delay(75);
    }

    destroyApp(&app);
    return 0;
}

void static initApp(app_t *app) {
    memset(app, 0, sizeof(app_t));
    path_initRomfetchersHome();
    database_init(app);

    cacheHandler.appData = app;
    cacheHandler.isValid = enginecache_isCacheValid;
    cacheHandler.clear = enginecache_clear;
    cacheHandler.add = enginecache_addEntry;
    cacheHandler.touch = enginecache_updateTimestamp;
    cacheHandler.get = enginecache_getSearchResults;


    hoster_t *hosters = loadHosters(&cacheHandler);
    app->engine.all = hosters;
    app->engine.cursor = hosters;
    databaselinkedlist_loadActivities(app->database.db, DATABASE_TABLE_ENGINES, (linkedlist_t *) app->engine.all);

    system_t *systems = loadSystems();
    app->systems.all = systems;
    app->systems.cursor = systems;
    databaselinkedlist_loadActivities(app->database.db, DATABASE_TABLE_SYSTEMS, (linkedlist_t *) systems);
    app->systems.active = ll_get1stActive(systems);

    themes_init(app);
    config_init(app);
    fonts_init(app);
    database_configLoad(app);
    ui_init(app);
    inputhandler_init();
    downloader_init(app);
    download_load(app);
}

void static destroyApp(app_t *app) {
    downloader_cancelAllDownloads(app);
    ui_destroy(app);
    themes_destroy(app);

    destroyResults(app->search.all);
    destroyHosters(app->engine.all);
    destroySystems(app->systems.all);

    config_destroy(app);
    inputhandler_destroy();
    fonts_destroy(app);
    downloader_destroy(app);
    database_destroy(app);
    SDL_Quit();
}