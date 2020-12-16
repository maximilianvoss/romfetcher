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

#include "init.h"
#include "advanced.h"

static config_t *createConfig(char *name, window_t destiny);

static void configresolution_init(app_t *app);

static resolution_t *createResolution(char *name, int width, int height);

static void configresolution_destroy(app_t *app);

void config_init(app_t *app) {
    app->config.all = linkedlist_appendElement(app->config.all,
                                               createConfig("Enable/Disable Hosters", window_config_hoster));
    app->config.all = linkedlist_appendElement(app->config.all,
                                               createConfig("Enable/Disable Systems", window_config_systems));
    app->config.all = linkedlist_appendElement(app->config.all, createConfig("Select Theme", window_config_themes));
    app->config.all = linkedlist_appendElement(app->config.all,
                                               createConfig("Change Window Resolution", window_config_resolution));
    app->config.all = linkedlist_appendElement(app->config.all,
                                               createConfig("Advanced Configurations", window_config_advanced));

    app->config.active = app->config.all;
    app->config.cursor = app->config.all;

    configresolution_init(app);
    configadvanced_init(app);
}

void config_destroy(app_t *app) {
    linkedlist_freeList(app->config.all, NULL);
    configresolution_destroy(app);
    configadvanced_destroy(app);
}

static config_t *createConfig(char *name, window_t destiny) {
    config_t *config = (config_t *) calloc(1, sizeof(config_t));
    config->name = name;
    config->destiny = destiny;
    return config;
}

static void configresolution_init(app_t *app) {
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("640x480", 640, 480));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("800x600", 800, 600));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("960x720", 960, 720));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1024x576", 1024, 576));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1024x768", 1024, 768));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1152x648", 1152, 648));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1280x720", 1280, 720));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1280x800", 1280, 800));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1280x960", 1280, 960));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1366x768", 1366, 768));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1440x900", 1440, 900));
    app->config.resolution.all = linkedlist_appendElement(app->config.resolution.all,
                                                          createResolution("1400x1050", 1400, 1050));
    app->config.resolution.active = app->config.resolution.all;
    app->config.resolution.cursor = app->config.resolution.all;
}

static resolution_t *createResolution(char *name, int width, int height) {
    resolution_t *resolution = (resolution_t *) calloc(1, sizeof(resolution_t));
    resolution->name = name;
    resolution->width = width;
    resolution->height = height;
    return resolution;
}

static void configresolution_destroy(app_t *app) {
    linkedlist_freeList(app->config.resolution.all, NULL);
}