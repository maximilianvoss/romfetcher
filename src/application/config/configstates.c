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

#include "configstates.h"
#include "../ui/display.h"
#include "config.h"
#include "../list/dblist.h"
#include "../constants.h"
#include "../themes/rendering.h"

static void modalAdvancedApprove(void *appPtr, void *data);

static void modalAdvancedCancel(void *appPtr, void *data);

static void persistHoster(app_t *app);

static void modalResolutionApprove(void *appPtr, void *data);

static void modalResolutionCancel(void *appPtr, void *data);

static void persistSystems(app_t *app);

static void persistTheme(app_t *app);

// config
window_t config_stateTarget(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return ((configMenu_t *) app->list.cursor)->destiny;
    }
    return window_search;
}

void config_statePersist(app_t *app) {

}

void config_stateInit(app_t *app) {
    app->win = window_config;
    app->list.active = (linkedlist_t *) app->config.active;
    app->list.all = (linkedlist_t *) ll_get1st(app->config.active);
    app->list.cursor = (linkedlist_t *) app->config.active;
    app->list.multi = 0;
    app->list.checkbox = 0;
    app->list.filterActive = 0;
}


// advanced configs
static void modalAdvancedApprove(void *appPtr, void *data) {
    app_t *app = appPtr;
    app->modal.displayed = 0;
    config_persist(app->database.db, app->config.advanced.active, app->config.resolution.cursor, app->themes.active,
                   app->engine.all, app->systems.all);
    config_stateInit((app_t *) app);
}

static void modalAdvancedCancel(void *appPtr, void *data) {
    app_t *app = appPtr;
    app->modal.displayed = 0;
    config_revert(app->database.db, &app->config.advanced.active, &app->config.resolution.active, &app->themes.active);
    display_reset(app);
    advancedConfig_stateInit(app);
}

window_t advancedConfig_stateTarget(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return window_config_advanced;
    }

    display_reset(app);

    app->modal.displayed = 1;
    app->modal.headline = "Save Config Settings";
    app->modal.text = "As this configurations impact the rendering,\ndo you want to save these new settings?";
    app->modal.actionButton = "Yes";
    app->modal.cancelButton = "No";
    app->modal.cursorPos = 1;
    app->modal.app = app;
    app->modal.callbackData = NULL;
    app->modal.callbackAction = &modalAdvancedApprove;
    app->modal.callbackCancel = &modalAdvancedCancel;

    return window_config_advanced;
}

void advancedConfig_statePersist(app_t *app) {
}

void advancedConfig_stateInit(app_t *app) {
    if (app->win != window_config_advanced) {
        app->win = window_config_advanced;

        app->list.cursor = (linkedlist_t *) app->config.advanced.cursor;
        app->list.all = (linkedlist_t *) ll_get1st(app->config.advanced.active);
        app->list.multi = 1;
        app->list.checkbox = 1;
        app->list.filterActive = 0;
    }
}

// hosters
window_t hosterConfig_stateTarget(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return window_config_hoster;
    }
    persistHoster(app);
    return window_config;
}

void hosterConfig_statePersist(app_t *app) {

}

void hosterConfig_stateInit(app_t *app) {
    if (app->win != window_config_hoster) {
        app->win = window_config_hoster;
        app->list.cursor = (linkedlist_t *) app->engine.all;
        app->list.all = (linkedlist_t *) app->engine.all;
        app->list.multi = 1;
        app->list.checkbox = 1;
        app->list.filterActive = 0;
    }
}

static void persistHoster(app_t *app) {
    dbll_persist(app->database.db, DATABASE_TABLE_HOSTER, (linkedlist_t *) app->engine.all);
}

// Resolution
static void modalResolutionApprove(void *appPtr, void *data) {
    app_t *app = appPtr;
    app->modal.displayed = 0;
    config_persist(app->database.db, app->config.advanced.active, app->config.resolution.cursor, app->themes.active,
                   app->engine.all, app->systems.all);
    config_stateInit(app);
}

static void modalResolutionCancel(void *appPtr, void *data) {
    app_t *app = appPtr;
    app->modal.displayed = 0;
    config_revert(app->database.db, &app->config.advanced.active, &app->config.resolution.active, &app->themes.active);
    display_reset(app);
    resolutioinConfig_stateInit(app);
}

window_t resolutionConfig_stateTarget(app_t *app, uint8_t isSelectButton) {
    app->config.resolution.active = (resolutionConfig_t *) app->list.active;

    display_reset(app);

    app->modal.displayed = 1;
    app->modal.headline = "Save Config Settings";
    app->modal.text = "As this configurations impact the rendering,\ndo you want to save these new settings?";
    app->modal.actionButton = "Yes";
    app->modal.cancelButton = "No";
    app->modal.cursorPos = 1;
    app->modal.callbackData = app;
    app->modal.callbackAction = &modalResolutionApprove;
    app->modal.callbackCancel = &modalResolutionCancel;

    return window_config_advanced;
}

void resolutionConfig_statePersist(app_t *app) {}

void resolutioinConfig_stateInit(app_t *app) {
    app->win = window_config_resolution;
    app->list.active = (linkedlist_t *) app->config.resolution.active;
    app->list.all = (linkedlist_t *) ll_get1st(app->config.resolution.active);
    app->list.cursor = (linkedlist_t *) app->config.resolution.active;
    app->list.multi = 0;
    app->list.checkbox = 0;
    app->list.filterActive = 0;
}

// Systems
window_t systemConfig_stateTarget(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        return window_config_systems;
    }
    persistSystems(app);
    return window_config;
}

void systemConfig_statePersist(app_t *app) {

}

void systemConfig_stateInit(app_t *app) {
    if (app->win != window_config_systems) {
        app->win = window_config_systems;
        app->list.cursor = (linkedlist_t *) app->systems.all;
        app->list.all = (linkedlist_t *) app->systems.all;
        app->list.multi = 1;
        app->list.checkbox = 1;
        app->list.filterActive = 0;
    }
}

static void persistSystems(app_t *app) {
    dbll_persist(app->database.db, DATABASE_TABLE_SYSTEMS, (linkedlist_t *) app->systems.all);
    app->systems.active = linkedlist_getFirstActive(app->systems.all);
}

// Themes
window_t themeConfig_stateTarget(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        persistTheme(app);
    }
    return window_config;
}

void themeConfig_statePersist(app_t *app) {

}

void themeConfig_stateInit(app_t *app) {
    app->win = window_config_themes;
    app->list.cursor = (linkedlist_t *) app->themes.active;
    app->list.all = (linkedlist_t *) app->themes.all;
    app->list.multi = 0;
    app->list.checkbox = 0;
    app->list.filterActive = 0;
}

static void persistTheme(app_t *app) {
    themes_activate(app, (theme_t *) app->list.cursor);
    config_persist(app->database.db, app->config.advanced.active, app->config.resolution.active, app->themes.active,
                   app->engine.all, app->systems.all);
}