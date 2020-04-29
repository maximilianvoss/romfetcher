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

#include "uihandler.h"
#include "uisearch.h"
#include "uisystem.h"
#include "uikeyboard.h"
#include "uidownload.h"
#include "rendering.h"
#include "../config.h"

static void renderDefaults(app_t *app);

static char *copyright();

static char *copyright() {
    char *text = calloc(sizeof(char), strlen(COPYRIGHT) + 1);
    strcpy (text, COPYRIGHT);
    *text = 0xA9;
    return text;
}

static void renderDefaults(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->window, &width, &height);

    SDL_RenderCopy(app->renderer, app->textures.backgroundImage, NULL, NULL);

    char *text = copyright();
    SDL_Color textColor = TEXT_COLOR_INVERT;
    texture_t texture;
    rendering_loadText(app, &texture, text, app->fonts.small, &textColor);
    SDL_Rect renderQuad = {50, height - 40, texture.w, texture.h};
    SDL_RenderCopy(app->renderer, texture.texture, NULL, &renderQuad);
    free(text);

    SDL_DestroyTexture(texture.texture);

}

void uihandler_render(app_t *app) {
    void (*renderEngine)(app_t *app) = NULL;
    renderDefaults(app);

    switch (app->win) {
        case search:
            renderEngine = &uisearch_render;
            break;
        case systemselect:
            renderEngine = &uisystem_render;
            break;
        case keyboard:
            renderEngine = &uikeyboard_render;
            break;
        case download:
            renderEngine = &uidownload_render;
            break;
    }

    if (renderEngine != NULL) {
        renderEngine(app);
    }

    SDL_RenderPresent(app->renderer);
}