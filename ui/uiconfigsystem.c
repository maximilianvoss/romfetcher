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


#include "uiconfigsystem.h"
#include "../config.h"
#include "rendering.h"

void uiconfigsystem_render(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_Color textColor = TEXT_COLOR;
    texture_t texture;

    int deviceCountToDisplay = (height - 80 - 50) / 35 + 1;

    system_t *systems = app->config.systemCursor;
    if (systems == NULL) {
        return;
    }

    for (int i = 0; i < deviceCountToDisplay / 2 - 1 && systems->prev != NULL; i++) {
        systems = systems->prev;
    }

    for (int position = 50;
         position <= height - 80 && systems != NULL; position += 35, systems = systems->next) {
        rendering_loadText(app, &texture, systems->fullname, app->fonts.medium, &textColor);

        SDL_Rect r2 = {48, position - 2, width - 96, 40};
        SDL_SetRenderDrawColor(app->renderer, 0, 0, (systems == app->config.systemCursor) ? 255 : 0, 150);
        SDL_RenderFillRect(app->renderer, &r2);

        SDL_Rect r = {50, position, width - 100, 38};
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
        SDL_RenderFillRect(app->renderer, &r);

        SDL_Rect texture_rect = {60, position + 5, 25, 25};
        SDL_RenderCopy(app->renderer,
                       systems->active == 1 ? app->textures.checkboxChecked : app->textures.checkboxUnchecked, NULL,
                       &texture_rect);

        SDL_Rect renderQuad = {100, position + 3, texture.w, texture.h};
        SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}