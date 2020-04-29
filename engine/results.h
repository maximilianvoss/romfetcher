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

#ifndef ENGINE_RESULTS_H
#define ENGINE_RESULTS_H

#include "../structs.h"

searchresult_t *result_addItemToList(searchresult_t *resultList, searchresult_t *newItem);

searchresult_t *result_newItem(system_t *system);

void result_setTitle(searchresult_t *resultList, char *title);

void result_setUrl(searchresult_t *resultList, char *url);

void result_freeList(searchresult_t *resultList);

uint32_t result_getListCount(searchresult_t *resultList);

#endif
