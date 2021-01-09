/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
 *
 * Licensed under the Apache License, Version 2.0 (the "License"){}
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
#include "downloadmanager.h"
#include "downloadpipeline.h"
#include "../../common/utils.h"
#include "../state/statehandler.h"
#include "downloader/downloader.h"
#include "../ui/rendering.h"
#include "../themes/rendering.h"
#include "../helper/uihelper.h"

static void modalCancelDownload(void *appPtr, void *data);

static void modalNoCancelDownload(void *appPtr, void *data);

void downloadmanager_processUp(app_t *app) {
    download_t *prev = downloadpipeline_getPrev(app, app->download.cursor);
    if (prev != NULL) {
        app->download.cursor = prev;
    }
}

void downloadmanager_processDown(app_t *app) {
    download_t *next = downloadpipeline_getNext(app, app->download.cursor);
    if (next != NULL) {
        app->download.cursor = next;
    }
}

void downloadmanager_processLeft(app_t *app) {}

void downloadmanager_processRight(app_t *app) {}

void downloadmanager_processSelect(app_t *app) {
    if (!linkedlist_isElementInList(app->download.done, app->download.cursor)) {
        app->modal.displayed = 1;
        app->modal.headline = "Cancel Download?";
        app->modal.actionButton = "Yes";
        app->modal.cancelButton = "No";
        app->modal.cursorPos = 0;
        app->modal.app = app;
        app->modal.callbackData = app->download.cursor;
        app->modal.callbackAction = &modalCancelDownload;
        app->modal.callbackCancel = &modalNoCancelDownload;

        csafestring_t *tmp = safe_create("Sure you want to cancel this download?\n\n");
        safe_strcat(tmp, app->download.cursor->title);
        app->modal.text = str_clone(tmp->data);
        safe_destroy(tmp);
    }
}

void downloadmanager_processBack(app_t *app) {
    statehandler_switch(app, 0);
}

void downloadmanager_processOtherButton(app_t *app, controller_t *state) {}

void downloadmanager_processOtherKey(app_t *app, SDL_Scancode scancode) {}

static void modalNoCancelDownload(void *appPtr, void *data) {
    app_t *app = appPtr;
    app->modal.displayed = 0;
    free(app->modal.text);
}

static void modalCancelDownload(void *appPtr, void *data) {
    app_t *app = appPtr;
    app->modal.displayed = 0;
    free(app->modal.text);

    downloader_cancel(app, data);
}

window_t downloadmanager_stateTarget(app_t *app, uint8_t isSelectButton) {
    return window_search;
}

void downloadmanager_statePersist(app_t *app) {
}

void downloadmanager_stateInit(app_t *app) {
    app->win = window_downloadMgr;
    if (app->download.active != NULL) {
        app->download.cursor = app->download.active;
    } else if (app->download.queue != NULL) {
        app->download.cursor = app->download.queue;
    } else if (app->download.done != NULL) {
        app->download.cursor = app->download.done;
    } else {
        app->download.cursor = NULL;
    }
}

static void renderProgressBar(app_t *app, download_t *element, int position);

void downloadmanager_render(app_t *app) {
    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    int itemsToDisplay = (height - PADDING_TOP - PADDING_BOTTOM) / (LIST_ITEM_HEIGHT * 2) + 1;

    download_t *element = (app->download.cursor != NULL) ? app->download.cursor : app->download.active;
    if (element == NULL) {
        return;
    }

    int i = 0;
    while (i < itemsToDisplay / 2 - 1) {
        download_t *prev = downloadpipeline_getPrev(app, element);
        if (prev == NULL) {
            break;
        }
        element = prev;
        i++;
    }

    int position = PADDING_TOP;
    while (element != NULL && position <= height - PADDING_BOTTOM) {
        uiElementRects_t rects = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, position, width,
                                                                       2 * LIST_ITEM_HEIGHT);
        themes_setDrawColorDwnItemBackground(app, (element == app->download.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.outter);

        themes_setDrawColorDwnItemForeground(app, (element == app->download.cursor));
        SDL_RenderFillRect(app->sdlRenderer, &rects.inner);

        renderProgressBar(app, element, position);

        element = downloadpipeline_getNext(app, element);
        position += 65;
    }
}

static void renderProgressBar(app_t *app, download_t *element, int position) {
    int width, height;
    texture_t texture;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t textRect = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, position, width, LIST_ITEM_HEIGHT);
    rendering_loadText(app->sdlRenderer, &texture, element->title, app->themes.active->fonts.font24,
                       themes_getDrawColorDwnItemText(app, (element == app->download.cursor)));
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &textRect.content);
    uihelper_destroyTexture(&texture);

    uiElementRects_t progressBarRect = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, position + LIST_ITEM_HEIGHT,
                                                                             width, LIST_ITEM_HEIGHT);
    themes_setDrawColorDwnProgressBackground(app, (element == app->download.cursor));
    SDL_RenderFillRect(app->sdlRenderer, &progressBarRect.outter);

    themes_setDrawColorDwnProgressForeground(app, (element == app->download.cursor));
    SDL_RenderFillRect(app->sdlRenderer, &progressBarRect.inner);

    double percentage;
    char percentText[10];
    if (linkedlist_isElementInList(app->download.active, element)) {
        percentage = element->total != 0 ? (double) element->current / (double) element->total : 0.0;
        snprintf(percentText, 5, "%.0f%c", percentage * 100, '%');
    } else if (linkedlist_isElementInList(app->download.queue, element)) {
        percentage = 0.0f;
        strcpy (percentText, "QUEUED");
    } else {
        if (element->cancelled) {
            percentage = 0.0f;
            strcpy (percentText, "CANCELLED");
        } else {
            percentage = 1.0f;
            strcpy (percentText, "DONE");
        }
    }

    int contentWidth = progressBarRect.content.w;
    progressBarRect.content.w = contentWidth * percentage;
    themes_setDrawColorDwnProgressState(app, (element == app->download.cursor));
    SDL_RenderFillRect(app->sdlRenderer, &progressBarRect.content);
    progressBarRect.content.w = contentWidth;

    rendering_loadText(app->sdlRenderer, &texture, percentText, app->themes.active->fonts.font16,
                       themes_getDrawColorDwnProgressText(app, (element == app->download.cursor)));
    uihelper_renderTextureCentered(app->sdlRenderer, &texture, &progressBarRect.content);
    uihelper_destroyTexture(&texture);
}