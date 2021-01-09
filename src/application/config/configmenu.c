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

#include "configmenu.h"
#include <romfetcher.h>

static configMenu_t *createConfigMenuEntry(char *name, window_t destiny);

static advancedConfig_t *createAdvancedConfigEntry(char *name, int active, advancedConfigSetting_t setting);

static resolutionConfig_t *createResolutionConfigEntry(char *name, int width, int height);

configMenu_t *configMenu_init() {
    configMenu_t *configmenu = NULL;
    configmenu = ll_append(configmenu, createConfigMenuEntry("Enable/Disable Hosters", window_config_hoster));
    configmenu = ll_append(configmenu, createConfigMenuEntry("Enable/Disable Systems", window_config_systems));
    configmenu = ll_append(configmenu, createConfigMenuEntry("Select Theme", window_config_themes));
    configmenu = ll_append(configmenu, createConfigMenuEntry("Change Window Resolution", window_config_resolution));
    configmenu = ll_append(configmenu, createConfigMenuEntry("Advanced Configurations", window_config_advanced));
    return configmenu;
}

advancedConfig_t *advancedConfig_init() {
    advancedConfig_t *advancedconfigmenu = NULL;
    advancedconfigmenu = ll_append(advancedconfigmenu,
                                   createAdvancedConfigEntry("Fullscreen enabled", 1, advancedConfig_fullscreen));
    advancedconfigmenu = ll_append(advancedconfigmenu,
                                   createAdvancedConfigEntry("OpenGL rendering enabled", 0, advancedConfig_openGL));
    advancedconfigmenu = ll_append(advancedconfigmenu,
                                   createAdvancedConfigEntry("High DPI rendering enabled", 0,
                                                             advancedConfig_highDPI));
    advancedconfigmenu = ll_append(advancedconfigmenu,
                                   createAdvancedConfigEntry("Save download queue on exit", 1,
                                                             advancedConfig_downloadQueue));
    advancedconfigmenu = ll_append(advancedconfigmenu,
                                   createAdvancedConfigEntry("Download with external CURL", 0,
                                                             advancedConfig_externalCurl));
    return advancedconfigmenu;
}

resolutionConfig_t *resolutionConfig_init() {
    resolutionConfig_t *resolutionconfigmenu = NULL;
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("640x480", 640, 480));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("800x600", 800, 600));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("960x720", 960, 720));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1024x576", 1024, 576));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1024x768", 1024, 768));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1152x648", 1152, 648));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1280x720", 1280, 720));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1280x800", 1280, 800));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1280x960", 1280, 960));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1366x768", 1366, 768));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1440x900", 1440, 900));
    resolutionconfigmenu = ll_append(resolutionconfigmenu, createResolutionConfigEntry("1400x1050", 1400, 1050));
    return resolutionconfigmenu;
}

void configMenu_destroy(configMenu_t *configMenu) {
    configMenu = ll_get1st(configMenu);
    linkedlist_freeList(configMenu, NULL);
}


void advancedConfig_destroy(advancedConfig_t *advancedConfig) {
    advancedConfig = ll_get1st(advancedConfig);
    linkedlist_freeList(advancedConfig, NULL);
}

void resolutionConfig_destroy(resolutionConfig_t *resolutionConfig) {
    resolutionConfig = ll_get1st(resolutionConfig);
    linkedlist_freeList(resolutionConfig, NULL);
}

static configMenu_t *createConfigMenuEntry(char *name, window_t destiny) {
    configMenu_t *config = (configMenu_t *) calloc(1, sizeof(configMenu_t));
    config->name = name;
    config->destiny = destiny;
    return config;
}

static advancedConfig_t *createAdvancedConfigEntry(char *name, int active, advancedConfigSetting_t setting) {
    advancedConfig_t *config = (advancedConfig_t *) calloc(1, sizeof(advancedConfig_t));
    config->name = name;
    config->active = active;
    config->setting = setting;
    return config;
}

static resolutionConfig_t *createResolutionConfigEntry(char *name, int width, int height) {
    resolutionConfig_t *resolution = (resolutionConfig_t *) calloc(1, sizeof(resolutionConfig_t));
    resolution->name = name;
    resolution->width = width;
    resolution->height = height;
    return resolution;
}