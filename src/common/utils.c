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
#include <stdio.h>
#include <unistd.h>

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

    tmp = result = calloc(strlen(orig) + (len_with - len_rep) * count + 1, sizeof(char));

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

char *file_suffix(char *path) {
    char *ptr = path;
    char *result = path;
    if (ptr == NULL || *ptr == '\0') {
        return result;
    }
    while (*ptr != '\0') {
        if (*ptr == '.') {
            result = ptr;
        }
        ptr++;
    }
    return result;
}

char *file_parentDirectory(char *path) {
    char *ptr = str_clone(path);
    char *result = ptr;
    char *lastSlash = NULL;

    while (ptr != NULL && *ptr != '\0') {
        if (*ptr == '/') {
            lastSlash = ptr;
        }
        ptr++;
    }
    if (lastSlash != NULL) {
        *lastSlash = '\0';
    }
    return result;
}

char *str_urlDecode(const char *string) {
    char *o;
    int length = strlen(string);
    const char *end = string + length;
    int c;
    char *decoded = calloc(length + 1, sizeof(char));

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
    char *decoded = calloc(strlen(string) + 1, sizeof(char));
    decode_html_entities_utf8(decoded, string);
    return decoded;
}

char *str_concat(char *str1, char *str2) {
    if (str1 == NULL || str2 == NULL) {
        return NULL;
    }
    int length = strlen(str1) + strlen(str2) + 1;
    char *newStr = (char *) calloc(length, sizeof(char));
    char *ptr = newStr;
    while (*str1 != '\0') {
        *ptr = *str1;
        ptr++;
        str1++;
    }
    while (*str2 != '\0') {
        *ptr = *str2;
        ptr++;
        str2++;
    }
    *ptr = '\0';
    return newStr;
}

char *str_quoteDecode(char *string) {
    char *returnStr = calloc(strlen(string) + 1, sizeof(char));
    char *strIn = string;
    char *ptr = returnStr;

    while (*strIn != '\0') {
        if (*strIn == '\\') {
            strIn++;
        }
        *ptr = *strIn;
        ptr++;
        strIn++;
    }
    return returnStr;
}

uint8_t file_exists(char *path) {
    return (access(path, F_OK) != -1) ? 1 : 0;
}

char *str_clone(char *str) {
    if (str == NULL) {
        return NULL;
    }
    char *clone = calloc(strlen(str) + 1, sizeof(char));
    strcpy(clone, str);
    return clone;
}

uint8_t str_contains(char *str, char *searchStr) {
    if (strstr(str, searchStr) != NULL) {
        return 1;
    }
    return 0;
}

char *str_trim(char *str) {
    if (str == NULL) {
        return NULL;
    }

    char *tmp = str;
    while ((*tmp == ' ' || *tmp == '\t' || *tmp == '\v' || *tmp == '\f' || *tmp == '\n' || *tmp == '\r' ||
            *tmp == -62 || *tmp == -96) &&
           *tmp != '\0') {
        tmp++;
    }

    if (*tmp == '\0') {
        return "";
    }

    char *start = tmp;
    while (*tmp != '\0') {
        tmp++;
    }
    tmp--;

    while (*tmp == ' ' || *tmp == '\t' || *tmp == '\v' || *tmp == '\f' || *tmp == '\n' || *tmp == '\r' || *tmp == -62 ||
           *tmp == -96) {
        tmp--;
    }
    char *end = tmp;


    size_t length = end - start + 1;
    memmove(str, start, length);

    tmp = str + length;
    *tmp = '\0';

    return str;
}

static int ishex(int x) {
    return (x >= '0' && x <= '9') ||
           (x >= 'a' && x <= 'f') ||
           (x >= 'A' && x <= 'F');
}

