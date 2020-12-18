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

#include <SDL_render.h>
#include "rendering.h"
#include "../ui/rendering.h"

void themes_setDrawColorField(app_t *app) {
    themes_setDrawColor(app, field);
}

void themes_setDrawColorBackground(app_t *app, uint8_t active) {
    if (active) {
        themes_setDrawColor(app, fieldActive);
    } else {
        themes_setDrawColor(app, fieldBackground);
    }
}

void themes_setDrawColor(app_t *app, enum theme_color color) {
    int r, g, b, a;
    switch (color) {
        case background:
            r = app->themes.active->colors.background.r;
            g = app->themes.active->colors.background.g;
            b = app->themes.active->colors.background.b;
            a = app->themes.active->colors.background.a;
            break;
        case field:
            r = app->themes.active->colors.field.r;
            g = app->themes.active->colors.field.g;
            b = app->themes.active->colors.field.b;
            a = app->themes.active->colors.field.a;
            break;
        case fieldBackground:
            r = app->themes.active->colors.fieldBackground.r;
            g = app->themes.active->colors.fieldBackground.g;
            b = app->themes.active->colors.fieldBackground.b;
            a = app->themes.active->colors.fieldBackground.a;
            break;
        case fieldActive:
            r = app->themes.active->colors.fieldHighlight.r;
            g = app->themes.active->colors.fieldHighlight.g;
            b = app->themes.active->colors.fieldHighlight.b;
            a = app->themes.active->colors.fieldHighlight.a;
            break;
        case modalBackground:
            r = app->themes.active->colors.modalBackground.r;
            g = app->themes.active->colors.modalBackground.g;
            b = app->themes.active->colors.modalBackground.b;
            a = app->themes.active->colors.modalBackground.a;
            break;
        case modalBackgroundInactive:
            r = app->themes.active->colors.modalBackgroundInactive.r;
            g = app->themes.active->colors.modalBackgroundInactive.g;
            b = app->themes.active->colors.modalBackgroundInactive.b;
            a = app->themes.active->colors.modalBackgroundInactive.a;
            break;
        case modalFrame:
            r = app->themes.active->colors.modalFrame.r;
            g = app->themes.active->colors.modalFrame.g;
            b = app->themes.active->colors.modalFrame.b;
            a = app->themes.active->colors.modalFrame.a;
            break;
        case modalButtonFrame:
            r = app->themes.active->colors.modalButtonFrame.r;
            g = app->themes.active->colors.modalButtonFrame.g;
            b = app->themes.active->colors.modalButtonFrame.b;
            a = app->themes.active->colors.modalButtonFrame.a;
            break;
        case modalButtonFrameActive:
            r = app->themes.active->colors.modalButtonFrameActive.r;
            g = app->themes.active->colors.modalButtonFrameActive.g;
            b = app->themes.active->colors.modalButtonFrameActive.b;
            a = app->themes.active->colors.modalButtonFrameActive.a;
            break;
        default:
            r = 255;
            g = 255;
            b = 255;
            a = 255;
    }
    SDL_SetRenderDrawColor(app->sdlRenderer, r, g, b, a);
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
    app->themes.active->images.background = rendering_loadImage(app, app->themes.active->images.backgroundPath);
    app->themes.active->images.searchChevron = rendering_loadImage(app, app->themes.active->images.selectorIconPath);
    app->themes.active->images.settingsIcon = rendering_loadImage(app, app->themes.active->images.settingsIconPath);
    app->themes.active->images.checkboxChecked = rendering_loadImage(app,
                                                                     app->themes.active->images.checkboxCheckedPath);
    app->themes.active->images.checkboxUnchecked = rendering_loadImage(app,
                                                                       app->themes.active->images.checkboxUncheckedPath);
    app->themes.active->images.downloadManagerIcon = rendering_loadImage(app,
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