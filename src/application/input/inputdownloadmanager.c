/*
 * Copyright 2020 Maximilian Voss (maximilian@voss.rocks)
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
#include "inputdownloadmanager.h"
#include "../download/downloadpipeline.h"
#include "../../common/utils.h"
#include "../state/statehandler.h"
#include "../download/downloader.h"

static void modalCancelDownload(void *appPtr, void *data);

static void modalNoCancelDownload(void *appPtr, void *data);

void inputdownloadmanager_processUp(app_t *app) {
    download_t *prev = downloadpipeline_getPrev(app, app->download.cursor);
    if (prev != NULL) {
        app->download.cursor = prev;
    }
}

void inputdownloadmanager_processDown(app_t *app) {
    download_t *next = downloadpipeline_getNext(app, app->download.cursor);
    if (next != NULL) {
        app->download.cursor = next;
    }
}

void inputdownloadmanager_processLeft(app_t *app) {}

void inputdownloadmanager_processRight(app_t *app) {}

void inputdownloadmanager_processSelect(app_t *app) {
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

void inputdownloadmanager_processBack(app_t *app) {
    statehandler_switch(app, 0);
}

void inputdownloadmanager_processOtherButton(app_t *app, GameControllerState_t *state) {}

void inputdownloadmanager_processOtherKey(app_t *app, SDL_Scancode scancode) {}

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