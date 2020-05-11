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

#include "inputconfig.h"
#include "statehandler.h"


void inputconfig_processUp(app_t *app) {
    switch (app->config.cursor) {
        case config_engine:
            break;
        case config_systems:
            app->config.cursor = config_engine;
            break;
    }
}

void inputconfig_processDown(app_t *app) {
    switch (app->config.cursor) {
        case config_engine:
            app->config.cursor = config_systems;
            break;
        case config_systems:
            break;
    }
}

void inputconfig_processLeft(app_t *app) {
}

void inputconfig_processRight(app_t *app) {
}

void inputconfig_processSelect(app_t *app) {
    switch (app->config.cursor) {
        case config_engine:
            statehandler_change(app, window_config_engine);
            break;
        case config_systems:
            statehandler_change(app, window_config_systems);
            break;
    }
}

void inputconfig_processBack(app_t *app) {
    app->win = window_search;
}

void inputconfig_processOtherButton(app_t *app, GameControllerState_t *state) {

}