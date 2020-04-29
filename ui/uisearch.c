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
#include "../config.h"
#include "rendering.h"

static void renderSearchField(app_t *app);

static void renderSearchButton(app_t *app);

static void renderDeviceSelector(app_t *app);

static void renderSearchResults(app_t *app);

void uisearch_render(app_t *app) {
    renderDeviceSelector(app);
    renderSearchField(app);
    renderSearchButton(app);
    renderSearchResults(app);
}

static void renderDeviceSelector(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_Rect r2 = {48, 50, width - 96, 54};
    SDL_SetRenderDrawColor(app->renderer, 0, 0, (app->search.position == searchsystem) ? 255 : 0, 150);
    SDL_RenderFillRect(app->renderer, &r2);

    SDL_Rect r = {50, 52, width - 100, 50};
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
    SDL_RenderFillRect(app->renderer, &r);

    SDL_Rect texture_rect = {width - 100, 51, 52, 52};
    SDL_RenderCopy(app->renderer, app->textures.searchChevron, NULL, &texture_rect);

    SDL_Color textColor = TEXT_COLOR;
    texture_t texture;
    rendering_loadText(app, &texture, app->search.systemActive->fullname, app->fonts.big, &textColor);
    SDL_Rect renderQuad = {60, 55, texture.w, texture.h};
    SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

static void renderSearchField(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_Rect r2 = {48, 118, width - 266, 54};
    SDL_SetRenderDrawColor(app->renderer, 0, 0, (app->search.position == searchfield) ? 255 : 0, 150);
    SDL_RenderFillRect(app->renderer, &r2);

    SDL_Rect r = {50, 120, width - 270, 50};
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
    SDL_RenderFillRect(app->renderer, &r);

    // TODO: Handle overflow
    if (*(app->search.searchText) != '\0') {
        SDL_Color textColor = TEXT_COLOR;
        texture_t texture;
        rendering_loadText(app, &texture, app->search.searchText, app->fonts.big, &textColor);
        SDL_Rect renderQuad = {60, 125, texture.w, texture.h};
        SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}

static void renderSearchButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_Rect r2 = {width - 202, 118, 154, 54};
    SDL_SetRenderDrawColor(app->renderer, 0, 0, (app->search.position == searchbutton) ? 255 : 0, 150);
    SDL_RenderFillRect(app->renderer, &r2);

    SDL_Rect r = {width - 200, 120, 150, 50};
    SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
    SDL_RenderFillRect(app->renderer, &r);

    SDL_Color textColor = TEXT_COLOR;
    texture_t texture;
    rendering_loadText(app, &texture, "Search", app->fonts.big, &textColor);
    SDL_Rect renderQuad = {width - 180, 125, texture.w, texture.h};
    SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture.texture);
}

static void renderSearchResults(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_Color textColor = TEXT_COLOR;
    texture_t texture;

    int deviceCountToDisplay = (height - 190 - 50) / 35 + 1;
    searchresult_t *result = app->search.resultHovered;
    if (app->search.resultHovered == NULL) {
        return;
    }

    for (int i = 0; i < deviceCountToDisplay / 2 - 1 && result->prev != NULL; i++) {
        result = result->prev;
    }

    for (int position = 190;
         position <= height - 80 && result != NULL; position += 35, result = result->next) {
        rendering_loadText(app, &texture, result->title, app->fonts.medium, &textColor);

        SDL_Rect r2 = {48, position - 2, width - 96, 40};
        SDL_SetRenderDrawColor(app->renderer, 0, 0,
                               (result == app->search.resultHovered && app->search.position == searchresults) ? 255 : 0,
                               150);
        SDL_RenderFillRect(app->renderer, &r2);

        SDL_Rect r = {50, position, width - 100, 38};
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
        SDL_RenderFillRect(app->renderer, &r);

        SDL_Rect renderQuad = {60, position + 3, texture.w, texture.h};
        SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}

