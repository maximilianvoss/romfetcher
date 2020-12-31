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
#include "../search/statesearch.h"
#include "statesystem.h"
#include "statekeyboard.h"
#include "../download/statedownload.h"
#include "../download/statedownloadmanager.h"
#include "../config/configstates.h"

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
            state_target = &configStateConfig_target;
            state_persist = &configStateConfig_persist;
            break;
        case window_config_advanced:
            state_target = &configStateAdvanced_target;
            state_persist = &configStateAdvanced_persist;
            break;
        case window_config_hoster:
            state_target = &configStateHoster_target;
            state_persist = &configStateHoster_persist;
            break;
        case window_config_resolution:
            state_target = &configStateResolution_target;
            state_persist = &configStateResolution_persist;
            break;
        case window_config_systems:
            state_target = &configStateSystems_target;
            state_persist = &configStateSystems_persist;
            break;
        case window_config_themes:
            state_target = &configStateTheme_target;
            state_persist = &configStateTheme_persit;
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
            return &configStateConfig_init;
        case window_config_advanced:
            return &configStateAdvanced_init;
        case window_config_hoster:
            return &configStateHoster_init;
        case window_config_resolution:
            return &configStateResolution_init;
        case window_config_systems:
            return &configStateSystems_init;
        case window_config_themes:
            return &configStateTheme_init;
    }
    return NULL;
}
