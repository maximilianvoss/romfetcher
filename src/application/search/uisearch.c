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

#include "uisearch.h"
#include "../ui/rendering.h"
#include "../themes/rendering.h"
#include "../helper/uihelper.h"
#include "../constants.h"

static void renderSearchField(app_t *app);

static void renderSearchButton(app_t *app);

static void renderSystemSelector(app_t *app);

static void renderSearchResult(app_t *app, int offset);

static inline void drawHeadDivider(app_t *app, uiElementRects_t *rects);

static inline void drawDivider(app_t *app, uiElementRects_t *rects);

void uisearch_render(app_t *app) {
    renderSystemSelector(app);
    renderSearchField(app);
    renderSearchButton(app);
    renderSearchResult(app, 2 * (LIST_ITEM_HEIGHT + 10) + 50);
}

static void renderSystemSelector(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRectsFullScreenWidth(20, 50, width, LIST_ITEM_HEIGHT);
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_system));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    SDL_Rect texture_rect = {width - element.outter.x - element.outter.h, element.outter.y, element.outter.h,
                             element.outter.h};
    SDL_RenderCopy(app->sdlRenderer, app->themes.active->images.searchChevron, NULL, &texture_rect);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, app->systems.active->fullname, app->themes.active->fonts.font24,
                       &app->themes.active->colors.text);
    uihelper_renderTexture(app->sdlRenderer, &texture, &element.content);
    uihelper_destroyTexture(&texture);
}

static void renderSearchField(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(20, 60 + LIST_ITEM_HEIGHT, width - width / 3 - 5,
                                                      LIST_ITEM_HEIGHT);
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_field));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    if (*(app->search.searchText) != '\0') {
        texture_t texture;
        rendering_loadText(app->sdlRenderer, &texture, app->search.searchText, app->themes.active->fonts.font24,
                           &app->themes.active->colors.text);
        uihelper_renderTexture(app->sdlRenderer, &texture, &element.content);
        uihelper_destroyTexture(&texture);
    }
}

static void renderSearchButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - width / 3 + 30, 60 + LIST_ITEM_HEIGHT, width / 3 - 50,
                                                      LIST_ITEM_HEIGHT);
    themes_setDrawColorBackground(app, (app->search.position == searchactivity_button));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorField(app);
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, "Search", app->themes.active->fonts.font24,
                       &app->themes.active->colors.text);
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &element.content);
    uihelper_destroyTexture(&texture);
}

static void renderSearchResult(app_t *app, int offset) {
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

    rendering_loadText(app->sdlRenderer, &texture, "Game Title", app->themes.active->fonts.font24,
                       &app->themes.active->colors.text);
    rects.content.x += LIST_ITEM_HEIGHT - 5;
    uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);

    rendering_loadText(app->sdlRenderer, &texture, "Dwn", app->themes.active->fonts.font24,
                       &app->themes.active->colors.text);
    rects.content.x = width - 3 * width / 10;
    rects.content.w = width / 10;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);
    drawHeadDivider(app, &rects);

    rendering_loadText(app->sdlRenderer, &texture, "Rat", app->themes.active->fonts.font24,
                       &app->themes.active->colors.text);
    rects.content.x = width - 2 * width / 10;
    rects.content.w = width / 10 - 25;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);
    drawHeadDivider(app, &rects);

    rendering_loadText(app->sdlRenderer, &texture, "Size", app->themes.active->fonts.font24,
                       &app->themes.active->colors.text);
    rects.content.x = width - width / 10 - 25;
    rects.content.w = width / 10;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);
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
                SDL_Texture *icon = rendering_memImage(app->sdlRenderer, element->hoster->favicon->binary,
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
        rendering_loadText(app->sdlRenderer, &texture, element->title, app->themes.active->fonts.font24,
                           &app->themes.active->colors.text);
        uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
        uihelper_destroyTexture(&texture);

        // Downloads
        char buffer[7];
        snprintf(buffer, 7, "%d", element->downloads);
        rects = uihelper_generateRects(width - 3 * width / 10, position, width / 10,
                                       LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rendering_loadText(app->sdlRenderer, &texture, buffer, app->themes.active->fonts.font24,
                           &app->themes.active->colors.text);
        uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
        uihelper_destroyTexture(&texture);
        drawDivider(app, &rects);

        // Rating
        snprintf(buffer, 7, "%2.1f", element->rating);
        rects = uihelper_generateRects(width - 2 * width / 10, position, width / 10 - 25, LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rendering_loadText(app->sdlRenderer, &texture, buffer, app->themes.active->fonts.font24,
                           &app->themes.active->colors.text);
        uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
        uihelper_destroyTexture(&texture);
        drawDivider(app, &rects);

        // File size
        rects = uihelper_generateRects(width - width / 10 - 25, position, width / 10, LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rects.content.x += 3;
        rects.content.w -= 3;
        if (element->fileSize != NULL) {
            rendering_loadText(app->sdlRenderer, &texture, element->fileSize, app->themes.active->fonts.font24,
                               &app->themes.active->colors.text);
            uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
            uihelper_destroyTexture(&texture);
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