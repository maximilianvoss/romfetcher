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

#include "application.h"
#include "ui/uihandler.h"
#include "input/input.h"
#include "ui/display.h"
#include "database/database.h"
#include "themes/loading.h"
#include "config/configmenu.h"
#include "download/downloader.h"
#include "download/download.h"
#include "helper/path.h"
#include "list/list.h"
#include "enginecache/enginecache.h"
#include "themes/rendering.h"
#include "config/config.h"
#ifdef RL_DYNAMIC
#include "loader/dynamic.h"
#endif

void static initApp(app_t *app);

void static destroyApp(app_t *app);

rl_cache cacheHandler;

int main() {
    app_t app;
    sdl_init();
    initApp(&app);

    while (!app.quit) {
        input_processInputs(&app);
        if (app.renderingRequired) {
            LOG_TRACE("re-rendering the UI");
            app.renderingRequired = 0;
            uihandler_render(&app);
        }
        SDL_Delay(25);
    }

    destroyApp(&app);
    return 0;
}

void static initApp(app_t *app) {
    LOG_TRACE("initApp start");
    memset(app, 0, sizeof(app_t));
    app->renderingRequired = 1;

#ifdef RL_DYNAMIC
    loadLibraryFunctions();
#endif

    path_initRomfetchersHome();
    app->database.db = database_init();
    themes_init(app);

    cacheHandler.data = app;
    cacheHandler.isValid = enginecache_isCacheValid;
    cacheHandler.clear = enginecache_clear;
    cacheHandler.add = enginecache_addEntry;
    cacheHandler.touch = enginecache_updateTimestamp;
    cacheHandler.get = enginecache_getSearchResults;

    acll_t *hosters = rl_hoster_init(&cacheHandler);
    acll_t *systems = rl_systems_init();

    config_load(app->database.db, &app->config.active, &app->config.advanced.active, &app->config.resolution.active,
                &app->themes.active, hosters, systems);

    app->systems.active = acll_firstFilter(systems, ll_filterActive, NULL);
    app->systems.all = acll_first(systems);

    app->engine.all = hosters;

    app->config.cursor = app->config.active;
    app->config.resolution.cursor = app->config.resolution.active;
    app->config.advanced.cursor = app->config.advanced.active;

    display_init(app);
    input_init();

    themes_activate(app, app->themes.active);

    downloader_init(app);
    download_load(app);
    LOG_TRACE("initApp done");
}

void static destroyApp(app_t *app) {
    LOG_TRACE("destroyApp start");
    downloader_cancelAllDownloads(app);
    display_destroy(app);
    themes_destroy(app);

    rl_results_free(app->search.all);
    rl_hoster_free(app->engine.all);
    rl_system_free(app->systems.all);

    downloader_destroy(app);

    config_destroy(app->config.active, app->config.advanced.active, app->config.resolution.active);

    input_destroy();

    database_destroy(app->database.db);
    SDL_Quit();
    LOG_TRACE("destroyApp done");
}