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

#ifndef _MODAL_MODAL_H
#define _MODAL_MODAL_H

#include "../application.h"

void modal_processUp(app_t *app);

void modal_processDown(app_t *app);

void modal_processLeft(app_t *app);

void modal_processRight(app_t *app);

void modal_processSelect(app_t *app);

void modal_processBack(app_t *app);

void modal_processOtherButton(app_t *app, controller_t *state);

void modal_processOtherKey(app_t *app, SDL_Scancode scancode);

void model_render(app_t *app);

#endif
