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

#ifndef _ENGINECACHE_ENGINECACHE_H
#define _ENGINECACHE_ENGINECACHE_H

#include <romlibrary.h>
#include <sqlite3.h>

void enginecache_init(sqlite3 *db);

void enginecache_initstate(sqlite3 *db);

uint8_t enginecache_isCacheValid(rl_hoster *hoster, rl_system *system, void *data);

void enginecache_clear(rl_hoster *hoster, rl_system *system, void *data);

void enginecache_addEntry(rl_hoster *hoster, rl_system *system, char *searchString, rl_result *entry, void *data);

void enginecache_updateTimestamp(rl_hoster *hoster, rl_system *system, void *data);

acll_t *enginecache_getSearchResults(rl_hoster *hoster, rl_system *system, char *searchString, void *data);

#endif
