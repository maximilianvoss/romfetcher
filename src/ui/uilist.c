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


#include "uilist.h"
#include "rendering.h"
#include "../themes/rendering.h"


static void renderWithCheckbox(app_t *app, int position, linkedlist_t *element, texture_t *texture);

static void renderWithOutCheckbox(app_t *app, int position, linkedlist_t *element, texture_t *texture);

void uilist_render(app_t *app) {
    uilist_renderList(app, 50);
}

void uilist_renderList(app_t *app, int offsetX) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;

    int deviceCountToDisplay = (height - offsetX - 80) / 35 + 1;

    linkedlist_t *element = (app->list.cursor != NULL) ? app->list.cursor : app->list.all;
    if (element == NULL) {
        return;
    }

    for (int i = 0; i < deviceCountToDisplay / 2 - 1 && element->prev != NULL; i++) {
        element = element->prev;
    }

    for (int position = offsetX;
         position <= height - 80 && element != NULL; position += 35, element = element->next) {

        rendering_loadText(app, &texture, element->name, app->fonts.medium, &app->themes.enabled->colors.text);

        SDL_Rect r2 = {48, position - 2, width - 96, 40};
        themes_setDrawColorBackground(app, (element == app->list.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &r2);

        SDL_Rect r = {50, position, width - 100, 38};
        themes_setDrawColorField(app);
        SDL_RenderFillRect(app->sdlRenderer, &r);

        if (app->list.checkbox) {
            renderWithCheckbox(app, position, element, &texture);
        } else {
            renderWithOutCheckbox(app, position, element, &texture);
        }
    }
}


static void renderWithCheckbox(app_t *app, int position, linkedlist_t *element, texture_t *texture) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    SDL_Rect srcQuad = {0, 0, width - 160, texture->h};
    SDL_Rect renderQuad = {100, position + 3, (texture->w > width - 160) ? width - 160 : texture->w, texture->h};

    SDL_RenderCopy(app->sdlRenderer, texture->texture, &srcQuad, &renderQuad);
    SDL_DestroyTexture(texture->texture);

    SDL_Rect texture_rect = {60, position + 5, 25, 25};

    uint8_t active;
    if (app->list.multi) {
        active = element->active;
    } else {
        active = (app->list.active == element);
    }
    SDL_RenderCopy(app->sdlRenderer,
                   (active == 1) ? app->textures.checkboxChecked : app->textures.checkboxUnchecked, NULL,
                   &texture_rect);
}

static void renderWithOutCheckbox(app_t *app, int position, linkedlist_t *element, texture_t *texture) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    SDL_Rect srcQuad = {0, 0, width - 120, texture->h};
    SDL_Rect renderQuad = {60, position + 3, (texture->w > width - 120) ? width - 120 : texture->w, texture->h};

    SDL_RenderCopy(app->sdlRenderer, texture->texture, &srcQuad, &renderQuad);
    SDL_DestroyTexture(texture->texture);
}