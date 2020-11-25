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

#ifndef UI_RENDERING_H
#define UI_RENDERING_H

#include "../application.h"

SDL_Texture *rendering_loadImage(app_t *app, char *filename);

SDL_Texture *rendering_memImage(app_t *app, void *data, int size);

void rendering_loadText(app_t *app, texture_t *texture, char *str, TTF_Font *font, SDL_Color *color);

void rendering_circle(app_t *app, int x, int y, int radius);

#endif
