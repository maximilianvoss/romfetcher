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

static configMenu_t *createConfigMenuEntry(char *name, window_t destiny);

static advancedConfig_t *createAdvancedConfigEntry(char *name, int active, advancedConfigSetting_t setting);

static resolutionConfig_t *createResolutionConfigEntry(char *name, int width, int height);

acll_t *configMenu_init() {
    acll_t *configmenu = NULL;
    configmenu = acll_append(configmenu, createConfigMenuEntry("Enable/Disable Hosters", window_config_hoster));
    configmenu = acll_append(configmenu, createConfigMenuEntry("Enable/Disable Systems", window_config_systems));
    configmenu = acll_append(configmenu, createConfigMenuEntry("Select Theme", window_config_themes));
    configmenu = acll_append(configmenu, createConfigMenuEntry("Change Window Resolution", window_config_resolution));
    configmenu = acll_append(configmenu, createConfigMenuEntry("Advanced Configurations", window_config_advanced));
    return configmenu;
}

acll_t *advancedConfig_init() {
    acll_t *advancedconfigmenu = NULL;
    advancedconfigmenu = acll_append(advancedconfigmenu,
                                     createAdvancedConfigEntry("Fullscreen enabled", 1, advancedConfig_fullscreen));
    advancedconfigmenu = acll_append(advancedconfigmenu,
                                     createAdvancedConfigEntry("OpenGL rendering enabled", 0, advancedConfig_openGL));
    advancedconfigmenu = acll_append(advancedconfigmenu,
                                     createAdvancedConfigEntry("High DPI rendering enabled", 0,
                                                               advancedConfig_highDPI));
    advancedconfigmenu = acll_append(advancedconfigmenu,
                                     createAdvancedConfigEntry("Save download queue on exit", 1,
                                                               advancedConfig_downloadQueue));
    advancedconfigmenu = acll_append(advancedconfigmenu,
                                     createAdvancedConfigEntry("Download with external CURL", 0,
                                                               advancedConfig_externalCurl));
    return advancedconfigmenu;
}

acll_t *resolutionConfig_init() {
    acll_t *resolutionconfigmenu = NULL;
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("640x480", 640, 480));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("800x600", 800, 600));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("960x720", 960, 720));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1024x576", 1024, 576));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1024x768", 1024, 768));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1152x648", 1152, 648));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1280x720", 1280, 720));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1280x800", 1280, 800));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1280x960", 1280, 960));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1366x768", 1366, 768));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1440x900", 1440, 900));
    resolutionconfigmenu = acll_append(resolutionconfigmenu, createResolutionConfigEntry("1400x1050", 1400, 1050));
    return resolutionconfigmenu;
}

void configMenu_destroy(acll_t *configMenu) {
    acll_free(configMenu, NULL);
}


void advancedConfig_destroy(acll_t *advancedConfig) {
    acll_free(advancedConfig, NULL);
}

void resolutionConfig_destroy(acll_t *resolutionConfig) {
    acll_free(resolutionConfig, NULL);
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