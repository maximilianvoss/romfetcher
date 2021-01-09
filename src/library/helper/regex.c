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

#include "regex.h"
#include <stdio.h>
#include <regex.h>
#include <string.h>

regexMatches_t *regex_getMatches(char *text, char *regexString, size_t maxGroups) {
    regexMatches_t *matches = NULL;
    regexMatches_t *matchPtr;
    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups + 1];
    char *cursor;

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
        printf("Could not compile regular expression: %s\n", regexString);
        return NULL;
    };

    cursor = text;
    while (1) {
        if (regexec(&regexCompiled, cursor, maxGroups + 1, groupArray, 0)) {
            break;  // No more matches
        }

        regexMatches_t *match = (regexMatches_t *) calloc(1, sizeof(regexMatches_t));
        match->groups = (char **) calloc(maxGroups, sizeof(char *));
        match->maxGroups = maxGroups;

        if (matches == NULL) {
            matches = match;
            matchPtr = match;
        } else {
            matchPtr->next = match;
            match->prev = matchPtr;
            matchPtr = match;
        };

        unsigned int g = 0;
        unsigned int offset = 0;
        for (g = 0; g < maxGroups + 1; g++) {
            if (groupArray[g].rm_so == (size_t) -1) {
                break;  // No more groups
            }

            if (g == 0) {
                offset = groupArray[g].rm_eo;
            } else {
                char cursorCopy[strlen(cursor) + 1];
                strcpy(cursorCopy, cursor);
                cursorCopy[groupArray[g].rm_eo] = 0;

                match->groups[g - 1] = (char *) calloc(strlen(cursorCopy + groupArray[g].rm_so) + 1, sizeof(char));
                strcpy(match->groups[g - 1], cursorCopy + groupArray[g].rm_so);
            }
        }
        cursor += offset;
    }
    regfree(&regexCompiled);
    return matches;
}

char *regex_cpyGroupText(regexMatches_t *match, int group) {
    if (group >= match->maxGroups) {
        return NULL;
    }
    if (match->groups[group] == NULL) {
        return NULL;
    }
    char *str = calloc(strlen(match->groups[group]) + 1, sizeof(char));
    strcpy(str, match->groups[group]);
    return str;
}

void regex_destroyMatches(regexMatches_t *matches) {
    if (matches == NULL) {
        return;
    }
    for (int i = 0; i < matches->maxGroups; i++) {
        if (matches->groups[i] != NULL) {
            free(matches->groups[i]);
        }
    }
    free(matches->groups);
    regex_destroyMatches(matches->next);
    free(matches);
}