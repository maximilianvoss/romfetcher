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

#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <curl/curl.h>

typedef enum {
    search,
    systemselect,
    keyboard,
    download
} window_t;

typedef enum {
    romsmania
} engine_t;

typedef enum {
    searchsystem,
    searchfield,
    searchbutton,
    searchresults
} searchactivity_t;

typedef enum {
    posCancel,
    posDownload,
    posDone
} downloadactivity_t;

typedef struct system_s {
    char *name;
    char *fullname;
    char *path;
    struct system_s *prev;
    struct system_s *next;
} system_t;

typedef struct searchresult_s {
    char *title;
    char *url;
    system_t *system;
    struct searchresult_s *prev;
    struct searchresult_s *next;
} searchresult_t;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    struct {
        SDL_Texture *backgroundImage;
        SDL_Texture *searchChevron;
    } textures;
    struct {
        TTF_Font *huge;
        TTF_Font *big;
        TTF_Font *medium;
        TTF_Font *small;
    } fonts;
    struct {
        char activeChar;
        char text[256];
        int8_t pointerPosition;
    } keyboard;
    struct {
        volatile uint8_t started;
        volatile uint8_t complete;
        downloadactivity_t cursorPos;
        volatile curl_off_t current;
        volatile curl_off_t total;
    } download;
    struct {
        engine_t engine;
        system_t *systemActive;
        system_t *systemHovered;
        searchresult_t *results;
        searchresult_t *resultHovered;
        searchresult_t *resultActive;
        searchactivity_t position;
        char searchText[256];
    } search;
    window_t win;
} app_t;

typedef struct {
    uint8_t up;
    uint8_t down;
    uint8_t left;
    uint8_t right;
    uint8_t start;
    uint8_t back;
    uint8_t leftShoulder;
    uint8_t rightShoulder;
    uint8_t leftStick;
    uint8_t rightStick;
    uint8_t buttonA;
    uint8_t buttonB;
    uint8_t buttonX;
    uint8_t buttonY;

    int16_t stickX;
    int16_t stickY;
} GameControllerState_t;

typedef struct {
    SDL_Texture *texture;
    int w;
    int h;
} texture_t;

#endif
