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

#include "stateconfigresolution.h"
#include "../ui/core.h"
#include "stateconfig.h"
#include "../database/config.h"


static void modalApprove(void *app, void *data) {
    database_configPersist(app);
    stateconfig_init(app);
}

static void modalCancel(void *app, void *data) {
    database_configLoad(app);
    ui_destroy(app);
    ui_init(app);
    stateconfigresolution_init(app);
}

window_t stateconfigresolution_target(app_t *app, uint8_t isSelectButton) {
    app->config.resolution.active = (resolution_t *) app->list.active;

    ui_destroy(app);
    ui_init(app);

    app->modal.displayed = 1;
    app->modal.headline = "Save Config Settings";
    app->modal.text = "As this configurations impact the rendering,\ndo you want to save these new settings?";
    app->modal.actionButton = "Yes";
    app->modal.cancelButton = "No";
    app->modal.cursorPos = 1;
    app->modal.callbackData = app;
    app->modal.callbackAction = &modalApprove;
    app->modal.callbackCancel = &modalCancel;

    return window_config_advanced;
}

void stateconfigresolution_persist(app_t *app) {}

void stateconfigresolution_init(app_t *app) {
    app->win = window_config_resolution;
    app->list.active = (linkedlist_t *) app->config.resolution.active;
    app->list.all = (linkedlist_t *) app->config.resolution.all;
    app->list.cursor = (linkedlist_t *) app->config.resolution.active;
    app->list.multi = 0;
    app->list.checkbox = 0;
    app->list.filterActive = 0;
}
