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

#include "enums.h"
#include "helper/linkedlist.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <curl/curl.h>
#include <sqlite3.h>

typedef struct theme_s {
    struct theme_s *prev;
    struct theme_s *next;
    char *name;
    struct {
        SDL_Color background;
        SDL_Color field;
        SDL_Color fieldBackground;
        SDL_Color fieldHighlight;
        SDL_Color text;
        SDL_Color textHighlight;
        SDL_Color textInverted;
    } colors;
    char *fileReference;
    char *font;
    struct {
        char *background;
        char *checkboxChecked;
        char *checkboxUnchecked;
        char *selectorIcon;
        char *settingsIcon;
    } images;
} theme_t;

typedef struct system_s {
    struct system_s *prev;
    struct system_s *next;
    char *fullname;
    int active;
    char *name;
    char *path;
} system_t;

typedef struct searchresult_s {
    struct searchresult_s *prev;
    struct searchresult_s *next;
    char *title;
    char *url;
    system_t *system;
} searchresult_t;

typedef struct engine_s {
    struct engine_s *prev;
    struct engine_s *next;
    char *title;

    searchresult_t *(*search)(void *app, system_t *system, char *searchString);

    void (*download)(void *app, searchresult_t *item, void (*callback)(void *app));
} engine_t;

typedef struct {
    SDL_Renderer *sdlRenderer;
    SDL_Window *sdlWindow;
    window_t win;

    struct {
        sqlite3 *db;
    } database;

    struct {
        linkedlist_t *active;
        linkedlist_t *all;
        linkedlist_t *cursor;
        uint8_t multi;
        uint8_t checkbox;
        window_t destiny;
    } list;

    struct {
        SDL_Texture *backgroundImage;
        SDL_Texture *searchChevron;
        SDL_Texture *settingsIcon;
        SDL_Texture *checkboxChecked;
        SDL_Texture *checkboxUnchecked;
    } textures;

    struct {
        TTF_Font *small;
        TTF_Font *medium;
        TTF_Font *big;
        TTF_Font *huge;
    } fonts;

    struct {
        char activeChar;
        char text[256];
        int8_t pointerPosition;
    } keyboard;

    struct {
        theme_t *active;
        theme_t *all;
        theme_t *cursor;
        theme_t *enabled;
    } themes;

    struct {
        system_t *active;
        system_t *all;
        system_t *cursor;
        system_t *enabled;
    } systems;

    struct {
        engine_t *active;
        engine_t *all;
        engine_t *cursor;
    } engine;

    struct {
        config_t cursor;
    } config;

    struct {
        searchresult_t *active;
        searchresult_t *cursor;
        searchresult_t *all;
        char searchText[256];
        searchactivity_t position;
    } search;

    struct {
        volatile uint8_t started;
        volatile uint8_t complete;
        volatile curl_off_t current;
        volatile curl_off_t total;
        downloadactivity_t cursorPos;
    } download;
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
