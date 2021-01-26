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

#ifndef KEYBOARD_KEYBOARD_H
#define KEYBOARD_KEYBOARD_H

#include "../application.h"

void keyboard_processUp(app_t *app);

void keyboard_processDown(app_t *app);

void keyboard_processLeft(app_t *app);

void keyboard_processRight(app_t *app);

void keyboard_processSelect(app_t *app);

void keyboard_processBack(app_t *app);

void keyboard_processOtherButton(app_t *app, controller_t *state);

void keyboard_processOtherKey(app_t *app, SDL_Scancode scancode);

void keyboard_render(app_t *app);

window_t keyboard_stateTarget(app_t *app, uint8_t isSelectButton);

void keyboard_statePersist(app_t *app);

void keyboard_stateInit(app_t *app);

#endif
