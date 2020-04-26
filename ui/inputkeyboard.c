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

#include "inputkeyboard.h"
#include "../structs.h"

static void addActiveCharToText(app_t *app);

static void removeLastCharFromText(app_t *app);

void keyboard_processUp(app_t *app) {
    addActiveCharToText(app);
    app->keyboard.pointerPosition -= 3;
    if (app->keyboard.pointerPosition < 0) {
        app->keyboard.pointerPosition = 12 + app->keyboard.pointerPosition;
    }
}

void keyboard_processDown(app_t *app) {
    addActiveCharToText(app);
    app->keyboard.pointerPosition += 3;
    if (app->keyboard.pointerPosition > 11) {
        app->keyboard.pointerPosition = app->keyboard.pointerPosition % 3;
    }
}

void keyboard_processLeft(app_t *app) {
    addActiveCharToText(app);
    app->keyboard.pointerPosition--;
    if (app->keyboard.pointerPosition < 0 || app->keyboard.pointerPosition % 3 == 2) {
        app->keyboard.pointerPosition += 3;
    }
}

void keyboard_processRight(app_t *app) {
    addActiveCharToText(app);
    app->keyboard.pointerPosition++;
    if (app->keyboard.pointerPosition % 3 == 0) {
        app->keyboard.pointerPosition -= 3;
    }
}

void keyboard_processSelect(app_t *app) {
    switch (app->keyboard.pointerPosition) {
        case 0:
            if (app->keyboard.activeChar < '0' || app->keyboard.activeChar > '9') {
                addActiveCharToText(app);
                app->keyboard.activeChar = '0';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > '9') {
                app->keyboard.activeChar = '0';
            }
            break;
        case 1 :
            if (app->keyboard.activeChar < 'A' || app->keyboard.activeChar > 'C') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'A';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'C') {
                app->keyboard.activeChar = 'A';
            }
            break;
        case 2:
            if (app->keyboard.activeChar < 'D' || app->keyboard.activeChar > 'F') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'D';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'F') {
                app->keyboard.activeChar = 'D';
            }
            break;
        case 3:
            if (app->keyboard.activeChar < 'G' || app->keyboard.activeChar > 'I') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'J';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'I') {
                app->keyboard.activeChar = 'G';
            }
            break;
        case 4:
            if (app->keyboard.activeChar < 'J' || app->keyboard.activeChar > 'L') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'J';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'L') {
                app->keyboard.activeChar = 'J';
            }
            break;
        case 5:
            if (app->keyboard.activeChar < 'M' || app->keyboard.activeChar > 'O') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'M';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'O') {
                app->keyboard.activeChar = 'M';
            }
            break;
        case 6:
            if (app->keyboard.activeChar < 'P' || app->keyboard.activeChar > 'S') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'P';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'S') {
                app->keyboard.activeChar = 'P';
            }
            break;
        case 7:
            if (app->keyboard.activeChar < 'T' || app->keyboard.activeChar > 'V') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'T';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'V') {
                app->keyboard.activeChar = 'T';
            }
            break;
        case 8:
            if (app->keyboard.activeChar < 'W' || app->keyboard.activeChar > 'Z') {
                addActiveCharToText(app);
                app->keyboard.activeChar = 'W';
            } else {
                app->keyboard.activeChar++;
            }
            if (app->keyboard.activeChar > 'Z') {
                app->keyboard.activeChar = 'W';
            }
            break;
        case 9:
            removeLastCharFromText(app);
            break;
        case 10:
            addActiveCharToText(app);
            app->keyboard.activeChar = ' ';
            addActiveCharToText(app);
            break;
        case 11:
            addActiveCharToText(app);
            memcpy(app->search.searchText, app->keyboard.text, 256);
            app->win = search;
            break;
        default:
            break;
    }
}

void keyboard_processBack(app_t *app) {
    app->win = search;
}

void keyboard_processOtherButton(app_t *app, GameControllerState_t *state) {
    if (state->buttonY) {
        removeLastCharFromText(app);
    }
    if (state->buttonX) {
        addActiveCharToText(app);
        memcpy(app->search.searchText, app->keyboard.text, 256);
        app->win = search;
    }
}

static void addActiveCharToText(app_t *app) {
    int length = strlen(app->keyboard.text);
    if (length > 254 || app->keyboard.activeChar == 0) {
        return;
    }
    app->keyboard.text[length] = app->keyboard.activeChar;
    app->keyboard.text[length + 1] = '\0';
    app->keyboard.activeChar = 0;
}

static void removeLastCharFromText(app_t *app) {
    if (app->keyboard.activeChar == 0) {
        int length = strlen(app->keyboard.text);
        if (length == 0) {
            return;
        }
        app->keyboard.text[length - 1] = '\0';
        app->keyboard.activeChar = 0;
    } else {
        app->keyboard.activeChar = 0;
    }
}