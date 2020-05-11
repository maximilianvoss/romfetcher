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
#include "uiconfig.h"
#include "uiconfigengine.h"
#include "uiconfigsystem.h"

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

    SDL_RenderCopy(app->sdlRenderer, app->textures.backgroundImage, NULL, NULL);

    char *text = copyright();
    texture_t texture;
    rendering_loadText(app, &texture, text, app->fonts.small, &app->themes.enabled->colors.textInverted);
    SDL_Rect renderQuad = {50, height - 40, texture.w, texture.h};
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
                app->systems.active = app->systems.enabled;
            }
            renderEngine = &uisearch_render;
            break;
        case window_system:
            if (app->systems.cursor == NULL) {
                app->systems.cursor = app->systems.active;
            }
            renderEngine = &uisystem_render;
            break;
        case window_keyboard:
            renderEngine = &uikeyboard_render;
            break;
        case window_download:
            renderEngine = &uidownload_render;
            break;
        case window_config:
            renderEngine = &uiconfig_render;
            break;
        case window_config_engine:
            if (app->engine.cursor == engine_notdefined) {
                app->engine.cursor = app->engine.active;
            }
            renderEngine = &uiconfigengine_render;
            break;
        case window_config_systems:
            if (app->systems.active == NULL) {
                app->systems.active = app->systems.enabled;
                app->systems.cursor = app->systems.enabled;
            }
            renderEngine = &uiconfigsystem_render;
            break;
    }

    if (renderEngine != NULL) {
        renderEngine(app);
    }
    SDL_RenderPresent(app->sdlRenderer);
}