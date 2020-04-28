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

int main() {
    app_t app;
    memset(&app, 0, sizeof(app_t));

    database_init(&app);
    ui_init(&app);

    system_t *systems = database_systemList(&app);
    app.search.systemActive = systems;

    uint8_t quit = 0;
    while (!quit) {
        quit = ui_processInputs(&app);
        ui_render(&app);
        SDL_Delay(75);
    }

    ui_destroy(&app);
    if ( app.search.results != NULL ) {
        freeResultList(app.search.results);
    }
    database_systemsDestroy();
    database_destroy(&app);
    return 0;
}


