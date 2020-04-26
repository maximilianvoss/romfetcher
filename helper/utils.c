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

#include "utils.h"
#include "entities.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL_video.h>

static int ishex(int x);

char *str_replace(char *orig, char *rep, char *with) {
    char *result;
    char *ins;
    char *tmp;
    int len_rep;
    int len_with;
    int len_front;
    int count;

    if (!orig || !rep) {
        return NULL;
    }
    len_rep = strlen(rep);
    if (len_rep == 0) {
        return NULL;
    }
    if (!with) {
        with = "";
    }
    len_with = strlen(with);

    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result) {
        return NULL;
    }

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep;
    }
    strcpy(tmp, orig);
    return result;
}

char *file_name(char *path) {
    char *ptr = path;
    char *result = path;
    if (ptr == NULL || *ptr == '\0') {
        return result;
    }
    while (*ptr != '\0') {
        if (*ptr == '/') {
            result = ptr;
        }
        ptr++;
    }
    return result;
}

char *str_urlDecode(const char *string) {
    char *o;
    int length = strlen(string);
    const char *end = string + length;
    int c;
    char *decoded = malloc(sizeof(char) * length + 1);

    for (o = decoded; string <= end; o++) {
        c = *string++;
        if (c == '+') c = ' ';
        else if (c == '%' && (!ishex(*string++) ||
                              !ishex(*string++) ||
                              !sscanf(string - 2, "%2x", &c)))
            return NULL;

        if (decoded) *o = c;
    }
    return decoded;
}

char *str_htmlDecode(const char *string) {
    char *decoded = malloc(sizeof(char) * strlen(string) + 1);
    decode_html_entities_utf8(decoded, string);
    return decoded;
}

char *path_toResource(char *path) {

}

static int ishex(int x) {
    return (x >= '0' && x <= '9') ||
           (x >= 'a' && x <= 'f') ||
           (x >= 'A' && x <= 'F');
}

