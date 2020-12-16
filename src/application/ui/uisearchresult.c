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
#include "../helper/uihelper.h"
#include "../config.h"

static inline void drawHeadDivider(app_t *app, uiElementRects_t *rects);

static inline void drawDivider(app_t *app, uiElementRects_t *rects);

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

    int deviceCountToDisplay = (height - offset - 80) / LIST_ITEM_HEIGHT + 1;

    int i = 0;
    while (i < deviceCountToDisplay / 2 - 1) {
        if (element->prev == NULL) {
            break;
        }
        element = element->prev;
        i++;
    }

    int position = offset;

    uiElementRects_t rects = uihelper_generateRectsFullScreenWidth(20, position, width, LIST_ITEM_HEIGHT);
    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &rects.outter);
    themes_setDrawColorBackground(app, 1);
    SDL_RenderFillRect(app->sdlRenderer, &rects.inner);
    position += LIST_ITEM_HEIGHT;

    rendering_loadText(app, &texture, "Game Title", app->fonts.font24, &app->themes.active->colors.text);
    rects.content.x += LIST_ITEM_HEIGHT - 5;
    uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
    SDL_DestroyTexture(texture.texture);

    rendering_loadText(app, &texture, "Dwn", app->fonts.font24, &app->themes.active->colors.text);
    rects.content.x = width - 3 * width / 10;
    rects.content.w = width / 10;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    SDL_DestroyTexture(texture.texture);
    drawHeadDivider(app, &rects);

    rendering_loadText(app, &texture, "Rat", app->fonts.font24, &app->themes.active->colors.text);
    rects.content.x = width - 2 * width / 10;
    rects.content.w = width / 10 - 25;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    SDL_DestroyTexture(texture.texture);
    drawHeadDivider(app, &rects);

    rendering_loadText(app, &texture, "Size", app->fonts.font24, &app->themes.active->colors.text);
    rects.content.x = width - width / 10 - 25;
    rects.content.w = width / 10;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    SDL_DestroyTexture(texture.texture);
    drawHeadDivider(app, &rects);

    while (element != NULL && position <= height - 80) {
        rects = uihelper_generateRectsFullScreenWidth(20, position, width, LIST_ITEM_HEIGHT);
        themes_setDrawColorBackground(app, (element == app->search.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.outter);

        themes_setDrawColorField(app);
        SDL_RenderFillRect(app->sdlRenderer, &rects.inner);

        // Favicon
        uiElementRects_t iconItem = uihelper_generateRects(25, position + 5, LIST_ITEM_HEIGHT - 10,
                                                           LIST_ITEM_HEIGHT - 10);
        // TODO: Keep Texture
        if (element->hoster->favicon != NULL) {
            if (element->hoster->favicon->size > 0 && element->hoster->favicon->binary != NULL) {
                SDL_Texture *icon = rendering_memImage(app, element->hoster->favicon->binary,
                                                       element->hoster->favicon->size);
                if (icon != NULL) {
                    uihelper_renderSDLTexture(app->sdlRenderer, icon, &iconItem.inner);
                    SDL_DestroyTexture(icon);
                }
            }
        }

        // Title
        rects = uihelper_generateRects(20 + LIST_ITEM_HEIGHT - 5, position,
                                       6 * width / 10, LIST_ITEM_HEIGHT);
        rendering_loadText(app, &texture, element->title, app->fonts.font24, &app->themes.active->colors.text);
        uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
        SDL_DestroyTexture(texture.texture);

        // Downloads
        char buffer[7];
        snprintf(buffer, 7, "%d", element->downloads);
        rects = uihelper_generateRects(width - 3 * width / 10, position, width / 10,
                                       LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rendering_loadText(app, &texture, buffer, app->fonts.font24, &app->themes.active->colors.text);
        uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
        SDL_DestroyTexture(texture.texture);
        drawDivider(app, &rects);

        // Rating
        snprintf(buffer, 7, "%2.1f", element->rating);
        rects = uihelper_generateRects(width - 2 * width / 10, position, width / 10 - 25, LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rendering_loadText(app, &texture, buffer, app->fonts.font24, &app->themes.active->colors.text);
        uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
        SDL_DestroyTexture(texture.texture);
        drawDivider(app, &rects);

        // File size
        rects = uihelper_generateRects(width - width / 10 - 25, position, width / 10, LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rects.content.x += 3;
        rects.content.w -= 3;
        if (element->fileSize != NULL) {
            rendering_loadText(app, &texture, element->fileSize, app->fonts.font24, &app->themes.active->colors.text);
            uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
            SDL_DestroyTexture(texture.texture);
        }
        drawDivider(app, &rects);

        element = element->next;
        position += LIST_ITEM_HEIGHT;
    }
}

static inline void drawHeadDivider(app_t *app, uiElementRects_t *rects) {
    SDL_RenderDrawLine(app->sdlRenderer, rects->content.x, rects->inner.y, rects->content.x,
                       rects->inner.y + rects->inner.h);
}

static inline void drawDivider(app_t *app, uiElementRects_t *rects) {
    themes_setDrawColorBackground(app, 0);
    SDL_RenderDrawLine(app->sdlRenderer, rects->inner.x, rects->inner.y, rects->inner.x,
                       rects->inner.y + rects->inner.h);
}