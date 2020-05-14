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

void config_init(app_t *app) {
    app->config.all = linkedlist_appendElement(app->config.all,
                                               createConfig("Select Search Engine", window_config_engine));
    app->config.all = linkedlist_appendElement(app->config.all,
                                               createConfig("Enable/Disable Systems", window_config_systems));
    app->config.all = linkedlist_appendElement(app->config.all, createConfig("Select Theme", window_config_themes));
    app->config.all = linkedlist_appendElement(app->config.all,
                                               createConfig("Advanced Configurations", window_config_advanced));

    app->config.active = app->config.all;
    app->config.cursor = app->config.all;

    configadvanced_init(app);
}

void config_destroy(app_t *app) {
    linkedlist_freeList(app->config.all, NULL);
    configadvanced_destroy(app);
}

static config_t *createConfig(char *name, window_t destiny) {
    config_t *config = (config_t *) calloc(1, sizeof(config_t));
    config->name = name;
    config->destiny = destiny;
    return config;
}

