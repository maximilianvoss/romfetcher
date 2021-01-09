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
            activeColor = &app->themes.active->colors.windowBackgroundActive;
            break;
        case windowBackgroundInactive:
            activeColor = &app->themes.active->colors.windowBackgroundInactive;
            break;
        case windowCopyright:
            activeColor = &app->themes.active->colors.windowCopyright;
            break;
        case fieldTextInactive:
            activeColor = &app->themes.active->colors.fieldTextInactive;
            break;
        case fieldForegroundInactive:
            activeColor = &app->themes.active->colors.fieldForegroundInactive;
            break;
        case fieldBackgroundInactive:
            activeColor = &app->themes.active->colors.fieldBackgroundInactive;
            break;
        case fieldTextActive:
            activeColor = &app->themes.active->colors.fieldTextActive;
            break;
        case fieldForegroundActive:
            activeColor = &app->themes.active->colors.fieldForegroundActive;
            break;
        case fieldBackgroundActive:
            activeColor = &app->themes.active->colors.fieldBackgroundActive;
            break;
        case resultHeaderBackbround:
            activeColor = &app->themes.active->colors.resultHeaderBackbround;
            break;
        case resultHeaderForeground:
            activeColor = &app->themes.active->colors.resultHeaderForeground;
            break;
        case resultHeaderText:
            activeColor = &app->themes.active->colors.resultHeaderText;
            break;
        case resultDivider:
            activeColor = &app->themes.active->colors.resultDivider;
            break;
        case resultRowTextActive:
            activeColor = &app->themes.active->colors.resultRowTextActive;
            break;
        case resultRowForegroundActive:
            activeColor = &app->themes.active->colors.resultRowForegroundActive;
            break;
        case resultRowBackgroundActive:
            activeColor = &app->themes.active->colors.resultRowBackgroundActive;
            break;
        case resultRowTextInactive:
            activeColor = &app->themes.active->colors.resultRowTextInactive;
            break;
        case resultRowForegroundInactive:
            activeColor = &app->themes.active->colors.resultRowForegroundInactive;
            break;
        case resultRowBackgroundInactive:
            activeColor = &app->themes.active->colors.resultRowBackgroundInactive;
            break;
        case modalBackground:
            activeColor = &app->themes.active->colors.modalBackground;
            break;
        case modalForeground:
            activeColor = &app->themes.active->colors.modalForeground;
            break;
        case modalHeadText:
            activeColor = &app->themes.active->colors.modalHeadText;
            break;
        case modalHeadForeground:
            activeColor = &app->themes.active->colors.modalHeadForeground;
            break;
        case modalButtonTextInactive:
            activeColor = &app->themes.active->colors.modalButtonTextInactive;
            break;
        case modalButtonForegroundInactive:
            activeColor = &app->themes.active->colors.modalButtonForegroundInactive;
            break;
        case modalButtonBackgroundInactive:
            activeColor = &app->themes.active->colors.modalButtonBackgroundInactive;
            break;
        case modalButtonTextActive:
            activeColor = &app->themes.active->colors.modalButtonTextActive;
            break;
        case modalButtonForegroundActive:
            activeColor = &app->themes.active->colors.modalButtonForegroundActive;
            break;
        case modalButtonBackgroundActive:
            activeColor = &app->themes.active->colors.modalButtonBackgroundActive;
            break;
        case iconForegroundColorInactive:
            activeColor = &app->themes.active->colors.iconForegroundColorInactive;
            break;
        case iconBackgroundColorInactive:
            activeColor = &app->themes.active->colors.iconBackgroundColorInactive;
            break;
        case iconForegroundColorActive:
            activeColor = &app->themes.active->colors.iconForegroundColorActive;
            break;
        case iconBackgroundColorActive:
            activeColor = &app->themes.active->colors.iconBackgroundColorActive;
            break;
        case iconDownloadsTextInactive:
            activeColor = &app->themes.active->colors.iconDownloadsTextInactive;
            break;
        case iconDownloadsCircleInactive:
            activeColor = &app->themes.active->colors.iconDownloadsCircleInactive;
            break;
        case iconDownloadsTextActive:
            activeColor = &app->themes.active->colors.iconDownloadsTextActive;
            break;
        case iconDownloadsCircleActive:
            activeColor = &app->themes.active->colors.iconDownloadsCircleActive;
            break;
        case keyboardText:
            activeColor = &app->themes.active->colors.keyboardText;
            break;
        case keyboardActiveChar:
            activeColor = &app->themes.active->colors.keyboardActiveChar;
            break;
        case dwnldsItemForegroundInactive:
            activeColor = &app->themes.active->colors.dwnldsItemForegroundInactive;
            break;
        case dwnldsItemBackgroundInactive:
            activeColor = &app->themes.active->colors.dwnldsItemBackgroundInactive;
            break;
        case dwnldsItemTextInactive:
            activeColor = &app->themes.active->colors.dwnldsItemTextInactive;
            break;
        case dwnldsItemForegroundActive:
            activeColor = &app->themes.active->colors.dwnldsItemForegroundActive;
            break;
        case dwnldsItemBackgroundActive:
            activeColor = &app->themes.active->colors.dwnldsItemBackgroundActive;
            break;
        case dwnldsItemTextActive:
            activeColor = &app->themes.active->colors.dwnldsItemTextActive;
            break;
        case dwnldsBarForegroundInactive:
            activeColor = &app->themes.active->colors.dwnldsBarForegroundInactive;
            break;
        case dwnldsBarBackgroundInactive:
            activeColor = &app->themes.active->colors.dwnldsBarBackgroundInactive;
            break;
        case dwnldsBarTextInactive:
            activeColor = &app->themes.active->colors.dwnldsBarTextInactive;
            break;
        case dwnldsBarProgressBarInactive:
            activeColor = &app->themes.active->colors.dwnldsBarProgressBarInactive;
            break;
        case dwnldsBarForegroundActive:
            activeColor = &app->themes.active->colors.dwnldsBarForegroundActive;
            break;
        case dwnldsBarBackgroundActive:
            activeColor = &app->themes.active->colors.dwnldsBarBackgroundActive;
            break;
        case dwnldsBarTextActive:
            activeColor = &app->themes.active->colors.dwnldsBarTextActive;
            break;
        case dwnldsBarProgressBarActive:
            activeColor = &app->themes.active->colors.dwnldsBarProgressBarActive;
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
    SDL_DestroyTexture(app->themes.active->images.background);
    SDL_DestroyTexture(app->themes.active->images.searchChevron);
    SDL_DestroyTexture(app->themes.active->images.settingsIcon);
    SDL_DestroyTexture(app->themes.active->images.checkboxChecked);
    SDL_DestroyTexture(app->themes.active->images.checkboxUnchecked);
    SDL_DestroyTexture(app->themes.active->images.downloadManagerIcon);

    TTF_CloseFont(app->themes.active->fonts.font24);
    TTF_CloseFont(app->themes.active->fonts.font34);
    TTF_CloseFont(app->themes.active->fonts.font26);
    TTF_CloseFont(app->themes.active->fonts.font16);
}

static void themes_initTheme(app_t *app) {
    app->themes.active->images.background = rendering_loadImage(app->sdlRenderer,
                                                                app->themes.active->images.backgroundPath);
    app->themes.active->images.searchChevron = rendering_loadImage(app->sdlRenderer,
                                                                   app->themes.active->images.selectorIconPath);
    app->themes.active->images.settingsIcon = rendering_loadImage(app->sdlRenderer,
                                                                  app->themes.active->images.settingsIconPath);
    app->themes.active->images.checkboxChecked = rendering_loadImage(app->sdlRenderer,
                                                                     app->themes.active->images.checkboxCheckedPath);
    app->themes.active->images.checkboxUnchecked = rendering_loadImage(app->sdlRenderer,
                                                                       app->themes.active->images.checkboxUncheckedPath);
    app->themes.active->images.downloadManagerIcon = rendering_loadImage(app->sdlRenderer,
                                                                         app->themes.active->images.downloadManagerIconPath);

    app->themes.active->fonts.font24 = TTF_OpenFont(app->themes.active->fonts.font, 16);
    if (app->themes.active->fonts.font24 == NULL) {
        LOG_ERROR("Failed to load font24  SDL_ttf Error: %s", TTF_GetError());
    }

    app->themes.active->fonts.font34 = TTF_OpenFont(app->themes.active->fonts.font, 34);
    if (app->themes.active->fonts.font34 == NULL) {
        LOG_ERROR("Failed to load font34 SDL_ttf Error: %s", TTF_GetError());
    }

    app->themes.active->fonts.font26 = TTF_OpenFont(app->themes.active->fonts.font, 26);
    if (app->themes.active->fonts.font26 == NULL) {
        LOG_ERROR("Failed to load font26 SDL_ttf Error: %s", TTF_GetError());
    }

    app->themes.active->fonts.font16 = TTF_OpenFont(app->themes.active->fonts.font, 12);
    if (app->themes.active->fonts.font16 == NULL) {
        LOG_ERROR("Failed to load font16 SDL_ttf Error: %s", TTF_GetError());
    }
}

void themes_activate(app_t *app, theme_t *theme) {
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