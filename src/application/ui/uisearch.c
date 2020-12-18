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

#include "uisearch.h"
#include "rendering.h"
#include "../themes/rendering.h"
#include "uisearchresult.h"
#include "../helper/uihelper.h"
#include "../constants.h"

static void renderSearchField(app_t *app);

static void renderSearchButton(app_t *app);

static void renderSystemSelector(app_t *app);


static void renderSettingsIcon(app_t *app);

static void renderDownloadManagerIcon(app_t *app);

void uisearch_render(app_t *app) {
    renderSettingsIcon(app);
    renderDownloadManagerIcon(app);
    renderSystemSelector(app);
    renderSearchField(app);
    renderSearchButton(app);
    uisearchresult_render(app, 2 * (LIST_ITEM_HEIGHT + 10) + 50);
}

static void renderSettingsIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - 35, 10, 25, 25);
    if (app->search.position == searchactivity_config) {
        themes_setDrawColor(app, fieldActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
    }
    uihelper_renderSDLTexture(app->sdlRenderer, app->themes.active->images.settingsIcon, &element.inner);

}

static void renderDownloadManagerIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - 85, 10, 25, 25);
    if (app->search.position == searchactivity_downloadMgr) {
        themes_setDrawColor(app, fieldActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
    }
    uihelper_renderSDLTexture(app->sdlRenderer, app->themes.active->images.downloadManagerIcon, &element.inner);

    int downloadCount =
            linkedlist_getElementCount(app->download.active) + linkedlist_getElementCount(app->download.queue);
    if (downloadCount > 0) {
        themes_setDrawColor(app, fieldActive);
        rendering_circle(app, width - 65, 32, 10);

        char buffer[4];
        sprintf(buffer, "%d", downloadCount);

        texture_t texture;
        rendering_loadText(app, &texture, buffer, app->themes.active->fonts.font16,
                           &app->themes.active->colors.textInverted);
        SDL_Rect srcQuad = {0, 0, width - 100 - 60, texture.h};
        SDL_Rect renderQuad = {width - 65 - texture.w / 2, 32 - texture.h / 2, texture.w, texture.h};
        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}

static void renderSystemSelector(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRectsFullScreenWidth(20, 50, width, LIST_ITEM_HEIGHT);
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_system));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    SDL_Rect texture_rect = {width - element.outter.x - element.outter.h, element.outter.y, element.outter.h,
                             element.outter.h};
    SDL_RenderCopy(app->sdlRenderer, app->themes.active->images.searchChevron, NULL, &texture_rect);

    texture_t texture;
    rendering_loadText(app, &texture, app->systems.active->fullname, app->themes.active->fonts.font24,
                       &app->themes.active->colors.text);
    uihelper_renderTexture(app->sdlRenderer, &texture, &element.content);
    SDL_DestroyTexture(texture.texture);
}

static void renderSearchField(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(20, 60 + LIST_ITEM_HEIGHT, width - width / 3 - 5,
                                                      LIST_ITEM_HEIGHT);
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_field));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    if (*(app->search.searchText) != '\0') {
        texture_t texture;
        rendering_loadText(app, &texture, app->search.searchText, app->themes.active->fonts.font24,
                           &app->themes.active->colors.text);
        uihelper_renderTexture(app->sdlRenderer, &texture, &element.content);
        SDL_DestroyTexture(texture.texture);
    }
}

static void renderSearchButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - width / 3 + 30, 60 + LIST_ITEM_HEIGHT, width / 3 - 50,
                                                      LIST_ITEM_HEIGHT);
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_button));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    texture_t texture;
    rendering_loadText(app, &texture, "Search", app->themes.active->fonts.font24, &app->themes.active->colors.text);
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &element.content);
    SDL_DestroyTexture(texture.texture);
}

