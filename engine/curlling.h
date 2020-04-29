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

#ifndef ENGINE_CURLLING_H
#define ENGINE_CURLLING_H

#include "../structs.h"

char *curlling_fetchURL(char *url);

int curlling_downloadURL(app_t *app, char *url, char *filename);

int curlling_downloadURLPost(app_t *app, char *url, char *data, char *filename);

char *curlling_fetchURLPost(char *url, char *postData);

#endif
