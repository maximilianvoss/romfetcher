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
#include "search.h"
#include "../state/statehandler.h"
#include "../ui/rendering.h"
#include "../themes/rendering.h"
#include "../helper/uihelper.h"
#include "../constants.h"
#include "../download/downloader.h"

static void modalDownload(void *appPtr, void *data);

static void renderSearchField(app_t *app);

static void renderSearchButton(app_t *app);

static void renderSystemSelector(app_t *app);

static void renderSearchResult(app_t *app, int offset);

static inline void drawHeadDivider(app_t *app, uiElementRects_t *rects);

static inline void drawDivider(app_t *app, uiElementRects_t *rects);

void search_processUp(app_t *app) {
    switch (app->search.position) {
        case searchactivity_downloadMgr:
        case searchactivity_config:
            break;
        case searchactivity_system:
            app->search.position = searchactivity_config;
            break;
        case searchactivity_field:
        case searchactivity_button:
            app->search.position = searchactivity_system;
            break;
        case searchactivity_results:
            if (app->search.cursor->prev == NULL) {
                app->search.position = searchactivity_field;
                app->search.cursor = NULL;
            } else {
                app->search.cursor = app->search.cursor->prev;
            }
            break;
    }
}

void search_processDown(app_t *app) {
    switch (app->search.position) {
        case searchactivity_downloadMgr:
        case searchactivity_config:
            app->search.position = searchactivity_system;
            break;
        case searchactivity_system:
            app->search.position = searchactivity_field;
            break;
        case searchactivity_field:
        case searchactivity_button:
            if (app->search.all != NULL) {
                app->search.cursor = app->search.all;
                app->search.position = searchactivity_results;
            }
            break;
        case searchactivity_results:
            if (app->search.cursor->next != NULL) {
                app->search.cursor = app->search.cursor->next;
            }
            break;
    }
}

void search_processLeft(app_t *app) {
    switch (app->search.position) {
        case searchactivity_downloadMgr:
            break;
        case searchactivity_config:
            app->search.position = searchactivity_downloadMgr;
            break;
        case searchactivity_system:
            break;
        case searchactivity_field:
            app->search.position = searchactivity_system;
            break;
        case searchactivity_button:
            app->search.position = searchactivity_field;
            break;
        case searchactivity_results:
            if (app->search.cursor->prev == NULL) {
                app->search.position = searchactivity_button;
            } else {
                app->search.cursor = app->search.cursor->prev;
            }
            break;
    }
}

void search_processRight(app_t *app) {
    switch (app->search.position) {
        case searchactivity_downloadMgr:
            app->search.position = searchactivity_config;
            break;
        case searchactivity_config:
            break;
        case searchactivity_system:
            app->search.position = searchactivity_field;
            break;
        case searchactivity_field:
            app->search.position = searchactivity_button;
            break;
        case searchactivity_button:
            if (app->search.all != NULL) {
                app->search.cursor = app->search.all;
                app->search.position = searchactivity_results;
            }
            break;
        case searchactivity_results:
            if (app->search.cursor->next != NULL) {
                app->search.cursor = app->search.cursor->next;
            }
            break;
    }
}

void search_processSelect(app_t *app) {
    statehandler_switch(app, 1);
}

void search_processBack(app_t *app) {
}

void search_processOtherButton(app_t *app, controller_t *state) {
}

void search_processOtherKey(app_t *app, SDL_Scancode scancode) {}

static void modalDownload(void *appPtr, void *data) {
    app_t *app = appPtr;
    app->modal.displayed = 0;
    rl_download(rl_getResult(app->search.cursor), downloader_addToQueue, app);
}

window_t search_stateTarget(app_t *app, uint8_t isSelectButton) {
    switch (app->search.position) {
        case searchactivity_config:
            return window_config;
        case searchactivity_system:
            return window_system;
        case searchactivity_field:
            return window_keyboard;
        case searchactivity_results:
            app->modal.displayed = 1;
            app->modal.headline = "Start Download";
            csafestring_t *text = safe_create("Shall we start download for:\n");
            safe_strcat(text, rl_getResult(app->search.cursor)->title);
            app->modal.text = text->data;
            app->modal.app = app;
            app->modal.cursorPos = 0;
            app->modal.actionButton = "Download";
            app->modal.callbackAction = modalDownload;
            app->modal.cancelButton = "Cancel";
            app->modal.callbackData = NULL;
            app->modal.callbackCancel = NULL;
            return window_search;
        case searchactivity_downloadMgr:
            return window_downloadMgr;
        default:
            return window_search;
    }
}

void search_statePersist(app_t *app) {
    if (app->search.position == searchactivity_button) {
        if (strlen(app->search.searchText) > 2) {
            if (app->search.all != NULL) {
                rl_results_free(app->search.all);
            }
            app->search.all = rl_search(app->engine.all, rl_getSystem(app->systems.active), app->search.searchText);
        }
    }
}

void search_stateInit(app_t *app) {
    app->win = window_search;
}

void search_render(app_t *app) {
    renderSystemSelector(app);
    renderSearchField(app);
    renderSearchButton(app);
    renderSearchResult(app, 2 * (LIST_ITEM_HEIGHT + 10) + PADDING_TOP);
}

static void renderSystemSelector(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, PADDING_TOP, width,
                                                                     LIST_ITEM_HEIGHT);
    themes_setDrawColorFieldBackground(app, (app->search.position == searchactivity_system));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorFieldForeground(app, (app->search.position == searchactivity_system));
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    SDL_Rect texture_rect = {width - element.outter.x - element.outter.h, element.outter.y, element.outter.h,
                             element.outter.h};
    SDL_RenderCopy(app->sdlRenderer, getActiveTheme(app)->images.searchChevron, NULL, &texture_rect);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, rl_getSystem(app->systems.active)->fullname,
                       getActiveTheme(app)->fonts.font24,
                       themes_getDrawColorFieldText(app, (app->search.position == searchactivity_system)));
    uihelper_renderTexture(app->sdlRenderer, &texture, &element.content);
    uihelper_destroyTexture(&texture);
}

static void renderSearchField(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(PADDING_SIDES, PADDING_TOP + 10 + LIST_ITEM_HEIGHT,
                                                      width - width / 3 - 5,
                                                      LIST_ITEM_HEIGHT);
    themes_setDrawColorFieldBackground(app, (app->search.position == searchactivity_field));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorFieldForeground(app, (app->search.position == searchactivity_field));
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    if (*(app->search.searchText) != '\0') {
        texture_t texture;
        rendering_loadText(app->sdlRenderer, &texture, app->search.searchText, getActiveTheme(app)->fonts.font24,
                           themes_getDrawColorFieldText(app, (app->search.position == searchactivity_field)));
        uihelper_renderTexture(app->sdlRenderer, &texture, &element.content);
        uihelper_destroyTexture(&texture);
    }
}

static void renderSearchButton(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t element = uihelper_generateRects(width - width / 3 + 30, PADDING_TOP + 10 + LIST_ITEM_HEIGHT,
                                                      width / 3 - PADDING_SIDES - 30,
                                                      LIST_ITEM_HEIGHT);
    themes_setDrawColorFieldBackground(app, (app->search.position == searchactivity_button));
    SDL_RenderFillRect(app->sdlRenderer, &element.outter);

    themes_setDrawColorFieldForeground(app, (app->search.position == searchactivity_button));
    SDL_RenderFillRect(app->sdlRenderer, &element.inner);

    texture_t texture;
    rendering_loadText(app->sdlRenderer, &texture, "Search", getActiveTheme(app)->fonts.font24,
                       themes_getDrawColorFieldText(app, (app->search.position == searchactivity_button)));
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &element.content);
    uihelper_destroyTexture(&texture);
}

static void renderSearchResult(app_t *app, int offset) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    texture_t texture;
    acll_t *element = app->search.cursor;

    if (element == NULL) {
        element = app->search.all;
    }

    if (element == NULL) {
        return;
    }

    int itemCountToDisplay = (height - offset - PADDING_BOTTOM) / LIST_ITEM_HEIGHT + 1;

    int i = 0;
    while (i < itemCountToDisplay / 2 - 1) {
        if (element->prev == NULL) {
            break;
        }
        element = element->prev;
        i++;
    }

    int position = offset;

    uiElementRects_t rects = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, position, width, LIST_ITEM_HEIGHT);
    themes_setDrawColor(app, resultHeaderBackbround);
    SDL_RenderFillRect(app->sdlRenderer, &rects.outter);
    themes_setDrawColor(app, resultHeaderForeground);
    SDL_RenderFillRect(app->sdlRenderer, &rects.inner);
    position += LIST_ITEM_HEIGHT;

    rendering_loadText(app->sdlRenderer, &texture, "Game Title", getActiveTheme(app)->fonts.font24,
                       themes_getDrawColor(app, resultHeaderText));
    rects.content.x += LIST_ITEM_HEIGHT - 5;
    uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);

    rendering_loadText(app->sdlRenderer, &texture, "Dwn", getActiveTheme(app)->fonts.font24,
                       themes_getDrawColor(app, resultHeaderText));
    rects.content.x = width - 3 * width / 10;
    rects.content.w = width / 10;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);
    drawHeadDivider(app, &rects);

    rendering_loadText(app->sdlRenderer, &texture, "Rat", getActiveTheme(app)->fonts.font24,
                       themes_getDrawColor(app, resultHeaderText));
    rects.content.x = width - 2 * width / 10;
    rects.content.w = width / 10 - 25;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);
    drawHeadDivider(app, &rects);

    rendering_loadText(app->sdlRenderer, &texture, "Size", getActiveTheme(app)->fonts.font24,
                       themes_getDrawColor(app, resultHeaderText));
    rects.content.x = width - width / 10 - 25;
    rects.content.w = width / 10;
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &rects.content);
    uihelper_destroyTexture(&texture);
    drawHeadDivider(app, &rects);

    while (element != NULL && position <= height - PADDING_BOTTOM) {
        rects = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, position, width, LIST_ITEM_HEIGHT);
        themes_setDrawColorResultBackground(app, (element == app->search.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.outter);

        themes_setDrawColorResultForeground(app, (element == app->search.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.inner);

        // Favicon
        uiElementRects_t iconItem = uihelper_generateRects(25, position + 5, LIST_ITEM_HEIGHT - 10,
                                                           LIST_ITEM_HEIGHT - 10);
        // TODO: Keep Texture
        if (rl_getResult(element)->hoster->favicon != NULL) {
            if (rl_getResult(element)->hoster->favicon->size > 0 &&
                rl_getResult(element)->hoster->favicon->binary != NULL) {
                SDL_Texture *icon = rendering_memImage(app->sdlRenderer, rl_getResult(element)->hoster->favicon->binary,
                                                       rl_getResult(element)->hoster->favicon->size);
                if (icon != NULL) {
                    uihelper_renderSDLTexture(app->sdlRenderer, icon, &iconItem.inner);
                    SDL_DestroyTexture(icon);
                }
            }
        }

        // Title
        rects = uihelper_generateRects(PADDING_SIDES + LIST_ITEM_HEIGHT - 5, position,
                                       6 * width / 10, LIST_ITEM_HEIGHT);
        rendering_loadText(app->sdlRenderer, &texture, rl_getResult(element)->title, getActiveTheme(app)->fonts.font24,
                           themes_getDrawColorResultText(app, (element == app->search.cursor)));
        uihelper_renderTexture(app->sdlRenderer, &texture, &rects.content);
        uihelper_destroyTexture(&texture);

        // Downloads
        char buffer[7];
        snprintf(buffer, 7, "%d", rl_getResult(element)->downloads);
        rects = uihelper_generateRects(width - 3 * width / 10, position, width / 10,
                                       LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rendering_loadText(app->sdlRenderer, &texture, buffer, getActiveTheme(app)->fonts.font24,
                           themes_getDrawColorResultText(app, (element == app->search.cursor)));
        uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
        uihelper_destroyTexture(&texture);
        drawDivider(app, &rects);

        // Rating
        snprintf(buffer, 7, "%2.1f", rl_getResult(element)->rating);
        rects = uihelper_generateRects(width - 2 * width / 10, position, width / 10 - 25, LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rendering_loadText(app->sdlRenderer, &texture, buffer, getActiveTheme(app)->fonts.font24,
                           themes_getDrawColorResultText(app, (element == app->search.cursor)));
        uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
        uihelper_destroyTexture(&texture);
        drawDivider(app, &rects);

        // File size
        rects = uihelper_generateRects(width - width / 10 - 25, position, width / 10, LIST_ITEM_HEIGHT);
        uihelper_noPaddingX(&rects);
        rects.content.x += 3;
        rects.content.w -= 3;
        if (rl_getResult(element)->fileSize != NULL) {
            rendering_loadText(app->sdlRenderer, &texture, rl_getResult(element)->fileSize,
                               getActiveTheme(app)->fonts.font24,
                               themes_getDrawColorResultText(app, (element == app->search.cursor)));
            uihelper_renderTextureRight(app->sdlRenderer, &texture, &rects.content);
            uihelper_destroyTexture(&texture);
        }
        drawDivider(app, &rects);

        element = element->next;
        position += LIST_ITEM_HEIGHT;
    }
}

static inline void drawHeadDivider(app_t *app, uiElementRects_t *rects) {
    themes_setDrawColor(app, resultDivider);
    SDL_RenderDrawLine(app->sdlRenderer, rects->content.x, rects->inner.y, rects->content.x,
                       rects->inner.y + rects->inner.h);
}

static inline void drawDivider(app_t *app, uiElementRects_t *rects) {
    themes_setDrawColor(app, resultDivider);
    SDL_RenderDrawLine(app->sdlRenderer, rects->inner.x, rects->inner.y, rects->inner.x,
                       rects->inner.y + rects->inner.h);
}