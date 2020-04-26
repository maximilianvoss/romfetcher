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

#include "core.h"
#include "../config.h"
#include "inputhandler.h"
#include "textures.h"
#include "fonts.h"

void initSDL(app_t *app) {

    int screenWidth;
    int screenHeight;
    Uint32 windowFlags = 0; //SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_OPENGL;
    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

#ifndef START_IN_WINDOW
    windowFlags |= SDL_WINDOW_FULLSCREEN;

    SDL_DisplayMode current;
    int retVal = SDL_GetCurrentDisplayMode(0, &current);
    if (retVal != 0) {
        SDL_Log("Could not get display mode for video display #%d: %s", 0, SDL_GetError());
        screenWidth = INIT_SCREEN_WIDTH;
        screenHeight = INIT_SCREEN_HEIGHT;
    } else {
        SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", 0, current.w, current.h, current.refresh_rate);
        screenWidth = current.w;
        screenHeight = current.h;
    }
#else
    windowFlags |= SDL_WINDOW_RESIZABLE;
    screenWidth = INIT_SCREEN_WIDTH;
    screenHeight = INIT_SCREEN_HEIGHT;
#endif

    app->window = SDL_CreateWindow("ROM Fetcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth,
                                   screenHeight, windowFlags);
    if (!app->window) {
        printf("Failed to open %d x %d window: %s\n", screenWidth, screenHeight, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    app->renderer = SDL_CreateRenderer(app->window, -1, rendererFlags);
    if (!app->renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderDrawBlendMode(app->renderer, SDL_BLENDMODE_BLEND);

    initTextures(app);
    initFonts(app);
    initGameController();
}

void cleanUp(app_t *app) {
    freeGameController();
    destroyTextures(app);
    destroyFonts(app);
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();
}