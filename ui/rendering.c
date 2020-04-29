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

#include "rendering.h"
#include <SDL_image.h>

SDL_Texture *rendering_loadImage(app_t *app, char *filename) {
    SDL_Texture *texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    texture = IMG_LoadTexture(app->renderer, filename);
    return texture;
}

void rendering_loadText(app_t *app, texture_t *texture, char *str, TTF_Font *font, SDL_Color *color) {
    SDL_Surface *textSurface = TTF_RenderText_Blended(font, str, *color);
    if (textSurface != NULL) {
        texture->texture = SDL_CreateTextureFromSurface(app->renderer, textSurface);
        texture->w = textSurface->w;
        texture->h = textSurface->h;
        SDL_FreeSurface(textSurface);
    }
}
