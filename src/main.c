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
#include <pthread.h>
#include "structs.h"
#include "ui/uihandler.h"
#include "input/inputhandler.h"
#include "ui/core.h"
#include "engine/results.h"
#include "database/init.h"
#include "database/config.h"
#include "helper/path.h"
#include "themes/loading.h"
#include "config/init.h"
#include "engine/enginehandler.h"
#include "systems/systemhandler.h"
#include "ui/fonts.h"
#include "download/downloader.h"

static void *tidyUpThread(void *appPtr);

int main() {
    app_t app;
    memset(&app, 0, sizeof(app_t));

    path_initRomfetchersHome();
    database_init(&app);
    enginehandler_init(&app);
    systemhandler_init(&app);
    themes_init(&app);
    config_init(&app);
    fonts_init(&app);
    database_configLoad(&app);
    ui_init(&app);
    inputhandler_init();
    downloader_init(&app);

    while (!app.quit) {
        app.quit = inputhandler_processInputs(&app);
        uihandler_render(&app);
        SDL_Delay(75);
    }

    ui_destroy(&app);
    themes_destroy(&app);
    result_freeList(app.search.all);
    enginehandler_destroy(&app);
    systemhandler_destroy(&app);
    config_destroy(&app);
    inputhandler_destroy();
    fonts_destroy(&app);

    pthread_t tidyUpTread;
    if (pthread_create(&tidyUpTread, NULL, tidyUpThread, &app)) {
        SDL_Log("Error creating thread\n");
    }
    pthread_detach(tidyUpTread);

    SDL_Quit();

    return 0;
}


static void *tidyUpThread(void *appPtr) {
    app_t *app = (app_t *) appPtr;
    while (downloader_isActive(app)) {
        sleep(1);
    }
    downloader_destroy(app);
    database_destroy(app);

    return NULL;
}