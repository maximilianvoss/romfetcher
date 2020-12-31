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
#include "../ui/rendering.h"
#include "../themes/rendering.h"
#include "../helper/uihelper.h"

static void renderTitle(app_t *app);

static void renderStartButton(app_t *app);

static void renderCancelButton(app_t *app);

void uidownload_render(app_t *app) {
    if (app->search.active == NULL) {
        LOG_DEBUG("Active search result is empty");
        return;
    }
    renderTitle(app);
    renderStartButton(app);
    renderCancelButton(app);
}

static void renderTitle(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, app->search.active->title, app->themes.active->fonts.font34,
                       &app->themes.active->colors.textInverted);

    int textureWidth = (texture.w > width + 100) ? width - 100 : texture.w;
    SDL_Rect renderQuad = {width / 2 - textureWidth / 2, 55, textureWidth, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    uihelper_destroyTexture(&texture);
}

static void renderStartButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, "Download", app->themes.active->fonts.font34, &app->themes.active->colors.text);

    SDL_Rect rect1 = {width - 50 - texture.w - 100, 300, texture.w + 100, 70};
    themes_setDrawColorBackground(app, (app->download.cursorPos == downloadActivity_start));
    SDL_RenderFillRect(app->sdlRenderer, &rect1);

    SDL_Rect rect2 = {width - 50 + 2 - texture.w - 100, 300 + 2, texture.w + 100 - 4, 70 - 4};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &rect2);

    SDL_Rect renderQuad = {width - texture.w - 100, 315, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    uihelper_destroyTexture(&texture);
}

static void renderCancelButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    rendering_loadText(app, &texture, "Cancel", app->themes.active->fonts.font34, &app->themes.active->colors.text);

    SDL_Rect rect1 = {50, 300, texture.w + 100, 70};
    themes_setDrawColorBackground(app, (app->download.cursorPos == downloadActivity_cancel));
    SDL_RenderFillRect(app->sdlRenderer, &rect1);

    SDL_Rect rect2 = {50 + 2, 300 + 2, texture.w + 100 - 4, 70 - 4};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &rect2);

    SDL_Rect renderQuad = {100, 315, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);

    uihelper_destroyTexture(&texture);
}
