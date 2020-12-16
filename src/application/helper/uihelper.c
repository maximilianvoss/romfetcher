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

#include "uihelper.h"


uiElementRects_t uihelper_generateRectsFullScreenWidth(int x, int y, int windowWidth, int height) {
    uiElementRects_t element;
    element.content.x = x + 10;
    element.content.y = y + 3;
    element.content.w = windowWidth - 2 * (x + 10);
    element.content.h = height - 6;

    element.inner.x = x;
    element.inner.y = y;
    element.inner.w = windowWidth - 2 * x;
    element.inner.h = height;

    element.outter.x = x - 2;
    element.outter.y = y - 2;
    element.outter.w = windowWidth - 2 * x + 4;
    element.outter.h = height + 4;

    return element;
}

uiElementRects_t uihelper_generateRects(int x, int y, int width, int height) {
    uiElementRects_t element;
    element.content.x = x + 10;
    element.content.y = y + 3;
    element.content.w = width - 20;
    element.content.h = height - 6;

    element.inner.x = x;
    element.inner.y = y;
    element.inner.w = width;
    element.inner.h = height;

    element.outter.x = x - 2;
    element.outter.y = y - 2;
    element.outter.w = width + 4;
    element.outter.h = height + 4;

    return element;
}

void uihelper_noPaddingX(uiElementRects_t *element) {
    element->content.x -= 10;
    element->content.w += 20;
}

void uihelper_renderTexture(SDL_Renderer *sdlRenderer, texture_t *texture, SDL_Rect *target) {
    SDL_Rect srcQuad = {0, 0, (texture->w > target->w) ? target->w : texture->w, texture->h};
    SDL_Rect renderQuad = {target->x,
                           target->y,
                           (texture->w > target->w) ? target->w : texture->w,
                           target->h};
    SDL_RenderCopy(sdlRenderer, texture->texture, &srcQuad, &renderQuad);
}

void uihelper_renderTextureCentered(SDL_Renderer *sdlRenderer, texture_t *texture, SDL_Rect *target) {
    SDL_Rect srcQuad = {0, 0, (texture->w > target->w) ? target->w : texture->w, texture->h};
    SDL_Rect renderQuad = {(texture->w > target->w) ? target->x : target->x + (target->w - texture->w) / 2,
                           target->y,
                           (texture->w > target->w) ? target->w : texture->w,
                           target->h};
    SDL_RenderCopy(sdlRenderer, texture->texture, &srcQuad, &renderQuad);
}

void uihelper_renderTextureRight(SDL_Renderer *sdlRenderer, texture_t *texture, SDL_Rect *target) {
    SDL_Rect srcQuad = {0, 0, (texture->w > target->w) ? target->w : texture->w, texture->h};
    SDL_Rect renderQuad = {(texture->w > target->w) ? target->x : target->x + target->w - texture->w - 3,
                           target->y,
                           (texture->w > target->w) ? target->w : texture->w,
                           target->h};
    SDL_RenderCopy(sdlRenderer, texture->texture, &srcQuad, &renderQuad);
}

void uihelper_renderSDLTexture(SDL_Renderer *sdlRenderer, SDL_Texture *texture, SDL_Rect *target) {
    SDL_Rect renderQuad = {target->x, target->y, target->w, target->h};
    SDL_RenderCopy(sdlRenderer, texture, NULL, &renderQuad);
}