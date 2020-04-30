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

#include "inputconfig.h"
#include "../database/config.h"

static void moveToNext(app_t *app);

static void moveToPrev(app_t *app);

void inputconfigengine_processUp(app_t *app) {
    moveToPrev(app);
}

void inputconfigengine_processDown(app_t *app) {
    moveToNext(app);
}

void inputconfigengine_processLeft(app_t *app) {
    moveToPrev(app);
}

void inputconfigengine_processRight(app_t *app) {
    moveToNext(app);
}

void inputconfigengine_processSelect(app_t *app) {
    app->search.engine = app->config.engineCursor;
    database_configPersist(app);
    app->config.engineCursor = engine_notdefined;
    app->win = window_config;
}

void inputconfigengine_processBack(app_t *app) {
    app->win = window_config;
}

void inputconfigengine_processOtherButton(app_t *app, GameControllerState_t *state) {}

static void moveToNext(app_t *app) {
    if (app->config.engineCursor < 5) {
        app->config.engineCursor++;
    }
}

static void moveToPrev(app_t *app) {
    if (app->config.engineCursor > 1) {
        app->config.engineCursor--;
    }
}