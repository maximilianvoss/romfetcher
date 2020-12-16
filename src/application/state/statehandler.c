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
#include "statesearch.h"
#include "statesystem.h"
#include "statekeyboard.h"
#include "statedownload.h"
#include "stateconfig.h"
#include "stateconfigengine.h"
#include "stateconfigsystems.h"
#include "stateconfigthemes.h"
#include "stateconfigadvanced.h"
#include "stateconfigresolution.h"
#include "statedownloadmanager.h"

static void *getInitFunction(window_t target);

static window_t (*state_target)(app_t *app, uint8_t isSelectButton);

static void (*state_persist)(app_t *app);

static void (*state_init)(app_t *app);

void statehandler_switch(app_t *app, uint8_t isSelectButton) {
    switch (app->win) {
        case window_search:
            state_target = &statesearch_target;
            state_persist = &statesearch_persist;
            break;
        case window_system:
            state_target = &statesystem_target;
            state_persist = &statesystem_persist;
            break;
        case window_keyboard:
            state_target = &statekeyboard_target;
            state_persist = &statekeyboard_persist;
            break;
        case window_download:
            state_target = &statedownload_target;
            state_persist = &statedownload_persist;
            break;
        case window_downloadMgr:
            state_target = &statedownloadmanager_target;
            state_persist = &statedownloadmanager_persist;
            break;
        case window_config:
            state_target = &stateconfig_target;
            state_persist = &stateconfig_persist;
            break;
        case window_config_advanced:
            state_target = &stateconfigadvanced_target;
            state_persist = &stateconfigadvanced_persist;
            break;
        case window_config_hoster:
            state_target = &stateconfigengine_target;
            state_persist = &stateconfigengine_persist;
            break;
        case window_config_resolution:
            state_target = &stateconfigresolution_target;
            state_persist = &stateconfigresolution_persist;
            break;
        case window_config_systems:
            state_target = &stateconfigsystems_target;
            state_persist = &stateconfigsystems_persist;
            break;
        case window_config_themes:
            state_target = &stateconfigthemes_target;
            state_persist = &stateconfigthemes_persist;
            break;
    }

    state_init = getInitFunction(state_target(app, isSelectButton));
    if (isSelectButton) {
        state_persist(app);
    }
    state_init(app);
}

static void *getInitFunction(window_t target) {
    switch (target) {
        case window_search:
            return &statesearch_init;
        case window_system:
            return &statesystem_init;
        case window_keyboard:
            return &statekeyboard_init;
        case window_download:
            return &statedownload_init;
        case window_downloadMgr:
            return &statedownloadmanager_init;
        case window_config:
            return &stateconfig_init;
        case window_config_advanced:
            return &stateconfigadvanced_init;
        case window_config_hoster:
            return &stateconfigengine_init;
        case window_config_resolution:
            return &stateconfigresolution_init;
        case window_config_systems:
            return &stateconfigsystems_init;
        case window_config_themes:
            return &stateconfigthemes_init;
    }
    return NULL;
}
