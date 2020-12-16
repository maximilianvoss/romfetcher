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

#ifndef HELPER_UIHELPER_H
#define HELPER_UIHELPER_H

#include <SDL_rect.h>
#include "../application.h"

typedef struct uiElementRects_s {
    SDL_Rect content;
    SDL_Rect inner;
    SDL_Rect outter;
} uiElementRects_t;

uiElementRects_t uihelper_generateRectsFullScreenWidth(int x, int y, int windowWidth, int height);

uiElementRects_t uihelper_generateRects(int x, int y, int width, int height);

void uihelper_noPaddingX(uiElementRects_t *element);

void uihelper_renderTexture(SDL_Renderer *sdlRenderer, texture_t *texture, SDL_Rect *target);

void uihelper_renderTextureCentered(SDL_Renderer *sdlRenderer, texture_t *texture, SDL_Rect *target);

void uihelper_renderTextureRight(SDL_Renderer *sdlRenderer, texture_t *texture, SDL_Rect *target);

void uihelper_renderSDLTexture(SDL_Renderer *sdlRenderer, SDL_Texture *texture, SDL_Rect *target);

#endif
