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

#ifndef DOWNLOAD_CURLLING_H
#define DOWNLOAD_CURLLING_H

#include <curl/system.h>
#include <stdlib.h>
#include "../romfetcher.h"

int
curlling_download(char *url, char *data, httpmethod_t method, char *filename, curl_off_t *current, curl_off_t *total,
                  volatile uint8_t *cancellation);

curl_response_t *curlling_fetch(char *url, char *postData, httpmethod_t method, long throwHeaderOut);

void curl_freeResponse(curl_response_t *response);

#endif
