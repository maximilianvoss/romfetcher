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

#ifndef UI_INPUTLIST_H
#define UI_INPUTLIST_H

#include "../structs.h"

void inputlist_processUp(app_t *app);

void inputlist_processDown(app_t *app);

void inputlist_processLeft(app_t *app);

void inputlist_processRight(app_t *app);

void inputlist_processSelect(app_t *app);

void inputlist_processBack(app_t *app);

void inputlist_processOtherButton(app_t *app, GameControllerState_t *state);

#endif
