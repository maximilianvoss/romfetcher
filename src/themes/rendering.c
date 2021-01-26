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

#include <SDL_render.h>
#include "rendering.h"
#include "../ui/rendering.h"

void themes_setDrawColorFieldForeground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, fieldForegroundActive);
    } else {
        themes_setDrawColor(app, fieldForegroundInactive);
    }
}

void themes_setDrawColorFieldBackground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, fieldBackgroundActive);
    } else {
        themes_setDrawColor(app, fieldBackgroundInactive);
    }
}

SDL_Color *themes_getDrawColorFieldText(app_t *app, uint8_t active) {
    if (active) {
        return themes_getDrawColor(app, fieldTextActive);
    } else {
        return themes_getDrawColor(app, fieldTextInactive);
    }
}

void themes_setDrawColorResultForeground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, resultRowForegroundActive);
    } else {
        themes_setDrawColor(app, resultRowForegroundInactive);
    }
}

void themes_setDrawColorResultBackground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, resultRowBackgroundActive);
    } else {
        themes_setDrawColor(app, resultRowBackgroundInactive);
    }
}

SDL_Color *themes_getDrawColorResultText(app_t *app, uint8_t active) {
    if (active) {
        return themes_getDrawColor(app, resultRowTextActive);
    } else {
        return themes_getDrawColor(app, resultRowTextInactive);
    }
}

void themes_setDrawColorDwnItemForeground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, dwnldsItemForegroundActive);
    } else {
        themes_setDrawColor(app, dwnldsItemForegroundInactive);
    }
}

void themes_setDrawColorDwnItemBackground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, dwnldsItemBackgroundActive);
    } else {
        themes_setDrawColor(app, dwnldsItemBackgroundInactive);
    }
}

SDL_Color *themes_getDrawColorDwnItemText(app_t *app, uint8_t active) {
    if (active) {
        return themes_getDrawColor(app, dwnldsItemTextActive);
    } else {
        return themes_getDrawColor(app, dwnldsItemTextInactive);
    }
}

void themes_setDrawColorDwnProgressForeground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, dwnldsBarForegroundActive);
    } else {
        themes_setDrawColor(app, dwnldsBarForegroundInactive);
    }
}

void themes_setDrawColorDwnProgressBackground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, dwnldsBarBackgroundActive);
    } else {
        themes_setDrawColor(app, dwnldsBarBackgroundInactive);
    }
}

void themes_setDrawColorDwnProgressState(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, dwnldsBarProgressBarActive);
    } else {
        themes_setDrawColor(app, dwnldsBarProgressBarInactive);
    }
}

SDL_Color *themes_getDrawColorDwnProgressText(app_t *app, uint8_t active) {
    if (active) {
        return themes_getDrawColor(app, dwnldsBarTextActive);
    } else {
        return themes_getDrawColor(app, dwnldsBarTextInactive);
    }
}

SDL_Color *themes_getDrawColor(app_t *app, enum theme_color color) {
    SDL_Color *activeColor;

    switch (color) {
        case windowBackgroundActive:
            activeColor = &getActiveTheme(app)->colors.windowBackgroundActive;
            break;
        case windowBackgroundInactive:
            activeColor = &getActiveTheme(app)->colors.windowBackgroundInactive;
            break;
        case windowCopyright:
            activeColor = &getActiveTheme(app)->colors.windowCopyright;
            break;
        case fieldTextInactive:
            activeColor = &getActiveTheme(app)->colors.fieldTextInactive;
            break;
        case fieldForegroundInactive:
            activeColor = &getActiveTheme(app)->colors.fieldForegroundInactive;
            break;
        case fieldBackgroundInactive:
            activeColor = &getActiveTheme(app)->colors.fieldBackgroundInactive;
            break;
        case fieldTextActive:
            activeColor = &getActiveTheme(app)->colors.fieldTextActive;
            break;
        case fieldForegroundActive:
            activeColor = &getActiveTheme(app)->colors.fieldForegroundActive;
            break;
        case fieldBackgroundActive:
            activeColor = &getActiveTheme(app)->colors.fieldBackgroundActive;
            break;
        case resultHeaderBackbround:
            activeColor = &getActiveTheme(app)->colors.resultHeaderBackbround;
            break;
        case resultHeaderForeground:
            activeColor = &getActiveTheme(app)->colors.resultHeaderForeground;
            break;
        case resultHeaderText:
            activeColor = &getActiveTheme(app)->colors.resultHeaderText;
            break;
        case resultDivider:
            activeColor = &getActiveTheme(app)->colors.resultDivider;
            break;
        case resultRowTextActive:
            activeColor = &getActiveTheme(app)->colors.resultRowTextActive;
            break;
        case resultRowForegroundActive:
            activeColor = &getActiveTheme(app)->colors.resultRowForegroundActive;
            break;
        case resultRowBackgroundActive:
            activeColor = &getActiveTheme(app)->colors.resultRowBackgroundActive;
            break;
        case resultRowTextInactive:
            activeColor = &getActiveTheme(app)->colors.resultRowTextInactive;
            break;
        case resultRowForegroundInactive:
            activeColor = &getActiveTheme(app)->colors.resultRowForegroundInactive;
            break;
        case resultRowBackgroundInactive:
            activeColor = &getActiveTheme(app)->colors.resultRowBackgroundInactive;
            break;
        case modalBackground:
            activeColor = &getActiveTheme(app)->colors.modalBackground;
            break;
        case modalForeground:
            activeColor = &getActiveTheme(app)->colors.modalForeground;
            break;
        case modalHeadText:
            activeColor = &getActiveTheme(app)->colors.modalHeadText;
            break;
        case modalHeadForeground:
            activeColor = &getActiveTheme(app)->colors.modalHeadForeground;
            break;
        case modalButtonTextInactive:
            activeColor = &getActiveTheme(app)->colors.modalButtonTextInactive;
            break;
        case modalButtonForegroundInactive:
            activeColor = &getActiveTheme(app)->colors.modalButtonForegroundInactive;
            break;
        case modalButtonBackgroundInactive:
            activeColor = &getActiveTheme(app)->colors.modalButtonBackgroundInactive;
            break;
        case modalButtonTextActive:
            activeColor = &getActiveTheme(app)->colors.modalButtonTextActive;
            break;
        case modalButtonForegroundActive:
            activeColor = &getActiveTheme(app)->colors.modalButtonForegroundActive;
            break;
        case modalButtonBackgroundActive:
            activeColor = &getActiveTheme(app)->colors.modalButtonBackgroundActive;
            break;
        case iconForegroundColorInactive:
            activeColor = &getActiveTheme(app)->colors.iconForegroundColorInactive;
            break;
        case iconBackgroundColorInactive:
            activeColor = &getActiveTheme(app)->colors.iconBackgroundColorInactive;
            break;
        case iconForegroundColorActive:
            activeColor = &getActiveTheme(app)->colors.iconForegroundColorActive;
            break;
        case iconBackgroundColorActive:
            activeColor = &getActiveTheme(app)->colors.iconBackgroundColorActive;
            break;
        case iconDownloadsTextInactive:
            activeColor = &getActiveTheme(app)->colors.iconDownloadsTextInactive;
            break;
        case iconDownloadsCircleInactive:
            activeColor = &getActiveTheme(app)->colors.iconDownloadsCircleInactive;
            break;
        case iconDownloadsTextActive:
            activeColor = &getActiveTheme(app)->colors.iconDownloadsTextActive;
            break;
        case iconDownloadsCircleActive:
            activeColor = &getActiveTheme(app)->colors.iconDownloadsCircleActive;
            break;
        case keyboardText:
            activeColor = &getActiveTheme(app)->colors.keyboardText;
            break;
        case keyboardActiveChar:
            activeColor = &getActiveTheme(app)->colors.keyboardActiveChar;
            break;
        case dwnldsItemForegroundInactive:
            activeColor = &getActiveTheme(app)->colors.dwnldsItemForegroundInactive;
            break;
        case dwnldsItemBackgroundInactive:
            activeColor = &getActiveTheme(app)->colors.dwnldsItemBackgroundInactive;
            break;
        case dwnldsItemTextInactive:
            activeColor = &getActiveTheme(app)->colors.dwnldsItemTextInactive;
            break;
        case dwnldsItemForegroundActive:
            activeColor = &getActiveTheme(app)->colors.dwnldsItemForegroundActive;
            break;
        case dwnldsItemBackgroundActive:
            activeColor = &getActiveTheme(app)->colors.dwnldsItemBackgroundActive;
            break;
        case dwnldsItemTextActive:
            activeColor = &getActiveTheme(app)->colors.dwnldsItemTextActive;
            break;
        case dwnldsBarForegroundInactive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarForegroundInactive;
            break;
        case dwnldsBarBackgroundInactive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarBackgroundInactive;
            break;
        case dwnldsBarTextInactive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarTextInactive;
            break;
        case dwnldsBarProgressBarInactive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarProgressBarInactive;
            break;
        case dwnldsBarForegroundActive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarForegroundActive;
            break;
        case dwnldsBarBackgroundActive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarBackgroundActive;
            break;
        case dwnldsBarTextActive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarTextActive;
            break;
        case dwnldsBarProgressBarActive:
            activeColor = &getActiveTheme(app)->colors.dwnldsBarProgressBarActive;
            break;
        default:
            activeColor = NULL;
            LOG_ERROR("Color not found");
    }
    return activeColor;
}

void themes_setDrawColor(app_t *app, enum theme_color color) {
    SDL_Color *activeColor;
    SDL_Color defaultColor;
    memset(&defaultColor, 0xFF, sizeof(SDL_Color));

    activeColor = themes_getDrawColor(app, color);
    if (activeColor == NULL) {
        activeColor = &defaultColor;
    }
    SDL_SetRenderDrawColor(app->sdlRenderer, activeColor->r, activeColor->g, activeColor->b, activeColor->a);
}

static void themes_unloadTheme(app_t *app) {
    SDL_DestroyTexture(getActiveTheme(app)->images.background);
    SDL_DestroyTexture(getActiveTheme(app)->images.searchChevron);
    SDL_DestroyTexture(getActiveTheme(app)->images.settingsIcon);
    SDL_DestroyTexture(getActiveTheme(app)->images.checkboxChecked);
    SDL_DestroyTexture(getActiveTheme(app)->images.checkboxUnchecked);
    SDL_DestroyTexture(getActiveTheme(app)->images.downloadManagerIcon);

    TTF_CloseFont(getActiveTheme(app)->fonts.font24);
    TTF_CloseFont(getActiveTheme(app)->fonts.font34);
    TTF_CloseFont(getActiveTheme(app)->fonts.font26);
    TTF_CloseFont(getActiveTheme(app)->fonts.font16);
}

static void themes_initTheme(app_t *app) {
    getActiveTheme(app)->images.background = rendering_loadImage(app->sdlRenderer,
                                                                 getActiveTheme(app)->images.backgroundPath);
    getActiveTheme(app)->images.searchChevron = rendering_loadImage(app->sdlRenderer,
                                                                    getActiveTheme(app)->images.selectorIconPath);
    getActiveTheme(app)->images.settingsIcon = rendering_loadImage(app->sdlRenderer,
                                                                   getActiveTheme(app)->images.settingsIconPath);
    getActiveTheme(app)->images.checkboxChecked = rendering_loadImage(app->sdlRenderer,
                                                                      getActiveTheme(app)->images.checkboxCheckedPath);
    getActiveTheme(app)->images.checkboxUnchecked = rendering_loadImage(app->sdlRenderer,
                                                                        getActiveTheme(
                                                                                app)->images.checkboxUncheckedPath);
    getActiveTheme(app)->images.downloadManagerIcon = rendering_loadImage(app->sdlRenderer,
                                                                          getActiveTheme(
                                                                                  app)->images.downloadManagerIconPath);

    getActiveTheme(app)->fonts.font24 = TTF_OpenFont(getActiveTheme(app)->fonts.font, 16);
    if (getActiveTheme(app)->fonts.font24 == NULL) {
        LOG_ERROR("Failed to load font24  SDL_ttf Error: %s", TTF_GetError());
    }

    getActiveTheme(app)->fonts.font34 = TTF_OpenFont(getActiveTheme(app)->fonts.font, 34);
    if (getActiveTheme(app)->fonts.font34 == NULL) {
        LOG_ERROR("Failed to load font34 SDL_ttf Error: %s", TTF_GetError());
    }

    getActiveTheme(app)->fonts.font26 = TTF_OpenFont(getActiveTheme(app)->fonts.font, 26);
    if (getActiveTheme(app)->fonts.font26 == NULL) {
        LOG_ERROR("Failed to load font26 SDL_ttf Error: %s", TTF_GetError());
    }

    getActiveTheme(app)->fonts.font16 = TTF_OpenFont(getActiveTheme(app)->fonts.font, 12);
    if (getActiveTheme(app)->fonts.font16 == NULL) {
        LOG_ERROR("Failed to load font16 SDL_ttf Error: %s", TTF_GetError());
    }
}

void themes_activate(app_t *app, acll_t *theme) {
    if (app->themes.active != NULL) {
        themes_unloadTheme(app);
    }
    if (theme == NULL) {
        app->themes.active = app->themes.all;
    } else {
        app->themes.active = theme;
    }
    themes_initTheme(app);
}