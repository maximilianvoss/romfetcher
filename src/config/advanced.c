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

#include "advanced.h"

static advancedconfig_t *createAdvancedConfig(char *name, int active, advancedConfigSetting_t setting);

void configadvanced_init(app_t *app) {
    app->config.advanced.all = linkedlist_appendElement(app->config.advanced.all,
                                                        createAdvancedConfig("Fullscreen enabled", 1,
                                                                             advancedConfig_fullscreen));
    app->config.advanced.all = linkedlist_appendElement(app->config.advanced.all,
                                                        createAdvancedConfig("OpenGL rendering enabled", 0,
                                                                             advancedConfig_openGL));
    app->config.advanced.all = linkedlist_appendElement(app->config.advanced.all,
                                                        createAdvancedConfig("High DPI rendering enabled", 0,
                                                                             advancedConfig_highDPI));
    app->config.advanced.all = linkedlist_appendElement(app->config.advanced.all,
                                                        createAdvancedConfig("Save download queue on exit", 1,
                                                                             advancedConfig_downloadQueue));

    app->config.advanced.active = app->config.advanced.all;
    app->config.advanced.cursor = app->config.advanced.all;
}

void configadvanced_destroy(app_t *app) {
    linkedlist_freeList(app->config.advanced.all, NULL);
}

void configadvanced_setConfig(app_t *app, advancedConfigSetting_t setting, uint8_t value) {
    switch (setting) {
        case advancedConfig_fullscreen:
            app->config.advanced.fullscreen = value;
        case advancedConfig_openGL:
            app->config.advanced.opengl = value;
        case advancedConfig_highDPI:
            app->config.advanced.highdpi = value;
            break;
        case advancedConfig_downloadQueue:
            app->config.advanced.downloadQueue = value;
            break;
    }
    advancedconfig_t *ptr = app->config.advanced.all;
    while (ptr != NULL) {
        if (ptr->setting == setting) {
            ptr->active = value;
            return;
        }
        ptr = ptr->next;
    }
}

void configadvanced_listToSettings(app_t *app) {
    advancedconfig_t *ptr = app->config.advanced.all;
    while (ptr != NULL) {
        uint8_t value = ptr->active;
        switch (ptr->setting) {
            case advancedConfig_fullscreen:
                app->config.advanced.fullscreen = value;
            case advancedConfig_openGL:
                app->config.advanced.opengl = value;
            case advancedConfig_highDPI:
                app->config.advanced.highdpi = value;
                break;
            case advancedConfig_downloadQueue:
                app->config.advanced.downloadQueue = value;
                break;
        }
        ptr = ptr->next;
    }
}

static advancedconfig_t *createAdvancedConfig(char *name, int active, advancedConfigSetting_t setting) {
    advancedconfig_t *config = (advancedconfig_t *) calloc(1, sizeof(advancedconfig_t));
    config->name = name;
    config->active = active;
    config->setting = setting;
    return config;
}