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

#ifndef HELPER_UTILS_H
#define HELPER_UTILS_H

#include <stdlib.h>
#include <stdint.h>

#define FREENOTNULL(x) if (x != NULL) free (x)

char *str_replace(char *orig, char *rep, char *with);

char *file_name(char *path);

char *file_suffix(char *path);

uint8_t file_exists(char *path);

char *str_urlDecode(const char *string);

char *str_htmlDecode(const char *string);

char *str_concat(char *str1, char *str2);

char *str_quoteDecode(char *strIn);

char *str_clone(char *str);

char *file_parentDirectory(char *path);

uint8_t str_contains(char *str, char *searchStr);

char *str_trim(char *str);

#endif
