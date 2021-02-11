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

#ifndef _CONFIG_CONFIGSTATES_H
#define _CONFIG_CONFIGSTATES_H

#include "../application.h"

window_t config_stateTarget(app_t *app, uint8_t isSelectButton);

void config_statePersist(app_t *app);

void config_stateInit(app_t *app);

window_t advancedConfig_stateTarget(app_t *app, uint8_t isSelectButton);

void advancedConfig_statePersist(app_t *app);

void advancedConfig_stateInit(app_t *app);

window_t hosterConfig_stateTarget(app_t *app, uint8_t isSelectButton);

void hosterConfig_statePersist(app_t *app);

void hosterConfig_stateInit(app_t *app);

window_t resolutionConfig_stateTarget(app_t *app, uint8_t isSelectButton);

void resolutionConfig_statePersist(app_t *app);

void resolutioinConfig_stateInit(app_t *app);

window_t systemConfig_stateTarget(app_t *app, uint8_t isSelectButton);

void systemConfig_statePersist(app_t *app);

void systemConfig_stateInit(app_t *app);

window_t themeConfig_stateTarget(app_t *app, uint8_t isSelectButton);

void themeConfig_statePersist(app_t *app);

void themeConfig_stateInit(app_t *app);

#endif
