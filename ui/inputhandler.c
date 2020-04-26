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
#include "../structs.h"
#include "../engine/results.h"
#include "inputsearch.h"
#include "inputsystemselect.h"
#include "inputdownload.h"
#include "inputkeyboard.h"

static uint8_t processGameController(app_t *app);

static uint8_t processEvents(app_t *app);

static void (*processUp)(app_t *app);

static void (*processDown)(app_t *app);

static void (*processLeft)(app_t *app);

static void (*processRight)(app_t *app);

static void (*processSelect)(app_t *app);

static void (*processBack)(app_t *app);

static void (*processOtherButton)(app_t *app, GameControllerState_t *state);

static SDL_GameController **gameControllers;
static GameControllerState_t gameControllerState;
static int gameControllerCount;


void initGameController() {
    if ( SDL_GameControllerAddMapping(CONTROLLER_MAPPING) ) {
        printf("Failed to open add mapping: %s", SDL_GetError());
        exit(1);
    }

    gameControllerCount = SDL_NumJoysticks();
    SDL_Log("There are %d gamepads attached\n", gameControllerCount);

    if (gameControllerCount > 0) {
        gameControllers = (SDL_GameController **) malloc(sizeof(SDL_GameController *) * gameControllerCount);

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

uint8_t processInputs(app_t *app) {
    switch (app->win) {
        case search:
            processUp = &search_processUp;
            processDown = &search_processDown;
            processLeft = &search_processLeft;
            processRight = &search_processRight;
            processSelect = &search_processSelect;
            processBack = &search_processBack;
            processOtherButton = &search_processOtherButton;
            break;
        case systemselect:
            processUp = &systemselect_processUp;
            processDown = &systemselect_processDown;
            processLeft = &systemselect_processLeft;
            processRight = &systemselect_processRight;
            processSelect = &systemselect_processSelect;
            processBack = &systemselect_processBack;
            processOtherButton = &systemselect_processOtherButton;
            break;
        case keyboard:
            processUp = &keyboard_processUp;
            processDown = &keyboard_processDown;
            processLeft = &keyboard_processLeft;
            processRight = &keyboard_processRight;
            processSelect = &keyboard_processSelect;
            processBack = &keyboard_processBack;
            processOtherButton = &keyboard_processOtherButton;
            break;
        case download:
            processUp = &download_processUp;
            processDown = &download_processDown;
            processLeft = &download_processLeft;
            processRight = &download_processRight;
            processSelect = &download_processSelect;
            processBack = &download_processBack;
            processOtherButton = &download_processOtherButton;
            break;
    }
    return processEvents(app);
}

void freeGameController() {
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
                        SDL_Log("Unused Button: %d\n", event.cbutton.button);
                        break;
                }
                return processGameController(app);
            case SDL_JOYBUTTONUP:
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode) {
                    case 20:
                        return 1;
                    case 82: //up
                        processUp(app);
                        break;
                    case 81: //down
                        processDown(app);
                        break;
                    case 80: // left
                        processLeft(app);
                        break;
                    case 79: // right
                        processRight(app);
                        break;
                    case 40: // enter
                    case 44: // space
                        processSelect(app);
                        break;
                    case 42: // backspace
                    case 41: // escape
                        processBack(app);
                        break;
                    default:
                        SDL_Log("Keycode %d Physical %s key acting as %s key",
                                event.key.keysym.scancode,
                                SDL_GetScancodeName(event.key.keysym.scancode),
                                SDL_GetKeyName(event.key.keysym.sym));
                        break;
                }
            default:
                break;
        }
    }
    return 0;
}