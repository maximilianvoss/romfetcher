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

#include "uisearchresult.h"
#include <csafestring.h>
#include "rendering.h"
#include "../themes/rendering.h"

void uisearchresult_render(app_t *app, int offset) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    searchresult_t *element = app->search.cursor;

    if (element == NULL) {
        element = app->search.all;
    }

    if (element == NULL) {
        return;
    }

    int deviceCountToDisplay = (height - offset - 80) / 35 + 1;

    int i = 0;
    while (i < deviceCountToDisplay / 2 - 1) {
        if (element->prev == NULL) {
            break;
        }
        element = element->prev;
        i++;
    }

    csafestring_t *itemName = safe_create(NULL);
    int position = offset;
    while (element != NULL && position <= height - 80) {
        safe_strcpy(itemName, "[");
        safe_strcat(itemName, ((engine_t *) element->engine)->name);
        safe_strcat(itemName, "] ");
        safe_strcat(itemName, element->title);

        rendering_loadText(app, &texture, itemName->data, app->fonts.medium, &app->themes.active->colors.text);

        SDL_Rect r2 = {48, position - 2, width - 96, 40};
        themes_setDrawColorBackground(app, (element == app->search.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &r2);

        SDL_Rect r = {50, position, width - 100, 38};
        themes_setDrawColorField(app);
        SDL_RenderFillRect(app->sdlRenderer, &r);

        SDL_Rect srcQuad = {0, 0, width - 120, texture.h};
        SDL_Rect renderQuad = {60, position + 3, (texture.w > width - 120) ? width - 120 : texture.w, texture.h};

        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        SDL_DestroyTexture(texture.texture);

        element = element->next;
        position += 35;
    }
    safe_destroy(itemName);
}
