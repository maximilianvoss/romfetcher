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
#include <sqlite3.h>
#include <stdint.h>
#include "enums.h"
#include <romfetcher.h>

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
        SDL_Color modalBackground;
        SDL_Color modalBackgroundInactive;
        SDL_Color modalFrame;
        SDL_Color modalText;
        SDL_Color modalButtonFrame;
        SDL_Color modalButtonFrameActive;
    } colors;
    char *fileReference;
    char *font;
    struct {
        char *background;
        char *checkboxChecked;
        char *checkboxUnchecked;
        char *selectorIcon;
        char *settingsIcon;
        char *downloadManagerIcon;
    } images;
} theme_t;

typedef struct config_s {
    struct config_s *prev;
    struct config_s *next;
    char *name;
    window_t destiny;
} config_t;



typedef struct advancedconfig_s {
    struct advancedconfig_s *prev;
    struct advancedconfig_s *next;
    char *name;
    uint8_t active;
    advancedConfigSetting_t setting;
} advancedconfig_t;





typedef struct resolution_s {
    struct resolution_s *prev;
    struct resolution_s *next;
    char *name;
    uint8_t active;
    int height;
    int width;
} resolution_t;

typedef struct download_s {
    struct download_s *prev;
    struct download_s *next;
    char *title;
    uint8_t active;
    system_t *system;
    char *url;
    char *data;
    char *filename;
    httpmethod_t method;
    curl_off_t current;
    curl_off_t total;
    volatile uint8_t cancelled;
} download_t;

typedef struct {
    SDL_Renderer *sdlRenderer;
    SDL_Window *sdlWindow;
    window_t win;
    volatile uint8_t quit;

    struct {
        sqlite3 *db;
    } database;

    struct {
        linkedlist_t *active;
        linkedlist_t *all;
        linkedlist_t *cursor;
        uint8_t multi;
        uint8_t filterActive;
        uint8_t checkbox;
    } list;

    struct {
        uint8_t displayed;
        char *headline;
        char *text;
        char *actionButton;
        char *cancelButton;
        uint8_t cursorPos;
        void *app;
        void *callbackData;

        void (*callbackAction)(void *app, void *data);

        void (*callbackCancel)(void *app, void *data);
    } modal;

    struct {
        SDL_Texture *backgroundImage;
        SDL_Texture *searchChevron;
        SDL_Texture *settingsIcon;
        SDL_Texture *downloadManagerIcon;
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
    } themes;

    struct {
        system_t *active;
        system_t *all;
        system_t *cursor;
    } systems;

    struct {
        hoster_t *all;
        hoster_t *cursor;
    } engine;

    struct {
        config_t *active;
        config_t *all;
        config_t *cursor;

        struct {
            advancedconfig_t *active;
            advancedconfig_t *all;
            advancedconfig_t *cursor;
            uint8_t fullscreen;
            uint8_t opengl;
            uint8_t highdpi;
            uint8_t downloadQueue;
            uint8_t externalCurl;
        } advanced;

        struct {
            resolution_t *active;
            resolution_t *all;
            resolution_t *cursor;
        } resolution;
    } config;

    struct {
        result_t *active;
        result_t *cursor;
        result_t *all;
        char searchText[256];
        searchactivity_t position;
    } search;

    struct {
        download_t *active;
        download_t *done;
        download_t *queue;
        download_t *cursor;
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
