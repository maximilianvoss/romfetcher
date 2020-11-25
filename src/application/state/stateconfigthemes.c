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

#include "stateconfigthemes.h"
#include "../database/config.h"
#include "../ui/textures.h"
#include "../ui/fonts.h"

static void persistTheme(app_t *app);

window_t stateconfigthemes_target(app_t *app, uint8_t isSelectButton) {
    if (isSelectButton) {
        persistTheme(app);
    }
    return window_config;
}

void stateconfigthemes_persist(app_t *app) {

}

void stateconfigthemes_init(app_t *app) {
    app->win = window_config_themes;
    app->list.cursor = (linkedlist_t *) app->themes.active;
    app->list.all = (linkedlist_t *) app->themes.all;
    app->list.multi = 0;
    app->list.checkbox = 0;
    app->list.filterActive = 0;
}

static void persistTheme(app_t *app) {
    app->themes.active = (theme_t *) app->list.cursor;
    database_configPersist(app);
    textures_destroy(app);
    textures_init(app);
    fonts_destroy(app);
    fonts_init(app);
}