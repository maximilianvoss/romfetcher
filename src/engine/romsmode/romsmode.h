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

#ifndef ENGINE_ROMSMODE_ROMSMODE_H
#define ENGINE_ROMSMODE_ROMSMODE_H

#include "../../structs.h"

searchresult_t *romsmode_search(void *app, system_t *system, char *searchString);

void romsmode_download(void *app, searchresult_t *item, void (*callback)(void *app));

#endif
