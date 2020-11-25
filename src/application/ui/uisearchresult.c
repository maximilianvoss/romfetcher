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
    result_t *element = app->search.cursor;

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

    int position = offset;
    while (element != NULL && position <= height - 80) {
        rendering_loadText(app, &texture, element->title, app->fonts.medium, &app->themes.active->colors.text);

        SDL_Rect r2 = {48, position - 2, width - 96, 40};
        themes_setDrawColorBackground(app, (element == app->search.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &r2);

        SDL_Rect r = {50, position, width - 100, 38};
        themes_setDrawColorField(app);
        SDL_RenderFillRect(app->sdlRenderer, &r);

        SDL_Rect srcQuad = {0, 0, width - 120, texture.h};
        SDL_Rect renderQuad = {100, position + 3, (texture.w > width - 160) ? width - 160 : texture.w, texture.h};

        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        SDL_DestroyTexture(texture.texture);

        // TODO: Keep Texture
        if (element->hoster->favicon != NULL) {
            if (element->hoster->favicon->size > 0 && element->hoster->favicon->binary != NULL) {
                SDL_Texture *icon = rendering_memImage(app, element->hoster->favicon->binary,
                                                       element->hoster->favicon->size);
                if (icon != NULL) {
                    SDL_Rect destIconQuad = {60, position + 3, 30, 30};
                    SDL_RenderCopy(app->sdlRenderer, icon, NULL, &destIconQuad);
                    SDL_DestroyTexture(icon);
                }
            }
        }

        element = element->next;
        position += 35;
    }
}
