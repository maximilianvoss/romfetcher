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

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
int rmask = 0xff000000;
int gmask = 0x00ff0000;
int bmask = 0x0000ff00;
int amask = 0x000000ff;
#else
int rmask = 0x000000ff;
int gmask = 0x0000ff00;
int bmask = 0x00ff0000;
int amask = 0xff000000;
#endif

SDL_Texture *rendering_loadImage(app_t *app, char *filename) {
    if (filename == NULL) {
        return NULL;
    }
    SDL_Texture *texture;
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
    texture = IMG_LoadTexture(app->sdlRenderer, filename);
    return texture;
}

void rendering_loadText(app_t *app, texture_t *texture, char *str, TTF_Font *font, SDL_Color *color) {
    char cpy[strlen(str) + 1];
    strcpy(cpy, str);

    SDL_Surface *blindSurface = NULL;

    char *token = strtok(cpy, "\n");
    while (token != NULL) {
        SDL_Surface *textSurface = TTF_RenderText_Blended(font, token, *color);

        if (blindSurface == NULL) {
            blindSurface = textSurface;
        } else {
            SDL_Surface *tmpSurface = SDL_CreateRGBSurface(
                    0,
                    (blindSurface->w >= textSurface->w) ? blindSurface->w : textSurface->w,
                    blindSurface->h + textSurface->h,
                    32,
                    rmask,
                    gmask,
                    bmask,
                    amask);

            SDL_Rect srcBlind = {0, 0, blindSurface->w, blindSurface->h};
            SDL_Rect destBlind = {0, 0, blindSurface->w, blindSurface->h};
            SDL_BlitSurface(blindSurface, &srcBlind, tmpSurface, &destBlind);

            SDL_Rect srcText = {0, 0, textSurface->w, textSurface->h};
            SDL_Rect destText = {0, blindSurface->h, textSurface->w, textSurface->h};
            SDL_BlitSurface(textSurface, &srcText, tmpSurface, &destText);

            SDL_FreeSurface(blindSurface);
            blindSurface = tmpSurface;
            SDL_FreeSurface(textSurface);
        }
        token = strtok(NULL, "\n");
    }

    if (blindSurface != NULL) {
        texture->texture = SDL_CreateTextureFromSurface(app->sdlRenderer, blindSurface);
        texture->w = blindSurface->w;
        texture->h = blindSurface->h;
        SDL_FreeSurface(blindSurface);
    }
}


void rendering_circle(app_t *app, int x, int y, int radius) {
    int point_x;
    int point_y;
    while (radius > 0) {
        for (int t = 0; t < 360; t++) {
            point_x = x + radius * cos(t);
            point_y = y + radius * sin(t);
            SDL_RenderDrawPoint(app->sdlRenderer, point_x, point_y);
        }

        radius--;
    }
}
