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

#include "uidownload.h"
#include "rendering.h"
#include "../themes/rendering.h"

static void renderTitle(app_t *app);

static void renderProgressBar(app_t *app);

static void renderStartButton(app_t *app);

static void renderCancelButton(app_t *app);

static void renderDoneButton(app_t *app);

void uidownload_render(app_t *app) {
    if (app->search.active == NULL) {
        SDL_Log("Active search result is empty");
        return;
    }
    renderTitle(app);
    if (app->download.started || app->download.complete) {
        renderProgressBar(app);
    } else {
        renderStartButton(app);
        renderCancelButton(app);
    }
    if (app->download.complete) {
        renderDoneButton(app);
    }
}

static void renderTitle(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, app->search.active->title, app->fonts.big,
                       &app->themes.active->colors.textInverted);

    int textureWidth = (texture.w > width + 100) ? width - 100 : texture.w;
    SDL_Rect renderQuad = {width / 2 - textureWidth / 2, 55, textureWidth, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    SDL_DestroyTexture(texture.texture);
}

static void renderProgressBar(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    SDL_Rect rect1 = {50, 200, width - 100, 50};
    themes_setDrawColor(app, fieldBackground);
    SDL_RenderFillRect(app->sdlRenderer, &rect1);

    SDL_Rect rect2 = {50 + 2, 200 + 2, width - 100 - 4, 50 - 4};
    themes_setDrawColor(app, field);
    SDL_SetRenderDrawColor(app->sdlRenderer, 255, 255, 255, 150);
    SDL_RenderFillRect(app->sdlRenderer, &rect2);


    char percentText[4];
    double percentage;
    if (app->download.complete) {
        percentage = 1.0;
    } else {
        percentage = app->download.total != 0 ? (double) app->download.current / (double) app->download.total : 0.0;
    }
    snprintf(percentText, 4, "%.0f%c", percentage * 100, '%');

    SDL_Rect rect3 = {50 + 2, 200 + 2, (width - 100 - 4) * percentage, 50 - 4};
    themes_setDrawColor(app, fieldActive);
    SDL_RenderFillRect(app->sdlRenderer, &rect3);

    texture_t texture;
    rendering_loadText(app, &texture, percentText, app->fonts.big, &app->themes.active->colors.text);
    SDL_Rect renderQuad = {width / 2 - texture.w / 2, 205, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    SDL_DestroyTexture(texture.texture);
}

static void renderStartButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, "Download", app->fonts.big, &app->themes.active->colors.text);

    SDL_Rect rect1 = {width - 50 - texture.w - 100, 300, texture.w + 100, 70};
    themes_setDrawColorBackground(app, (app->download.cursorPos == downloadActivity_start));
    SDL_RenderFillRect(app->sdlRenderer, &rect1);

    SDL_Rect rect2 = {width - 50 + 2 - texture.w - 100, 300 + 2, texture.w + 100 - 4, 70 - 4};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &rect2);

    SDL_Rect renderQuad = {width - texture.w - 100, 315, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    SDL_DestroyTexture(texture.texture);
}

static void renderCancelButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, "Cancel", app->fonts.big, &app->themes.active->colors.text);

    SDL_Rect rect1 = {50, 300, texture.w + 100, 70};
    themes_setDrawColorBackground(app, (app->download.cursorPos == downloadActivity_cancel));
    SDL_RenderFillRect(app->sdlRenderer, &rect1);

    SDL_Rect rect2 = {50 + 2, 300 + 2, texture.w + 100 - 4, 70 - 4};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &rect2);

    SDL_Rect renderQuad = {100, 315, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    SDL_DestroyTexture(texture.texture);
}

static void renderDoneButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, "Done", app->fonts.big, &app->themes.active->colors.text);

    SDL_Rect rect1 = {width / 2 - texture.w / 2 - 50, 300, texture.w + 100, 70};
    themes_setDrawColorBackground(app, (app->download.cursorPos == downloadActivity_done));
    SDL_RenderFillRect(app->sdlRenderer, &rect1);

    SDL_Rect rect2 = {width / 2 - texture.w / 2 - 50 + 2, 300 + 2, texture.w + 100 - 4, 70 - 4};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &rect2);

    SDL_Rect renderQuad = {width / 2 - texture.w / 2, 315, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    SDL_DestroyTexture(texture.texture);
}