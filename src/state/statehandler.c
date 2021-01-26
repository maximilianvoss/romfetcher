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

#include "statehandler.h"
#include "../search/search.h"
#include "statesystem.h"
#include "../input/keyboard.h"
#include "../download/downloadmanager.h"
#include "../config/configstates.h"

static void *getInitFunction(window_t target);

static window_t (*state_target)(app_t *app, uint8_t isSelectButton);

static void (*state_persist)(app_t *app);

static void (*state_init)(app_t *app);

void statehandler_switch(app_t *app, uint8_t isSelectButton) {
    switch (app->win) {
        case window_search:
            state_target = &search_stateTarget;
            state_persist = &search_statePersist;
            break;
        case window_system:
            state_target = &staterl_systemarget;
            state_persist = &statesystem_persist;
            break;
        case window_keyboard:
            state_target = &keyboard_stateTarget;
            state_persist = &keyboard_statePersist;
            break;
        case window_downloadMgr:
            state_target = &downloadmanager_stateTarget;
            state_persist = &downloadmanager_statePersist;
            break;
        case window_config:
            state_target = &config_stateTarget;
            state_persist = &config_statePersist;
            break;
        case window_config_advanced:
            state_target = &advancedConfig_stateTarget;
            state_persist = &advancedConfig_statePersist;
            break;
        case window_config_hoster:
            state_target = &hosterConfig_stateTarget;
            state_persist = &hosterConfig_statePersist;
            break;
        case window_config_resolution:
            state_target = &resolutionConfig_stateTarget;
            state_persist = &resolutionConfig_statePersist;
            break;
        case window_config_systems:
            state_target = &systemConfig_stateTarget;
            state_persist = &systemConfig_statePersist;
            break;
        case window_config_themes:
            state_target = &themeConfig_stateTarget;
            state_persist = &themeConfig_statePersist;
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
            return &search_stateInit;
        case window_system:
            return &statesystem_init;
        case window_keyboard:
            return &keyboard_stateInit;
        case window_downloadMgr:
            return &downloadmanager_stateInit;
        case window_config:
            return &config_stateInit;
        case window_config_advanced:
            return &advancedConfig_stateInit;
        case window_config_hoster:
            return &hosterConfig_stateInit;
        case window_config_resolution:
            return &resolutioinConfig_stateInit;
        case window_config_systems:
            return &systemConfig_stateInit;
        case window_config_themes:
            return &themeConfig_stateInit;
    }
    return NULL;
}
