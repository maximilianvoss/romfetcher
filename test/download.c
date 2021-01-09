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

#include "download.h"
#include "test.h"

uint8_t testDownloadCallback(void *appData, system_t *system, char *title, char *url, char *data, char *filename,
                             httpmethod_t method) {
    test_downloaddata_t *input = appData;

    ASSERTNOTNULL(input);

    if (input->system == NULL) {
        ASSERTNULL(system);
    } else {
        ASSERTNOTNULL(system);
        ASSERTSTR(input->system->name, system->name);
    }

    if (input->title == NULL) {
        ASSERTNULL(title);
    } else {
        ASSERTNOTNULL(title);
        ASSERTSTR(input->title, title);
    }

    if (input->url == NULL) {
        ASSERTNULL(url);
    } else {
        ASSERTNOTNULL(url);
        ASSERTSTR(input->url, url);
    }

    if (input->data == NULL) {
        ASSERTNULL(data);
    } else {
        ASSERTNOTNULL(data);
        ASSERTSTR(input->data, data);
    }

    if (input->filename == NULL) {
        ASSERTNULL(filename);
    } else {
        ASSERTNOTNULL(filename);
        ASSERTSTR(input->filename, filename);
    }

    ASSERTINT(input->method, method);
    return 0;
}
