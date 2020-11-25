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

#include "fonts.h"

void fonts_init(app_t *app) {
    TTF_Init();

    app->fonts.huge = TTF_OpenFont(app->themes.active->font, 144);
    if (app->fonts.huge == NULL) {
        printf("Failed to load headlineFont! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }


    app->fonts.big = TTF_OpenFont(app->themes.active->font, 34);
    if (app->fonts.big == NULL) {
        printf("Failed to load headlineFont! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }

    app->fonts.medium = TTF_OpenFont(app->themes.active->font, 24);
    if (app->fonts.medium == NULL) {
        printf("Failed to load headlineFont! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }

    app->fonts.small = TTF_OpenFont(app->themes.active->font, 12);
    if (app->fonts.small == NULL) {
        printf("Failed to load headlineFont! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }
}

void fonts_destroy(app_t *app) {
    TTF_CloseFont(app->fonts.huge);
    TTF_CloseFont(app->fonts.big);
    TTF_CloseFont(app->fonts.medium);
    TTF_CloseFont(app->fonts.small);
}
