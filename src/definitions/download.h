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

#ifndef DEFINITIONS_DOWNLOAD_H
#define DEFINITIONS_DOWNLOAD_H

typedef struct download_s {
    char *title;
    uint8_t active;
    rl_system *system;
    char *url;
    char *data;
    char *filename;
    chttp_method method;
    curl_off_t current;
    curl_off_t total;
    volatile uint8_t cancelled;
} download_t;

#define getDownload(download) ((download_t *) download->payload)

#endif
