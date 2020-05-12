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

#ifndef HELPER_REGEX_H
#define HELPER_REGEX_H

#include <stdlib.h>

typedef struct regexMatches_s {
    size_t maxGroups;
    char **groups;
    struct regexMatches_s *prev;
    struct regexMatches_s *next;
} regexMatches_t;


regexMatches_t *regex_getMatches(char *text, char *regexString, size_t maxGroups);

char *regex_cpyGroupText(regexMatches_t *match, int group);

void regex_destroyMatches(regexMatches_t *matches);

#endif
