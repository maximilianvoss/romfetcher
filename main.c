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
#include "systems.h"

int main() {
    app_t app;
    memset(&app, 0, sizeof(app_t));

    initSDL(&app);
    initSystems();

    system_t *systems = getSystemList();
    app.search.systemActive = systems;

    uint8_t quit = 0;
    while (!quit) {
        quit = processInputs(&app);
        renderUi(&app);
        SDL_Delay(75);
    }

    cleanUp(&app);
    freeSystems();
    if ( app.search.results != NULL ) {
        freeResultList(app.search.results);
    }
    return 0;
}


