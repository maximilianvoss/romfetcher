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

#include "stateconfigadvanced.h"
#include "../database/config.h"
#include "../ui/core.h"
#include "stateconfig.h"
#include "../config/advanced.h"

static void modalApprove(void *app, void *data) {
    database_configPersist((app_t *) app);
    stateconfig_init((app_t *) app);
}

static void modalCancel(void *app, void *data) {
    database_configLoad((app_t *) app);
    ui_destroy((app_t *) app);
    ui_init((app_t *) app);
    stateconfigadvanced_init((app_t *) app);
}

window_t stateconfigadvanced_target(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return window_config_advanced;
    }

    configadvanced_listToSettings(app);
    ui_destroy(app);
    ui_init(app);

    app->modal.displayed = 1;
    app->modal.headline = "Save Config Settings";
    app->modal.text = "As this configurations impact the rendering,\ndo you want to save these new settings?";
    app->modal.actionButton = "Yes";
    app->modal.cancelButton = "No";
    app->modal.cursorPos = 1;
    app->modal.app = app;
    app->modal.callbackData = NULL;
    app->modal.callbackAction = &modalApprove;
    app->modal.callbackCancel = &modalCancel;

    return window_config_advanced;
}

void stateconfigadvanced_persist(app_t *app) {
}

void stateconfigadvanced_init(app_t *app) {
    if (app->win != window_config_advanced) {
        app->win = window_config_advanced;

        app->list.cursor = (linkedlist_t *) app->config.advanced.all;
        app->list.all = (linkedlist_t *) app->config.advanced.all;
        app->list.multi = 1;
        app->list.checkbox = 1;
        app->list.filterActive = 0;
    }
}
