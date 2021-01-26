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

#include "input.h"
#include "../search/search.h"
#include "keyboard.h"
#include "../list/list.h"
#include "../modal/modal.h"
#include "../download/downloadmanager.h"
#include "../download/downloader/downloader.h"

static void processGameController(app_t *app);

static void processEvents(app_t *app);

static void questionAppQuitting(app_t *app);

static void modalQuit(void *app, void *data);

static void (*processUp)(app_t *app);

static void (*processDown)(app_t *app);

static void (*processLeft)(app_t *app);

static void (*processRight)(app_t *app);

static void (*processSelect)(app_t *app);

static void (*processBack)(app_t *app);

static void (*processOtherButton)(app_t *app, controller_t *state);

static void (*processOtherKey)(app_t *app, SDL_Scancode scancode);

static SDL_GameController **gameControllers;
static controller_t gameControllerState;
static int gameControllerCount;
static Uint32 lastKeyPressed = 0;

void input_init() {
    if (SDL_GameControllerAddMapping(CONTROLLER_MAPPING)) {
        LOG_ERROR("Failed to add mapping: %s", SDL_GetError());
    }

    gameControllerCount = SDL_NumJoysticks();
    LOG_INFO("There are %d gamepads attached", gameControllerCount);

    if (gameControllerCount > 0) {
        gameControllers = (SDL_GameController **) calloc(gameControllerCount, sizeof(SDL_GameController *));

        for (int i = 0; i < gameControllerCount; ++i) {
            gameControllers[i] = SDL_GameControllerOpen(i);
            if (gameControllers[i] == NULL) {
                LOG_ERROR("SDL_GameControllerOpen(%d) failed: %s", i, SDL_GetError());
            }
            char *mapping = SDL_GameControllerMapping(gameControllers[i]);
            LOG_INFO("Following Mapping is used: %s", mapping);
            SDL_free(mapping);
        }
    }
}

void input_processInputs(app_t *app) {
    switch (app->win) {
        case window_search:
            processUp = &search_processUp;
            processDown = &search_processDown;
            processLeft = &search_processLeft;
            processRight = &search_processRight;
            processSelect = &search_processSelect;
            processBack = &search_processBack;
            processOtherButton = &search_processOtherButton;
            processOtherKey = &search_processOtherKey;
            break;
        case window_keyboard:
            processUp = &keyboard_processUp;
            processDown = &keyboard_processDown;
            processLeft = &keyboard_processLeft;
            processRight = &keyboard_processRight;
            processSelect = &keyboard_processSelect;
            processBack = &keyboard_processBack;
            processOtherButton = &keyboard_processOtherButton;
            processOtherKey = &keyboard_processOtherKey;
            break;
        case window_downloadMgr:
            processUp = &downloadmanager_processUp;
            processDown = &downloadmanager_processDown;
            processLeft = &downloadmanager_processLeft;
            processRight = &downloadmanager_processRight;
            processSelect = &downloadmanager_processSelect;
            processBack = &downloadmanager_processBack;
            processOtherButton = &downloadmanager_processOtherButton;
            processOtherKey = &downloadmanager_processOtherKey;
            break;
        case window_config:
        case window_config_advanced:
        case window_config_hoster:
        case window_config_systems:
        case window_config_resolution:
        case window_config_themes:
        case window_system:
            processUp = &ll_processUp;
            processDown = &ll_processDown;
            processLeft = &ll_processLeft;
            processRight = &ll_processRight;
            processSelect = &ll_processSelect;
            processBack = &ll_processBack;
            processOtherButton = &ll_processOtherButton;
            processOtherKey = &ll_processOtherKey;
            break;
    }

    if (app->modal.displayed) {
        processUp = &modal_processUp;
        processDown = &modal_processDown;
        processLeft = &modal_processLeft;
        processRight = &modal_processRight;
        processSelect = &modal_processSelect;
        processBack = &modal_processBack;
        processOtherButton = &modal_processOtherButton;
        processOtherKey = &modal_processOtherKey;
    }

    processEvents(app);
}

void input_destroy() {
    for (int i = 0; i < gameControllerCount; i++) {
        SDL_GameControllerClose(gameControllers[i]);
    }
    free(gameControllers);
}


static void processGameController(app_t *app) {
    if (gameControllerState.start && gameControllerState.back) {
        questionAppQuitting(app);
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
}

static void processEvents(app_t *app) {
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
                questionAppQuitting(app);
                break;
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
                        LOG_DEBUG("Unassigned Button: %d", event.cbutton.button);
                        break;
                }
                lastKeyPressed = SDL_GetTicks();
                processGameController(app);
                break;
            case SDL_JOYBUTTONUP:
                break;
            case SDL_KEYDOWN:
                scancode = event.key.keysym.scancode;

                switch (scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        questionAppQuitting(app);
                        break;
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
}

static void questionAppQuitting(app_t *app) {
    app->modal.displayed = 1;
    app->modal.headline = "Quit Rom Fetcher";
    if (downloader_isActive(app)) {
        app->modal.text = "Are you sure you want to quit Rom Fetcher?\nThere are still some downloads running\nwhich will be cancelled";
    } else {
        app->modal.text = "Are you sure you want to quit Rom Fetcher?";
    }
    app->modal.actionButton = "Yes";
    app->modal.cancelButton = "No";
    app->modal.cursorPos = 0;
    app->modal.app = app;
    app->modal.callbackData = NULL;
    app->modal.callbackAction = &modalQuit;
    app->modal.callbackCancel = NULL;
}

static void modalQuit(void *app, void *data) {
    ((app_t *) app)->modal.displayed = 0;
    ((app_t *) app)->quit = 1;
}