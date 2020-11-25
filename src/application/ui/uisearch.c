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
    uisearchresult_render(app, 190);
}

static void renderSettingsIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    if (app->search.position == searchactivity_config) {
        SDL_Rect r2 = {width - 37, 8, 27, 27};
        themes_setDrawColor(app, fieldActive);
        SDL_RenderFillRect(app->sdlRenderer, &r2);
    }

    SDL_Rect texture_rect = {width - 35, 10, 25, 25};
    SDL_RenderCopy(app->sdlRenderer, app->textures.settingsIcon, NULL, &texture_rect);
}

static void renderDownloadManagerIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    if (app->search.position == searchactivity_downloadMgr) {
        SDL_Rect r2 = {width - 87, 8, 27, 27};
        themes_setDrawColor(app, fieldActive);
        SDL_RenderFillRect(app->sdlRenderer, &r2);
    }

    SDL_Rect texture_rect = {width - 85, 10, 25, 25};
    SDL_RenderCopy(app->sdlRenderer, app->textures.downloadManagerIcon, NULL, &texture_rect);

    int downloadCount =
            linkedlist_getElementCount(app->download.active) + linkedlist_getElementCount(app->download.queue);
    if (downloadCount > 0) {
        themes_setDrawColor(app, fieldActive);
        rendering_circle(app, width - 65, 32, 10);

        char buffer[4];
        sprintf(buffer, "%d", downloadCount);

        texture_t texture;
        rendering_loadText(app, &texture, buffer, app->fonts.small, &app->themes.active->colors.textInverted);
        SDL_Rect srcQuad = {0, 0, width - 100 - 60, texture.h};
        SDL_Rect renderQuad = {width - 65 - texture.w / 2, 32 - texture.h / 2, texture.w, texture.h};
        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}

static void renderSystemSelector(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    SDL_Rect r2 = {48, 50, width - 96, 54};
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_system));
    SDL_RenderFillRect(app->sdlRenderer, &r2);

    SDL_Rect r = {50, 52, width - 100, 50};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &r);

    SDL_Rect texture_rect = {width - 100, 51, 52, 52};
    SDL_RenderCopy(app->sdlRenderer, app->textures.searchChevron, NULL, &texture_rect);

    texture_t texture;
    rendering_loadText(app, &texture, app->systems.active->fullname, app->fonts.big,
                       &app->themes.active->colors.text);
    SDL_Rect srcQuad = {0, 0, width - 100 - 60, texture.h};
    SDL_Rect renderQuad = {60, 55, (texture.w > width - 100 - 60) ? width - 100 - 60 : texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

static void renderSearchField(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    SDL_Rect r2 = {48, 118, width - 266, 54};
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_field));
    SDL_RenderFillRect(app->sdlRenderer, &r2);

    SDL_Rect r = {50, 120, width - 270, 50};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &r);

    if (*(app->search.searchText) != '\0') {
        texture_t texture;
        rendering_loadText(app, &texture, app->search.searchText, app->fonts.big, &app->themes.active->colors.text);

        SDL_Rect srcQuad = {0, 0, width - 290, texture.h};
        SDL_Rect renderQuad = {60, 125, (texture.w > width - 290) ? width - 290 : texture.w, texture.h};

        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}

static void renderSearchButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    SDL_Rect r2 = {width - 202, 118, 154, 54};
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_button));
    SDL_RenderFillRect(app->sdlRenderer, &r2);

    SDL_Rect r = {width - 200, 120, 150, 50};
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &r);

    texture_t texture;
    rendering_loadText(app, &texture, "Search", app->fonts.big, &app->themes.active->colors.text);
    SDL_Rect renderQuad = {width - 180, 125, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

