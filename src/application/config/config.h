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

#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include <sqlite3.h>
#include <romfetcher.h>
#include "configmenu.h"
#include "../themes/themes.h"

void config_load(sqlite3 *db, configMenu_t **configMenu, advancedConfig_t **advancedConfigs,
                 resolutionConfig_t **resolutionConfigs, theme_t **themes,
                 hoster_t *hosters, system_t *systems);

void config_revert(sqlite3 *db, advancedConfig_t **advancedConfigs,
                   resolutionConfig_t **resolutionConfigs, theme_t **themes);

void
config_persist(sqlite3 *db, advancedConfig_t *advancedConfigs, resolutionConfig_t *resolutionConfigs,
               theme_t *themes,
               hoster_t *hosters, system_t *systems);

void config_destroy(advancedConfig_t *advancedConfigs, resolutionConfig_t *resolutionConfigs);

void config_set(advancedConfig_t *advancedConfigs, advancedConfigSetting_t setting, uint8_t value);

uint8_t config_get(advancedConfig_t *advancedConfigs, advancedConfigSetting_t setting);

#endif
