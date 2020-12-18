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

#ifndef CONFIG_CONFIGSTATES_H
#define CONFIG_CONFIGSTATES_H

#include "../application.h"

window_t configStateConfig_target(app_t *app, uint8_t isSelectButton);

void configStateConfig_persist(app_t *app);

void configStateConfig_init(app_t *app);

window_t configStateAdvanced_target(app_t *app, uint8_t isSelectButton);

void configStateAdvanced_persist(app_t *app);

void configStateAdvanced_init(app_t *app);

window_t configStateHoster_target(app_t *app, uint8_t isSelectButton);

void configStateHoster_persist(app_t *app);

void configStateHoster_init(app_t *app);

window_t configStateResolution_target(app_t *app, uint8_t isSelectButton);

void configStateResolution_persist(app_t *app);

void configStateResolution_init(app_t *app);

window_t configStateSystems_target(app_t *app, uint8_t isSelectButton);

void configStateSystems_persist(app_t *app);

void configStateSystems_init(app_t *app);

window_t configStateTheme_target(app_t *app, uint8_t isSelectButton);

void configStateTheme_persit(app_t *app);

void configStateTheme_init(app_t *app);

#endif
