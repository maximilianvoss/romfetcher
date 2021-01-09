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

#include "keyboard.h"
#include "../state/statehandler.h"
#include "../themes/rendering.h"
#include "../ui/rendering.h"
#include "../helper/uihelper.h"
#include "../constants.h"

static void addActiveCharToText(app_t *app);

static void removeLastCharFromText(app_t *app);

static void renderKey(app_t *app, int posx, int posy, int padWidth, int padHeight, uint8_t active, char *text);

static void renderSearchField(app_t *app);

static void renderDailPad(app_t *app);

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
            statehandler_switch(app, 1);
            break;
        default:
            break;
    }
}

void keyboard_processBack(app_t *app) {
    removeLastCharFromText(app);
}

void keyboard_processOtherButton(app_t *app, controller_t *state) {
    if (state->buttonY) {
        removeLastCharFromText(app);
    }
    if (state->buttonX) {
        addActiveCharToText(app);
        statehandler_switch(app, 1);
    }
}

void keyboard_processOtherKey(app_t *app, SDL_Scancode scancode) {
    if (scancode >= SDL_SCANCODE_A && scancode <= SDL_SCANCODE_0) {
        const char *key = SDL_GetScancodeName(scancode);
        app->keyboard.activeChar = *key;
        addActiveCharToText(app);
    }
    if (scancode == SDL_SCANCODE_SPACE) {
        app->keyboard.activeChar = ' ';
        addActiveCharToText(app);
    }
}


void keyboard_render(app_t *app) {
    renderSearchField(app);
    renderDailPad(app);
}

static void renderSearchField(app_t *app) {
    int width, height;
    int charPos;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    uiElementRects_t fullTextRect = uihelper_generateRectsFullScreenWidth(PADDING_SIDES, PADDING_TOP, width,
                                                                          LIST_ITEM_HEIGHT);
    themes_setDrawColorFieldBackground(app, 0);
    SDL_RenderFillRect(app->sdlRenderer, &fullTextRect.outter);

    themes_setDrawColorFieldForeground(app, 0);
    SDL_RenderFillRect(app->sdlRenderer, &fullTextRect.inner);

    charPos = fullTextRect.content.x;
    if (*(app->keyboard.text) != '\0') {
        texture_t texture;
        rendering_loadText(app->sdlRenderer, &texture, app->keyboard.text, app->themes.active->fonts.font24,
                           themes_getDrawColor(app, keyboardText));

        uihelper_renderTexture(app->sdlRenderer, &texture, &fullTextRect.content);
        charPos = fullTextRect.content.x + texture.w;
        uihelper_destroyTexture(&texture);
    }

    if (app->keyboard.activeChar != '\0') {
        char miniString[2] = {app->keyboard.activeChar, '\0'};
        texture_t texture;
        rendering_loadText(app->sdlRenderer, &texture, miniString, app->themes.active->fonts.font24,
                           themes_getDrawColor(app, keyboardActiveChar));

        fullTextRect.content.x = charPos;
        uihelper_renderTexture(app->sdlRenderer, &texture, &fullTextRect.content);
        uihelper_destroyTexture(&texture);
    }
}

static void renderDailPad(app_t *app) {
    const char keys[4][3][6] = {
            {"0-9",  "ABC",   "DEF"},
            {"GHI",  "JKL",   "MNO"},
            {"PQRS", "TUV",   "WXYZ"},
            {"DEL",  "SPACE", "SAVE"}
    };

    int width, height;
    SDL_GL_GetDrawableSize(app->sdlWindow, &width, &height);

    int padWidth = (width - 2 * PADDING_SIDES) / 4;
    int padHeight = (height - PADDING_TOP - PADDING_BOTTOM - 2 * LIST_ITEM_HEIGHT) / 4 - 20;
    int posX = (width / 2) - 1.5 * padWidth - 20;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            renderKey(app, posX + i * (padWidth + 20), PADDING_TOP + 2 * LIST_ITEM_HEIGHT + j * (padHeight + 20),
                      padWidth, padHeight,
                      app->keyboard.pointerPosition == i + j * 3 ? 1 : 0, (char *) keys[j][i]);
        }
    }
}

static void renderKey(app_t *app, int posx, int posy, int padWidth, int padHeight, uint8_t active, char *text) {
    uiElementRects_t keyRect = uihelper_generateRects(posx, posy, padWidth, padHeight);

    themes_setDrawColorFieldBackground(app, active);
    SDL_RenderFillRect(app->sdlRenderer, &keyRect.outter);

    themes_setDrawColorFieldForeground(app, active);
    SDL_RenderFillRect(app->sdlRenderer, &keyRect.inner);

    if (text != NULL && *text != '\0') {
        texture_t texture;
        rendering_loadText(app->sdlRenderer, &texture, text, app->themes.active->fonts.font26,
                           themes_getDrawColor(app, keyboardText));
        uihelper_renderTextureCentered(app->sdlRenderer, &texture, &keyRect.inner);
        uihelper_destroyTexture(&texture);
    }
}

static void addActiveCharToText(app_t *app) {
    int length = strlen(app->keyboard.text);
    if (length == MAX_SEARCHTEXT_LENGTH - 1 || app->keyboard.activeChar == 0) {
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

window_t keyboard_stateTarget(app_t *app, uint8_t isSelectButton) {
    return window_search;
}

void keyboard_statePersist(app_t *app) {
    memcpy(app->search.searchText, app->keyboard.text, MAX_SEARCHTEXT_LENGTH);
}

void keyboard_stateInit(app_t *app) {
    app->win = window_keyboard;
    strcpy(app->keyboard.text, app->search.searchText);
}