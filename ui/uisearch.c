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

static void renderSearchField(app_t *app);

static void renderSearchButton(app_t *app);

static void renderSystemSelector(app_t *app);

static void renderSearchResults(app_t *app);

static void renderSettingsIcon(app_t *app);

void uisearch_render(app_t *app) {
    renderSettingsIcon(app);
    renderSystemSelector(app);
    renderSearchField(app);
    renderSearchButton(app);
    renderSearchResults(app);
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
                       &app->themes.enabled->colors.text);
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
        rendering_loadText(app, &texture, app->search.searchText, app->fonts.big, &app->themes.enabled->colors.text);

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
    rendering_loadText(app, &texture, "Search", app->fonts.big, &app->themes.enabled->colors.text);
    SDL_Rect renderQuad = {width - 180, 125, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

static void renderSearchResults(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;

    int deviceCountToDisplay = (height - 190 - 50) / 35 + 1;
    searchresult_t *result = app->search.cursor;
    if (app->search.cursor == NULL) {
        return;
    }

    for (int i = 0; i < deviceCountToDisplay / 2 - 1 && result->prev != NULL; i++) {
        result = result->prev;
    }

    for (int position = 190;
         position <= height - 80 && result != NULL; position += 35, result = result->next) {

        //TODO: handle overflow
        rendering_loadText(app, &texture, result->title, app->fonts.medium, &app->themes.enabled->colors.text);

        SDL_Rect r2 = {48, position - 2, width - 96, 40};
        themes_setDrawColorBackground(app, (result == app->search.cursor &&
                                            app->search.position == searchactivity_results));
        SDL_RenderFillRect(app->sdlRenderer, &r2);

        SDL_Rect r = {50, position, width - 100, 38};
        themes_setDrawColorField(app);
        SDL_RenderFillRect(app->sdlRenderer, &r);

        SDL_Rect renderQuad = {60, position + 3, texture.w, texture.h};
        SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}

