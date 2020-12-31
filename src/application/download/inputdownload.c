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
#include "../state/statehandler.h"
#include "downloader.h"

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
        case downloadActivity_start:
            downloadItem(app->search.active, downloader_addToQueue, app);
            break;
        default:
            break;
    }
    statehandler_switch(app, 1);
}

void inputdownload_processBack(app_t *app) {
    statehandler_switch(app, 0);
}

void inputdownload_processOtherButton(app_t *app, GameControllerState_t *state) {
}

void inputdownload_processOtherKey(app_t *app, SDL_Scancode scancode) {
}

static void moveToNext(app_t *app) {
    app->download.cursorPos++;
    if (app->download.cursorPos > 1) {
        app->download.cursorPos = downloadActivity_cancel;
    }
}