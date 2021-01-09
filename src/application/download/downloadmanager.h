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

#ifndef DOWNLOAD_DOWNLOADMANAGER_H
#define DOWNLOAD_DOWNLOADMANAGER_H

#include "../application.h"

void downloadmanager_processUp(app_t *app);

void downloadmanager_processDown(app_t *app);

void downloadmanager_processLeft(app_t *app);

void downloadmanager_processRight(app_t *app);

void downloadmanager_processSelect(app_t *app);

void downloadmanager_processBack(app_t *app);

void downloadmanager_processOtherButton(app_t *app, controller_t *state);

void downloadmanager_processOtherKey(app_t *app, SDL_Scancode scancode);

window_t downloadmanager_stateTarget(app_t *app, uint8_t isSelectButton);

void downloadmanager_statePersist(app_t *app);

void downloadmanager_stateInit(app_t *app);

void downloadmanager_render(app_t *app);

#endif
