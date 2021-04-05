/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
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

#include <csafestring.h>
#include "uihandler.h"
#include "rendering.h"
#include "../search/search.h"
#include "../input/keyboard.h"
#include "../list/list.h"
#include "../themes/rendering.h"
#include "../modal/modal.h"
#include "../download/downloadmanager.h"
#include "../helper/uihelper.h"

static void renderDefaults(app_t *app);

static void renderSettingsIcon(app_t *app);

static void renderDownloadManagerIcon(app_t *app);

static void renderBottomText(app_t *app);

void uihandler_render(app_t *app) {
    void (*renderEngine)(app_t *app) = NULL;
    renderDefaults(app);

    switch (app->win) {
        case window_search:
            if (app->systems.active == NULL) {
                app->systems.active = acll_first(app->systems.all);
            }
            renderEngine = &search_render;
            break;
        case window_keyboard:
            renderEngine = &keyboard_render;
            break;
        case window_downloadMgr:
            renderEngine = &downloadmanager_render;
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

static void renderDefaults(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    themes_setDrawColor(app, windowBackgroundActive);
    SDL_RenderClear(app->sdlRenderer);
    SDL_RenderCopy(app->sdlRenderer, getActiveTheme(app)->images.background, NULL, NULL);

    renderSettingsIcon(app);
    renderDownloadManagerIcon(app);
    renderBottomText(app);
}

static void renderSettingsIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - 35, 10, 25, 25);
    if (app->search.position == searchactivity_config) {
        themes_setDrawColor(app, iconBackgroundColorActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
        themes_setDrawColor(app, iconForegroundColorActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.inner);
    } else {
        themes_setDrawColor(app, iconBackgroundColorInactive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
        themes_setDrawColor(app, iconForegroundColorInactive);
        SDL_RenderFillRect(app->sdlRenderer, &element.inner);
    }

    uihelper_renderSDLTexture(app->sdlRenderer, getActiveTheme(app)->images.settingsIcon, &element.inner);

}

static void renderDownloadManagerIcon(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - 85, 10, 25, 25);
    if (app->search.position == searchactivity_downloadMgr) {
        themes_setDrawColor(app, iconBackgroundColorActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
        themes_setDrawColor(app, iconForegroundColorActive);
        SDL_RenderFillRect(app->sdlRenderer, &element.inner);
    } else {
        themes_setDrawColor(app, iconBackgroundColorInactive);
        SDL_RenderFillRect(app->sdlRenderer, &element.outter);
        themes_setDrawColor(app, iconForegroundColorInactive);
        SDL_RenderFillRect(app->sdlRenderer, &element.inner);
    }
    uihelper_renderSDLTexture(app->sdlRenderer, getActiveTheme(app)->images.downloadManagerIcon, &element.inner);

    int downloadCount = acll_count(app->download.all);
    if (downloadCount > 0) {
        if (app->search.position == searchactivity_downloadMgr) {
            themes_setDrawColor(app, iconDownloadsCircleActive);
        } else {
            themes_setDrawColor(app, iconDownloadsCircleInactive);

        }
        rendering_circle(app->sdlRenderer, width - 65, 32, 10);

        char buffer[4];
        sprintf(buffer, "%d", downloadCount);

        texture_t texture;
        if (app->search.position == searchactivity_downloadMgr) {
            rendering_loadText(app->sdlRenderer, &texture, buffer, getActiveTheme(app)->fonts.font16,
                               &getActiveTheme(app)->colors.iconDownloadsTextActive);
        } else {
            rendering_loadText(app->sdlRenderer, &texture, buffer, getActiveTheme(app)->fonts.font16,
                               &getActiveTheme(app)->colors.iconDownloadsTextInactive);
        }
        SDL_Rect srcQuad = {0, 0, width - 100 - 60, texture.h};
        SDL_Rect renderQuad = {width - 65 - texture.w / 2, 32 - texture.h / 2, texture.w, texture.h};
        SDL_RenderCopy(app->sdlRenderer, texture.texture, &srcQuad, &renderQuad);
        uihelper_destroyTexture(&texture);
    }
}

static void renderBottomText(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    csafestring_t *text = safe_create(COPYRIGHT);
    *text->data = 0xA9;

    safe_strcat(text, " - romlibrary: ");
    safe_strcat(text, RML_VERSION_STRING);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, text->data, getActiveTheme(app)->fonts.font16,
                       &getActiveTheme(app)->colors.windowCopyright);
    SDL_Rect renderQuad = {PADDING_SIDES, height - PADDING_BOTTOM + texture.h, texture.w, texture.h};
    SDL_RenderCopy(app->sdlRenderer, texture.texture, NULL, &renderQuad);
    safe_destroy(text);

    uihelper_destroyTexture(&texture);
}