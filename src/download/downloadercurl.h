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

#ifndef DOWNLOAD_DOWNLOADERCURL_H
#define DOWNLOAD_DOWNLOADERCURL_H

#include "../structs.h"

void downloadercurl_init(app_t *app);

void downloadercurl_destroy(app_t *app);

void downloadercurl_addToQueue(app_t *app, system_t *systemt, char *title, char *url, char *data, char *filename,
                               httpmethod_t method);

void downloadercurl_cancel(app_t *app, download_t *download);

uint8_t downloadercurl_isActive(app_t *app);

void downloadercurl_cancelAllDownloads(app_t *app);

#endif