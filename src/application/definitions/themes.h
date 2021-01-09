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
        SDL_Color windowBackgroundActive;
        SDL_Color windowBackgroundInactive;
        SDL_Color windowCopyright;

        SDL_Color fieldTextInactive;
        SDL_Color fieldForegroundInactive;
        SDL_Color fieldBackgroundInactive;
        SDL_Color fieldTextActive;
        SDL_Color fieldForegroundActive;
        SDL_Color fieldBackgroundActive;

        SDL_Color buttonTextInactive;
        SDL_Color buttonForegroundInactive;
        SDL_Color buttonBackgroundInactive;
        SDL_Color buttonTextActive;
        SDL_Color buttonForegroundActive;
        SDL_Color buttonBackgroundActive;

        SDL_Color resultHeaderBackbround;
        SDL_Color resultHeaderForeground;
        SDL_Color resultHeaderText;
        SDL_Color resultDivider;
        SDL_Color resultRowTextActive;
        SDL_Color resultRowForegroundActive;
        SDL_Color resultRowBackgroundActive;
        SDL_Color resultRowTextInactive;
        SDL_Color resultRowForegroundInactive;
        SDL_Color resultRowBackgroundInactive;

        SDL_Color modalBackground;
        SDL_Color modalForeground;
        SDL_Color modalText;
        SDL_Color modalHeadText;
        SDL_Color modalHeadForeground;
        SDL_Color modalButtonTextInactive;
        SDL_Color modalButtonForegroundInactive;
        SDL_Color modalButtonBackgroundInactive;
        SDL_Color modalButtonTextActive;
        SDL_Color modalButtonForegroundActive;
        SDL_Color modalButtonBackgroundActive;

        SDL_Color iconForegroundColorInactive;
        SDL_Color iconBackgroundColorInactive;
        SDL_Color iconForegroundColorActive;
        SDL_Color iconBackgroundColorActive;
        SDL_Color iconDownloadsTextInactive;
        SDL_Color iconDownloadsCircleInactive;
        SDL_Color iconDownloadsTextActive;
        SDL_Color iconDownloadsCircleActive;

        SDL_Color keyboardText;
        SDL_Color keyboardActiveChar;

        SDL_Color dwnldsItemForegroundInactive;
        SDL_Color dwnldsItemBackgroundInactive;
        SDL_Color dwnldsItemTextInactive;
        SDL_Color dwnldsItemForegroundActive;
        SDL_Color dwnldsItemBackgroundActive;
        SDL_Color dwnldsItemTextActive;
        SDL_Color dwnldsBarForegroundInactive;
        SDL_Color dwnldsBarBackgroundInactive;
        SDL_Color dwnldsBarTextInactive;
        SDL_Color dwnldsBarProgressBarInactive;
        SDL_Color dwnldsBarForegroundActive;
        SDL_Color dwnldsBarBackgroundActive;
        SDL_Color dwnldsBarTextActive;
        SDL_Color dwnldsBarProgressBarActive;
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
