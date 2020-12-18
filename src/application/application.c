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

#include "application.h"
#include "ui/uihandler.h"
#include "input/inputhandler.h"
#include "ui/display.h"
#include "database/database.h"
#include "themes/loading.h"
#include "config/configmenu.h"
#include "download/downloader.h"
#include "database/download.h"
#include "helper/path.h"
#include "database/linkedlist.h"
#include "constants.h"
#include "enginecache/enginecache.h"
#include "themes/rendering.h"
#include "config/config.h"

void static initApp(app_t *app);

void static destroyApp(app_t *app);

cache_t cacheHandler;

int main() {
    app_t app;
    sdl_init();
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
    themes_init(app);

    cacheHandler.appData = app;
    cacheHandler.isValid = enginecache_isCacheValid;
    cacheHandler.clear = enginecache_clear;
    cacheHandler.add = enginecache_addEntry;
    cacheHandler.touch = enginecache_updateTimestamp;
    cacheHandler.get = enginecache_getSearchResults;

    hoster_t *hosters = loadHosters(&cacheHandler);
    system_t *systems = loadSystems();

    config_load(app->database.db, &app->config.active, &app->config.advanced.active, &app->config.resolution.active,
                &app->themes.active, hosters, systems);

    app->systems.active = ll_get1stActive(systems);
    app->systems.all = ll_get1st(systems);

    app->engine.all = hosters;

    app->config.cursor = app->config.active;
    app->config.resolution.cursor = app->config.resolution.active;
    app->config.advanced.cursor = app->config.advanced.active;

    display_init(app);
    inputhandler_init();

    themes_activate(app, app->themes.active);

    downloader_init(app);
    download_load(app);
}

void static destroyApp(app_t *app) {
    downloader_cancelAllDownloads(app);
    display_destroy(app);
    themes_destroy(app);

    destroyResults(app->search.all);
    destroyHosters(app->engine.all);
    destroySystems(app->systems.all);

    inputhandler_destroy();
    downloader_destroy(app);
    database_destroy(app);
    SDL_Quit();
}