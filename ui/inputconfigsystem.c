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

#include "inputconfigsystem.h"
#include "statehandler.h"

void inputconfigsystem_processUp(app_t *app) {
    if (app->systems.cursor->prev != NULL) {
        app->systems.cursor = app->systems.cursor->prev;
    }
}

void inputconfigsystem_processDown(app_t *app) {
    if (app->systems.cursor->next != NULL) {
        app->systems.cursor = app->systems.cursor->next;
    }
}

void inputconfigsystem_processLeft(app_t *app) {
    if (app->systems.cursor->prev != NULL) {
        app->systems.cursor = app->systems.cursor->prev;
    }
}

void inputconfigsystem_processRight(app_t *app) {
    if (app->systems.cursor->next != NULL) {
        app->systems.cursor = app->systems.cursor->next;
    }
}

void inputconfigsystem_processSelect(app_t *app) {
    if (app->systems.cursor->active) {
        app->systems.cursor->active = 0;
    } else {
        app->systems.cursor->active = 1;
    }
}

void inputconfigsystem_processBack(app_t *app) {
    statehandler_change(app, window_config);
}

void inputconfigsystem_processOtherButton(app_t *app, GameControllerState_t *state) {
}