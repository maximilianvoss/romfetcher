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

#ifndef DEFINITIONS_THEMES_H
#define DEFINITIONS_THEMES_H

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct theme_s {
    struct theme_s *prev;
    struct theme_s *next;
    char *name;
    char *fileReference;

    struct {
        SDL_Color background;
        SDL_Color field;
        SDL_Color fieldBackground;
        SDL_Color fieldHighlight;
        SDL_Color text;
        SDL_Color textHighlight;
        SDL_Color textInverted;
        SDL_Color modalBackground;
        SDL_Color modalBackgroundInactive;
        SDL_Color modalFrame;
        SDL_Color modalText;
        SDL_Color modalButtonFrame;
        SDL_Color modalButtonFrameActive;
    } colors;

    struct {
        char *backgroundPath;
        SDL_Texture *background;
        char *checkboxCheckedPath;
        SDL_Texture *checkboxChecked;
        char *checkboxUncheckedPath;
        SDL_Texture *checkboxUnchecked;
        char *selectorIconPath;
        SDL_Texture *selectorIcon;
        char *settingsIconPath;
        SDL_Texture *settingsIcon;
        char *downloadManagerIconPath;
        SDL_Texture *downloadManagerIcon;
        char *searchChevronPath;
        SDL_Texture *searchChevron;
    } images;

    struct {
        char *font;
        TTF_Font *font16;
        TTF_Font *font24;
        TTF_Font *font26;
        TTF_Font *font34;
    } fonts;
} theme_t;

#endif
