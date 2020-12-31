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
#include "../search/uisearch.h"
#include "uikeyboard.h"
#include "../download/uidownload.h"
#include "rendering.h"
#include "../constants.h"
#include "uilist.h"
#include "../themes/rendering.h"
#include "uimodal.h"
#include "../download/uidownloadmanager.h"

static void renderDefaults(app_t *app);

static char *copyright();

static char *copyright() {
    char *text = calloc(strlen(COPYRIGHT) + 1, sizeof(char));
    strcpy (text, COPYRIGHT);
    *text = 0xA9;
    return text;
}

static void renderDefaults(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    themes_setDrawColor(app, background);
    SDL_RenderClear(app->sdlRenderer);
    SDL_RenderCopy(app->sdlRenderer, app->themes.active->images.background, NULL, NULL);

    char *text = copyright();
    texture_t texture;
    rendering_loadText(app, &texture, text, app->themes.active->fonts.font16, &app->themes.active->colors.textInverted);
    SDL_Rect renderQuad = {50, height - 30, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);
    free(text);

    SDL_DestroyTexture(texture.texture);
}

void uihandler_render(app_t *app) {
    void (*renderEngine)(app_t *app) = NULL;
    renderDefaults(app);

    switch (app->win) {
        case window_search:
            if (app->systems.active == NULL) {
                app->systems.active = linkedlist_getFirstActive(app->systems.all);
            }
            renderEngine = &uisearch_render;
            break;
        case window_keyboard:
            renderEngine = &uikeyboard_render;
            break;
        case window_download:
            renderEngine = &uidownload_render;
            break;
        case window_downloadMgr:
            renderEngine = &uidownloadmanager_render;
            break;
        case window_config:
        case window_config_advanced:
        case window_config_hoster:
        case window_config_resolution:
        case window_config_systems:
        case window_config_themes:
        case window_system:
            renderEngine = &uilist_renderDefault;
            break;
    }

    if (renderEngine != NULL) {
        renderEngine(app);
    }
    uimodel_render(app);
    SDL_RenderPresent(app->sdlRenderer);
}