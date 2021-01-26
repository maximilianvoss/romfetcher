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

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "display.h"
#include "../config/config.h"
#include "../themes/rendering.h"


void sdl_init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        LOG_ERROR("Couldn't initialize SDL: %s", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() < 0) {
        LOG_ERROR("Couldn't initialize TTF: %s", TTF_GetError());
        exit(1);
    }

    if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) < 0) {
        LOG_ERROR("Couldn't initialize IMG: %s", IMG_GetError());
        exit(1);
    }
}

void display_init(app_t *app) {
    Uint32 rendererFlags = SDL_RENDERER_ACCELERATED;
    Uint32 windowFlags = 0;


    if (config_get(app->config.advanced.active, advancedConfig_openGL)) {
        windowFlags |= SDL_WINDOW_OPENGL;
    }

    if (config_get(app->config.advanced.active, advancedConfig_highDPI)) {
        windowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
    }

    int screenWidth = getResolutionConfig(app->config.resolution.active)->width;
    int screenHeight = getResolutionConfig(app->config.resolution.active)->height;
    if (config_get(app->config.advanced.active, advancedConfig_fullscreen)) {
        windowFlags |= SDL_WINDOW_FULLSCREEN;

        SDL_DisplayMode current;
        int retVal = SDL_GetCurrentDisplayMode(0, &current);
        if (retVal != 0) {
            LOG_ERROR("Could not get display mode for video display #%d: %s", 0, SDL_GetError());
        } else {
            LOG_INFO("Display #%d: current display mode is %dx%dpx @ %dhz.", 0, current.w, current.h,
                     current.refresh_rate);
            screenWidth = current.w;
            screenHeight = current.h;
        }
    } else {
        windowFlags |= SDL_WINDOW_RESIZABLE;
    }

    app->sdlWindow = SDL_CreateWindow("ROM Fetcher", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth,
                                      screenHeight, windowFlags);
    if (!app->sdlWindow) {
        LOG_ERROR("Failed to open %d x %d window: %s", screenWidth, screenHeight, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
    app->sdlRenderer = SDL_CreateRenderer(app->sdlWindow, -1, rendererFlags);
    if (!app->sdlRenderer) {
        LOG_ERROR("Failed to create renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderDrawBlendMode(app->sdlRenderer, SDL_BLENDMODE_BLEND);
}

void display_destroy(app_t *app) {
    SDL_DestroyRenderer(app->sdlRenderer);
    SDL_DestroyWindow(app->sdlWindow);
}

void display_reset(app_t *app) {
    display_destroy(app);
    display_init(app);
    themes_activate(app, app->themes.active);
}