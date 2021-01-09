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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <romfetcher.h>
#include <sqlite3.h>
#include "definitions/enums.h"
#include "definitions/themes.h"
#include "definitions/download.h"
#include "definitions/controller.h"
#include "definitions/texture.h"
#include "config/configmenu.h"
#include "constants.h"

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
        char activeChar;
        char text[MAX_SEARCHTEXT_LENGTH];
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
        configMenu_t *active;
        configMenu_t *cursor;

        struct {
            advancedConfig_t *active;
            advancedConfig_t *cursor;
        } advanced;

        struct {
            resolutionConfig_t *active;
            resolutionConfig_t *cursor;
        } resolution;
    } config;

    struct {
        result_t *cursor;
        result_t *all;
        char searchText[MAX_SEARCHTEXT_LENGTH];
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

#endif
