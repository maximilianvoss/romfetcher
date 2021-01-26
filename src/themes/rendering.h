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

#ifndef THEMES_RENDERING_H
#define THEMES_RENDERING_H

#include "../application.h"

enum theme_color {
    windowBackgroundActive,
    windowBackgroundInactive,
    windowCopyright,

    fieldTextInactive,
    fieldForegroundInactive,
    fieldBackgroundInactive,
    fieldTextActive,
    fieldForegroundActive,
    fieldBackgroundActive,

    resultHeaderBackbround,
    resultHeaderForeground,
    resultHeaderText,
    resultDivider,
    resultRowTextActive,
    resultRowForegroundActive,
    resultRowBackgroundActive,
    resultRowTextInactive,
    resultRowForegroundInactive,
    resultRowBackgroundInactive,

    modalBackground,
    modalForeground,
    modalHeadText,
    modalHeadForeground,
    modalButtonTextInactive,
    modalButtonForegroundInactive,
    modalButtonBackgroundInactive,
    modalButtonTextActive,
    modalButtonForegroundActive,
    modalButtonBackgroundActive,

    iconForegroundColorInactive,
    iconBackgroundColorInactive,
    iconForegroundColorActive,
    iconBackgroundColorActive,

    iconDownloadsTextInactive,
    iconDownloadsCircleInactive,
    iconDownloadsTextActive,
    iconDownloadsCircleActive,

    keyboardText,
    keyboardActiveChar,

    dwnldsItemForegroundInactive,
    dwnldsItemBackgroundInactive,
    dwnldsItemTextInactive,
    dwnldsItemForegroundActive,
    dwnldsItemBackgroundActive,
    dwnldsItemTextActive,
    dwnldsBarForegroundInactive,
    dwnldsBarBackgroundInactive,
    dwnldsBarTextInactive,
    dwnldsBarProgressBarInactive,
    dwnldsBarForegroundActive,
    dwnldsBarBackgroundActive,
    dwnldsBarTextActive,
    dwnldsBarProgressBarActive
};

void themes_setDrawColorFieldForeground(app_t *app, uint8_t active);

void themes_setDrawColorFieldBackground(app_t *app, uint8_t active);

SDL_Color *themes_getDrawColorFieldText(app_t *app, uint8_t active);

void themes_setDrawColorResultForeground(app_t *app, uint8_t active);

void themes_setDrawColorResultBackground(app_t *app, uint8_t active);

SDL_Color *themes_getDrawColorResultText(app_t *app, uint8_t active);

void themes_setDrawColorDwnItemForeground(app_t *app, uint8_t active);

void themes_setDrawColorDwnItemBackground(app_t *app, uint8_t active);

SDL_Color *themes_getDrawColorDwnItemText(app_t *app, uint8_t active);

void themes_setDrawColorDwnProgressForeground(app_t *app, uint8_t active);

void themes_setDrawColorDwnProgressBackground(app_t *app, uint8_t active);

void themes_setDrawColorDwnProgressState(app_t *app, uint8_t active);

SDL_Color *themes_getDrawColorDwnProgressText(app_t *app, uint8_t active);

SDL_Color *themes_getDrawColor(app_t *app, enum theme_color color);

void themes_setDrawColor(app_t *app, enum theme_color color);

void themes_activate(app_t *app, acll_t *theme);

#endif