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

#include "inputdownload.h"
#include <pthread.h>
#include "../engine/enginehandler.h"

static void downloadCallback(app_t *app);

static void startThread(app_t *app);

static void *executeThread(void *app_ptr);

static void moveToNext(app_t *app);

void inputdownload_processUp(app_t *app) {
    moveToNext(app);
}

void inputdownload_processDown(app_t *app) {
    moveToNext(app);
}

void inputdownload_processLeft(app_t *app) {
    moveToNext(app);
}

void inputdownload_processRight(app_t *app) {
    moveToNext(app);
}

void inputdownload_processSelect(app_t *app) {
    switch (app->download.cursorPos) {
        case downloadActivity_cancel:
            app->win = window_search;
            break;
        case downloadActivity_start:
            app->download.started = 1;
            startThread(app);
            break;
        case downloadActivity_done:
            app->download.cursorPos = downloadActivity_cancel;
            app->download.started = 0;
            app->download.complete = 0;
            app->win = window_search;
            break;
    }
}

void inputdownload_processBack(app_t *app) {
}

void inputdownload_processOtherButton(app_t *app, GameControllerState_t *state) {
}

static void moveToNext(app_t *app) {
    if (app->download.cursorPos == downloadActivity_done) {
        return;
    }
    app->download.cursorPos++;
    if (app->download.cursorPos > 1) {
        app->download.cursorPos = downloadActivity_cancel;
    }
}

static void *executeThread(void *app_ptr) {
    app_t *app = (app_t *) app_ptr;
    enginehandler_download(app, app->search.resultActive, &downloadCallback);
    return NULL;
}

static void startThread(app_t *app) {
    pthread_t downloadThread;
    if (pthread_create(&downloadThread, NULL, executeThread, app)) {
        SDL_Log("Error creating thread\n");
        app->download.started = 0;
        return;
    }
    pthread_detach(downloadThread);
}

static void downloadCallback(app_t *app) {
    app->download.started = 0;
    app->download.complete = 1;
    app->download.current = 0;
    app->download.total = 0;
    app->download.cursorPos = downloadActivity_done;
}