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

#include "structs.h"
#include "ui/uihandler.h"
#include "ui/inputhandler.h"
#include "ui/core.h"
#include "engine/results.h"
#include "database/init.h"
#include "database/sytems.h"
#include "database/config.h"
#include "helper/path.h"
#include "themes/themes.h"

int main() {
    app_t app;
    memset(&app, 0, sizeof(app_t));

    path_initRomfetchersHome();
    database_init(&app);
    themes_init(&app);
    ui_init(&app);

    database_configLoad(&app);
    app.systems.all = database_systemList(&app, 0);
    app.systems.enabled = database_systemList(&app, 1);

    uint8_t quit = 0;
    while (!quit) {
        quit = inputhandler_processInputs(&app);
        uihandler_render(&app);
        SDL_Delay(75);
    }

    ui_destroy(&app);
    themes_destroy(&app);
    if ( app.search.results != NULL ) {
        result_freeList(app.search.results);
    }
    database_systemsDestroy(app.systems.all);
    database_systemsDestroy(app.systems.enabled);
    database_destroy(&app);

    return 0;
}


