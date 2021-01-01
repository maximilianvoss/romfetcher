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
#include "../search/search.h"
#include "../keyboard/keyboard.h"
#include "../download/uidownload.h"
#include "rendering.h"
#include "../constants.h"
#include "../list/list.h"
#include "../themes/rendering.h"
#include "../modal/modal.h"
#include "../download/uidownloadmanager.h"
#include "../helper/uihelper.h"

static void renderDefaults(app_t *app);

static void renderSettingsIcon(app_t *app);

static void renderDownloadManagerIcon(app_t *app);

static void renderCopyright(app_t *app);

static void renderDefaults(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    themes_setDrawColor(app, background);
    SDL_RenderClear(app->sdlRenderer);
    SDL_RenderCopy(app->sdlRenderer, app->themes.active->images.background, NULL, NULL);

    renderSettingsIcon(app);
    renderDownloadManagerIcon(app);
    renderCopyright(app);
}

static void renderSettingsIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - 35, 10, 25, 25);
    if (app->search.position == searchactivity_config) {
        themes_setDrawColor(app, fieldActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
    }
    uihelper_renderSDLTexture(app->sdlRenderer, app->themes.active->images.settingsIcon, &element.inner);

}

static void renderDownloadManagerIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - 85, 10, 25, 25);
    if (app->search.position == searchactivity_downloadMgr) {
        themes_setDrawColor(app, fieldActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
    }
    uihelper_renderSDLTexture(app->sdlRenderer, app->themes.active->images.downloadManagerIcon, &element.inner);

    int downloadCount =
            linkedlist_getElementCount(app->download.active) + linkedlist_getElementCount(app->download.queue);
    if (downloadCount > 0) {
        themes_setDrawColor(app, fieldActive);
        rendering_circle(app->sdlRenderer, width - 65, 32, 10);

        char buffer[4];
        sprintf(buffer, "%d", downloadCount);

        texture_t texture;
        rendering_loadText(app->sdlRenderer, &texture, buffer, app->themes.active->fonts.font16,
                           &app->themes.active->colors.textInverted);
        SDL_Rect srcQuad = {0, 0, width - 100 - 60, texture.h};
        SDL_Rect renderQuad = {width - 65 - texture.w / 2, 32 - texture.h / 2, texture.w, texture.h};
        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        uihelper_destroyTexture(&texture);
    }
}

static void renderCopyright(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    char *text = calloc(strlen(COPYRIGHT) + 1, sizeof(char));
    strcpy (text, COPYRIGHT);
    *text = 0xA9;

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, text, app->themes.active->fonts.font16,
                       &app->themes.active->colors.textInverted);
    SDL_Rect renderQuad = {50, height - 30, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);
    free(text);

    uihelper_destroyTexture(&texture);
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
            renderEngine = &keyboard_render;
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
            renderEngine = &ll_renderDefault;
            break;
    }

    if (renderEngine != NULL) {
        renderEngine(app);
    }
    model_render(app);
    SDL_RenderPresent(app->sdlRenderer);
}