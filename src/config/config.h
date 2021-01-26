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

#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include <sqlite3.h>
#include <romlibrary.h>
#include "configmenu.h"
#include "../definitions/themes.h"

void config_load(sqlite3 *db, acll_t **configMenu, acll_t **advancedConfigs,
                 acll_t **resolutionConfigs, acll_t **themes,
                 acll_t *hosters, acll_t *systems);

void config_revert(sqlite3 *db, acll_t **advancedConfigs,
                   acll_t **resolutionConfigs, acll_t **themes);

void
config_persist(sqlite3 *db, acll_t *advancedConfigs, acll_t *resolutionConfigs,
               acll_t *themes,
               acll_t *hosters, acll_t *systems);

void config_destroy(acll_t *configMenu, acll_t *advancedConfigs, acll_t *resolutionConfigs);

void config_set(acll_t *advancedConfigs, advancedConfigSetting_t setting, uint8_t value);

uint8_t config_get(acll_t *advancedConfigs, advancedConfigSetting_t setting);

#endif
