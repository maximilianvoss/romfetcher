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

#include "modal.h"
#include "../themes/rendering.h"
#include "../ui/rendering.h"
#include "../helper/uihelper.h"

static void darkenBackground(app_t *app);

static void renderFrame(app_t *app);

static void renderHeadline(app_t *app);

static void renderText(app_t *app);

static void renderActionButton(app_t *app);

static void renderCancelButton(app_t *app);

void modal_processUp(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void modal_processDown(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void modal_processLeft(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void modal_processRight(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void modal_processSelect(app_t *app) {
    if (!app->modal.cursorPos) {
        if (app->modal.callbackAction != NULL) {
            app->modal.callbackAction(app, app->modal.callbackData);
        } else {
            app->modal.displayed = 0;
        }
    }
    if (app->modal.cursorPos) {
        if (app->modal.callbackCancel != NULL) {
            app->modal.callbackCancel(app, app->modal.callbackData);
        } else {
            app->modal.displayed = 0;
        }
    }
    app->modal.cursorPos = 0;
}

void modal_processBack(app_t *app) {
    if (app->modal.callbackCancel != NULL) {
        app->modal.callbackCancel(app, app->modal.callbackData);
    }
    app->modal.cursorPos = 0;
}

void modal_processOtherButton(app_t *app, controller_t *state) {}

void modal_processOtherKey(app_t *app, SDL_Scancode scancode) {}


void model_render(app_t *app) {
    if (!app->modal.displayed) {
        return;
    }
    darkenBackground(app);
    renderFrame(app);
    renderHeadline(app);
    renderText(app);
    renderActionButton(app);
    renderCancelButton(app);
}

static void darkenBackground(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    themes_setDrawColor(app, windowBackgroundInactive);
    SDL_Rect backgroundRect = {0, 0, width, height};
    SDL_RenderFillRect(app->sdlRenderer, &backgroundRect);
}

static void renderFrame(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t modalWindowRect = uihelper_generateRectsFullScreenWidth(75, 75, width, height - 150);

    themes_setDrawColor(app, modalBackground);
    SDL_RenderFillRect(app->sdlRenderer, &modalWindowRect.outter);

    themes_setDrawColor(app, modalForeground);
    SDL_RenderFillRect(app->sdlRenderer, &modalWindowRect.inner);
}

static void renderHeadline(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, app->modal.headline, app->themes.active->fonts.font34,
                       &app->themes.active->colors.modalHeadText);

    uiElementRects_t modalHeaderRect = uihelper_generateRectsFullScreenWidth(75, 75, width, texture.h);

    themes_setDrawColor(app, modalBackground);
    SDL_RenderFillRect(app->sdlRenderer, &modalHeaderRect.outter);

    themes_setDrawColor(app, modalHeadForeground);
    SDL_RenderFillRect(app->sdlRenderer, &modalHeaderRect.inner);

    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &modalHeaderRect.content);
    uihelper_destroyTexture(&texture);
}

static void renderText(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, app->modal.text, app->themes.active->fonts.font26,
                       &app->themes.active->colors.modalText);

    uiElementRects_t modalTextRect = uihelper_generateRectsFullScreenWidth(75, 140, width, texture.h);
    uihelper_renderTexture(app->sdlRenderer, &texture, &modalTextRect.content);
    uihelper_destroyTexture(&texture);
}

#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 50

static void renderActionButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t modalActionBtnRect = uihelper_generateRects(width - BUTTON_WIDTH - 85,
                                                                 height - 150 + 75 - BUTTON_HEIGHT - 15,
                                                                 BUTTON_WIDTH, BUTTON_HEIGHT);

    themes_setDrawColor(app, app->modal.cursorPos ? modalButtonBackgroundInactive : modalButtonBackgroundActive);
    SDL_RenderFillRect(app->sdlRenderer, &modalActionBtnRect.outter);

    themes_setDrawColor(app, app->modal.cursorPos ? modalButtonForegroundInactive : modalButtonForegroundActive);
    SDL_RenderFillRect(app->sdlRenderer, &modalActionBtnRect.inner);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, app->modal.actionButton, app->themes.active->fonts.font26,
                       app->modal.cursorPos ? &app->themes.active->colors.modalButtonTextInactive
                                            : &app->themes.active->colors.modalButtonTextActive);

    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &modalActionBtnRect.content);
    uihelper_destroyTexture(&texture);
}

static void renderCancelButton(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        int width, height;
        SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

        uiElementRects_t modalActionBtnRect = uihelper_generateRects(85, height - 150 + 75 - BUTTON_HEIGHT - 15,
                                                                     BUTTON_WIDTH, BUTTON_HEIGHT);

        themes_setDrawColor(app, !app->modal.cursorPos ? modalButtonBackgroundInactive : modalButtonBackgroundActive);
        SDL_RenderFillRect(app->sdlRenderer, &modalActionBtnRect.outter);

        themes_setDrawColor(app, !app->modal.cursorPos ? modalButtonForegroundInactive : modalButtonForegroundActive);
        SDL_RenderFillRect(app->sdlRenderer, &modalActionBtnRect.inner);

        texture_t texture;
        rendering_loadText(app->sdlRenderer, &texture, app->modal.cancelButton, app->themes.active->fonts.font26,
                           !app->modal.cursorPos ? &app->themes.active->colors.modalButtonTextInactive
                                                 : &app->themes.active->colors.modalButtonTextActive);

        uihelper_renderTextureCentered(app->sdlRenderer, &texture, &modalActionBtnRect.content);
        uihelper_destroyTexture(&texture);
    }
}