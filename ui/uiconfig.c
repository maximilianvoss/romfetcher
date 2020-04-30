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

#include "uiconfig.h"
#include "rendering.h"
#include "../config.h"

void uiconfig_render(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_Color textColor = TEXT_COLOR;
    texture_t texture;

    char menuEntries[2][23] = {
            "Select Search Engine", "Enable/Disable Systems"
    };

    for (int i = 0, position = 80; i < 2; i++, position += 35) {
        rendering_loadText(app, &texture, menuEntries[i], app->fonts.medium, &textColor);
        SDL_Rect r2 = {48, position - 2, width - 96, 40};
        SDL_SetRenderDrawColor(app->renderer, 0, 0, (app->config.configCursor == i) ? 255 : 0, 150);
        SDL_RenderFillRect(app->renderer, &r2);

        SDL_Rect r = {50, position, width - 100, 38};
        SDL_SetRenderDrawColor(app->renderer, 255, 255, 255, 150);
        SDL_RenderFillRect(app->renderer, &r);

        SDL_Rect renderQuad = {60, position + 3, texture.w, texture.h};
        SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
        SDL_DestroyTexture(texture.texture);
    }
}
