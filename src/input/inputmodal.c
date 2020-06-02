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

#include "inputmodal.h"

void inputmodal_processUp(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void inputmodal_processDown(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void inputmodal_processLeft(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void inputmodal_processRight(app_t *app) {
    if (app->modal.cancelButton != NULL) {
        app->modal.cursorPos = app->modal.cursorPos ? 0 : 1;
    }
}

void inputmodal_processSelect(app_t *app) {
    if (!app->modal.cursorPos && app->modal.callbackAction != NULL) {
        app->modal.callbackAction(app->modal.callbackData);
    }
    if (app->modal.cursorPos && app->modal.callbackCancel != NULL) {
        app->modal.callbackCancel(app->modal.callbackData);
    }
    app->modal.cursorPos = 0;
    app->modal.displayed = 0;
}

void inputmodal_processBack(app_t *app) {
    if (app->modal.callbackCancel != NULL) {
        app->modal.callbackCancel(app->modal.callbackData);
    }
    app->modal.cursorPos = 0;
    app->modal.displayed = 0;
}

void inputmodal_processOtherButton(app_t *app, GameControllerState_t *state) {}