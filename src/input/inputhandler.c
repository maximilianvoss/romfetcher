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

#include "inputhandler.h"
#include "../config.h"
#include "inputsearch.h"
#include "inputdownload.h"
#include "inputkeyboard.h"
#include "inputlist.h"
#include "inputmodal.h"
#include "inputdownloadmanager.h"

static uint8_t processGameController(app_t *app);

static uint8_t processEvents(app_t *app);

static void (*processUp)(app_t *app);

static void (*processDown)(app_t *app);

static void (*processLeft)(app_t *app);

static void (*processRight)(app_t *app);

static void (*processSelect)(app_t *app);

static void (*processBack)(app_t *app);

static void (*processOtherButton)(app_t *app, GameControllerState_t *state);

static void (*processOtherKey)(app_t *app, SDL_Scancode scancode);

static SDL_GameController **gameControllers;
static GameControllerState_t gameControllerState;
static int gameControllerCount;
static Uint32 lastKeyPressed = 0;


void inputhandler_init() {
    if (SDL_GameControllerAddMapping(CONTROLLER_MAPPING)) {
        SDL_Log("Failed to add mapping: %s", SDL_GetError());
    }

    gameControllerCount = SDL_NumJoysticks();
    SDL_Log("There are %d gamepads attached\n", gameControllerCount);

    if (gameControllerCount > 0) {
        gameControllers = (SDL_GameController **) calloc(gameControllerCount, sizeof(SDL_GameController *));

        for (int i = 0; i < gameControllerCount; ++i) {
            gameControllers[i] = SDL_GameControllerOpen(i);
            if (gameControllers[i] == NULL) {
                SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GameControllerOpen(%d) failed: %s\n", i,
                             SDL_GetError());
            }
            char *mapping = SDL_GameControllerMapping(gameControllers[i]);
            SDL_Log("Following Mapping is used: %s\n", mapping);
            SDL_free(mapping);
        }
    }
}

uint8_t inputhandler_processInputs(app_t *app) {
    switch (app->win) {
        case window_search:
            processUp = &inputsearch_processUp;
            processDown = &inputsearch_processDown;
            processLeft = &inputsearch_processLeft;
            processRight = &inputsearch_processRight;
            processSelect = &inputsearch_processSelect;
            processBack = &inputsearch_processBack;
            processOtherButton = &inputsearch_processOtherButton;
            processOtherKey = &inputsearch_processOtherKey;
            break;
        case window_keyboard:
            processUp = &inputkeyboard_processUp;
            processDown = &inputkeyboard_processDown;
            processLeft = &inputkeyboard_processLeft;
            processRight = &inputkeyboard_processRight;
            processSelect = &inputkeyboard_processSelect;
            processBack = &inputkeyboard_processBack;
            processOtherButton = &inputkeyboard_processOtherButton;
            processOtherKey = &inputkeyboard_processOtherKey;
            break;
        case window_download:
            processUp = &inputdownload_processUp;
            processDown = &inputdownload_processDown;
            processLeft = &inputdownload_processLeft;
            processRight = &inputdownload_processRight;
            processSelect = &inputdownload_processSelect;
            processBack = &inputdownload_processBack;
            processOtherButton = &inputdownload_processOtherButton;
            processOtherKey = &inputdownload_processOtherKey;
            break;
        case window_downloadMgr:
            processUp = &inputdownloadmanager_processUp;
            processDown = &inputdownloadmanager_processDown;
            processLeft = &inputdownloadmanager_processLeft;
            processRight = &inputdownloadmanager_processRight;
            processSelect = &inputdownloadmanager_processSelect;
            processBack = &inputdownloadmanager_processBack;
            processOtherButton = &inputdownloadmanager_processOtherButton;
            processOtherKey = &inputdownloadmanager_processOtherKey;
            break;
        case window_config:
        case window_config_advanced:
        case window_config_engine:
        case window_config_systems:
        case window_config_resolution:
        case window_config_themes:
        case window_system:
            processUp = &inputlist_processUp;
            processDown = &inputlist_processDown;
            processLeft = &inputlist_processLeft;
            processRight = &inputlist_processRight;
            processSelect = &inputlist_processSelect;
            processBack = &inputlist_processBack;
            processOtherButton = &inputlist_processOtherButton;
            processOtherKey = &inputlist_processOtherKey;
            break;
    }

    if (app->modal.displayed) {
        processUp = &inputmodal_processUp;
        processDown = &inputmodal_processDown;
        processLeft = &inputmodal_processLeft;
        processRight = &inputmodal_processRight;
        processSelect = &inputmodal_processSelect;
        processBack = &inputmodal_processBack;
        processOtherButton = &inputmodal_processOtherButton;
        processOtherKey = &inputmodal_processOtherKey;
    }

    return processEvents(app);
}

void inputhandler_destroy() {
    for (int i = 0; i < gameControllerCount; i++) {
        SDL_GameControllerClose(gameControllers[i]);
    }
    free(gameControllers);
}


static uint8_t processGameController(app_t *app) {
    if (gameControllerState.start && gameControllerState.back) {
        return 1;
    } else if (gameControllerState.up) {
        processUp(app);
    } else if (gameControllerState.down) {
        processDown(app);
    } else if (gameControllerState.left) {
        processLeft(app);
    } else if (gameControllerState.right) {
        processRight(app);
    } else if (gameControllerState.buttonA) {
        processSelect(app);
    } else if (gameControllerState.buttonB) {
        processBack(app);
    } else {
        processOtherButton(app, &gameControllerState);
    }
    return 0;
}

static uint8_t processEvents(app_t *app) {
    SDL_Event event;
    SDL_Scancode scancode;

    if (SDL_TICKS_PASSED(SDL_GetTicks(), lastKeyPressed + 300L)) {
        if (gameControllerState.up || gameControllerState.down || gameControllerState.left ||
            gameControllerState.right) {
            processGameController(app);
            lastKeyPressed = SDL_GetTicks();
        }
    }

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return 1;
            case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERBUTTONDOWN:
                switch (event.cbutton.button) {
                    case SDL_CONTROLLER_BUTTON_A:
                        gameControllerState.buttonA = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_B:
                        gameControllerState.buttonB = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_X:
                        gameControllerState.buttonX = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_Y:
                        gameControllerState.buttonY = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_BACK:
                        gameControllerState.back = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_START:
                        gameControllerState.start = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
                        gameControllerState.leftStick = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
                        gameControllerState.rightStick = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
                        gameControllerState.leftShoulder = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
                        gameControllerState.rightShoulder = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_UP:
                        gameControllerState.up = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
                        gameControllerState.down = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                        gameControllerState.left = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                        gameControllerState.right = event.type == SDL_CONTROLLERBUTTONDOWN ? 1 : 0;
                        break;
                    default:
                        SDL_Log("Unassigned Button: %d\n", event.cbutton.button);
                        break;
                }
                lastKeyPressed = SDL_GetTicks();
                return processGameController(app);
            case SDL_JOYBUTTONUP:
                break;
            case SDL_KEYDOWN:
                scancode = event.key.keysym.scancode;

                switch (scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        return 1;
                    case SDL_SCANCODE_UP:
                        processUp(app);
                        break;
                    case SDL_SCANCODE_DOWN:
                        processDown(app);
                        break;
                    case SDL_SCANCODE_LEFT:
                        processLeft(app);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        processRight(app);
                        break;
                    case SDL_SCANCODE_RETURN:
                        processSelect(app);
                        break;
                    case SDL_SCANCODE_BACKSPACE:
                        processBack(app);
                        break;
                    default:
                        processOtherKey(app, scancode);
                        break;
                }
            default:
                break;
        }
    }
    return 0;
}