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

#include "uimodal.h"
#include "../themes/rendering.h"
#include "rendering.h"

static void darkenBackground(app_t *app);

static void renderBox(app_t *app);

static void renderHeadline(app_t *app);

static void renderText(app_t *app);

static void renderActionButton(app_t *app);

static void renderCancelButton(app_t *app);


void uimodel_render(app_t *app) {
    if (!app->modal.displayed) {
        return;
    }
    darkenBackground(app);
    darkenBackground(app);
    renderBox(app);
    renderHeadline(app);
    renderText(app);
    renderActionButton(app);
    renderCancelButton(app);
}

static void darkenBackground(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    int r, g, b, a;
    r = app->themes.active->colors.background.r;
    g = app->themes.active->colors.background.g;
    b = app->themes.active->colors.background.b;
    a = app->themes.active->colors.background.a;
    if (a > 150) {
        a = 150;
    }
    SDL_SetRenderDrawColor(app->sdlRenderer, r, g, b, a);

    SDL_Rect backgroundRect = {0, 0, width, height};
    SDL_RenderFillRect(app->sdlRenderer, &backgroundRect);
}

static void renderBox(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    themes_setDrawColor(app, fieldActive);
    SDL_Rect shade = {75, 75, width - 150, height - 150};
    SDL_RenderFillRect(app->sdlRenderer, &shade);

    themes_setDrawColor(app, background);
    SDL_Rect box = {75 + 2, 75 + 2, width - 150 - 4, height - 150 - 4};
    SDL_RenderFillRect(app->sdlRenderer, &box);
}

static void renderHeadline(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, app->modal.headline, app->fonts.big, &app->themes.active->colors.textInverted);
    SDL_Rect srcQuad = {0, 0, width - 85 - 85, texture.h};
    SDL_Rect renderQuad = {(texture.w > width - 85 - 85) ? 85 : (width - texture.w) / 2, 85,
                           (texture.w > width - 85 - 85) ? width - 85 - 85 : texture.w, texture.h};

    themes_setDrawColor(app, fieldActive);
    SDL_Rect shade = {75, 75, width - 150, texture.h + 15};
    SDL_RenderFillRect(app->sdlRenderer, &shade);

    themes_setDrawColor(app, background);
    SDL_Rect box = {75 + 2, 75 + 2, width - 150 - 4, texture.h + 15 - 4};
    SDL_RenderFillRect(app->sdlRenderer, &box);

    SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

static void renderText(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, app->modal.text, app->fonts.medium, &app->themes.active->colors.textInverted);
    SDL_Rect srcQuad = {0, 0, width - 85 - 85, texture.h};
    SDL_Rect renderQuad = {85, 140, (texture.w > width - 85 - 85) ? width - 85 - 85 : texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

static void renderActionButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    themes_setDrawColor(app, app->modal.cursorPos ? fieldActive : background);
    SDL_Rect shade = {width - 85 - 200, height - 150 + 75 - 65, 200, 50};
    SDL_RenderFillRect(app->sdlRenderer, &shade);

    themes_setDrawColor(app, app->modal.cursorPos ? background : fieldActive);
    SDL_Rect box = {width - 85 - 200 + 2, height - 150 + 75 - 65 + 2, 200 - 4, 50 - 4};
    SDL_RenderFillRect(app->sdlRenderer, &box);

    texture_t texture;
    rendering_loadText(app, &texture, app->modal.actionButton, app->fonts.medium,
                       &app->themes.active->colors.textInverted);
    SDL_Rect srcQuad = {0, 0, 180, texture.h};

    SDL_Rect renderQuad = {(texture.w > 180) ? width - 85 - 200 : width - 85 - 200 + (200 - texture.w) / 2,
                           height - 150 + 75 - 55, (texture.w > 180) ? 180 : texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

static void renderCancelButton(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        int width, height;
        SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

        themes_setDrawColor(app, app->modal.cursorPos ? background : fieldActive);
        SDL_Rect shade = {85, height - 150 + 75 - 65, 200, 50};
        SDL_RenderFillRect(app->sdlRenderer, &shade);

        themes_setDrawColor(app, app->modal.cursorPos ? fieldActive : background);
        SDL_Rect box = {85 + 2, height - 150 + 75 - 65 + 2, 200 - 4, 50 - 4};
        SDL_RenderFillRect(app->sdlRenderer, &box);

        texture_t texture;
        rendering_loadText(app, &texture, app->modal.cancelButton, app->fonts.medium,
                           &app->themes.active->colors.textInverted);
        SDL_Rect srcQuad = {0, 0, 180, texture.h};

        SDL_Rect renderQuad = {(texture.w > 180) ? 95 : 85 + (200 - texture.w) / 2, height - 150 + 75 - 55,
                               (texture.w > 180) ? 180 : texture.w, texture.h};
        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}