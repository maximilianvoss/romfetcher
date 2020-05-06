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

#ifndef THEMES_RENDERING_H
#define THEMES_RENDERING_H

#include "../structs.h"

enum theme_color {
    field,
    fieldBackground,
    fieldActive
};

void themes_setDrawColorField(app_t *app);

void themes_setDrawColorBackground(app_t *app, uint8_t active);

void themes_setDrawColor(app_t *app, enum theme_color color);

#endif