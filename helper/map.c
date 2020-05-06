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

#include "map.h"
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

typedef struct s_linkedlist {
    char *key;
    char *value;
    struct s_linkedlist *next;
} linkedlist_t;

void *hash_createMap() {
#ifdef DEBUG
    printf("hash_createMap()...\n");
#endif
    linkedlist_t *map = (linkedlist_t *) malloc(sizeof(linkedlist_t));
    map->key = NULL;
    map->value = NULL;
    map->next = NULL;
#ifdef DEBUG
    printf("hash_createMap()... DONE\n");
#endif
    return map;
}

void hash_destroyMap(void *data) {
#ifdef DEBUG
    printf("hash_destroyMap([void *])...\n");
#endif
    if (data == NULL) {
#ifdef DEBUG
        printf("hash_destroyMap([void *])... DONE\n");
#endif
        return;
    }
    linkedlist_t *map = (linkedlist_t *) data;
    hash_destroyMap(map->next);
    if (map->key != NULL) {
        free(map->key);
    }
    if (map->value != NULL) {
        free(map->value);
    }
    free(map);
#ifdef DEBUG
    printf("hash_destroyMap([void *])... DONE\n");
#endif
}

void hash_set(void *data, char *key, char *value) {
#ifdef DEBUG
    printf("hash_put([void *], %s, %s)...\n", key, value);
#endif
    linkedlist_t *map = (linkedlist_t *) data;

    if (key == NULL) {
#ifdef DEBUG
        printf("hash_put([void *], %s, %s)... DONE\n", key, value);
#endif
        return;
    }

    size_t length = 0;
    if (value != NULL) {
        length = strlen(value) + 1;
    }
    while (map != NULL) {
        if (map->key != NULL && !strcmp(map->key, key)) {
            if (map->value != NULL) {
                free(map->value);
            }
            if (value != NULL) {
                map->value = (char *) malloc(sizeof(char) * length);
                strcpy(map->value, value);
            } else {
                map->value = NULL;
            }
            return;
        }
        map = map->next;
    }

    linkedlist_t *element = (linkedlist_t *) malloc(sizeof(linkedlist_t));
    if (value != NULL) {
        element->value = (char *) malloc(sizeof(char) * length);
        strcpy(element->value, value);
    } else {
        element->value = NULL;
    }

    length = strlen(key) + 1;
    element->key = (char *) malloc(sizeof(char) * length);
    strcpy(element->key, key);

    map = (linkedlist_t *) data;
    element->next = map->next;
    map->next = element;
#ifdef DEBUG
    printf("hash_put([void *], %s, %s)... DONE\n", key, value);
#endif
}

void hash_unset(void *data, char *key) {
#ifdef DEBUG
    printf("hash_unset([void *], %s)...\n", key);
#endif

    linkedlist_t *map = (linkedlist_t *) data;
    if (key == NULL) {
#ifdef DEBUG
        printf("hash_unset([void *], %s)... DONE\n", key);
#endif
        return;
    }

    char *realKey = key;
    char wildcard = 0;
    size_t keyLength = strlen(key);
    if (key[keyLength - 1] == '*') {
        realKey = calloc(sizeof(char), keyLength);
        strncpy(realKey, key, keyLength - 1);
        keyLength--;
        wildcard = 1;
    }

    while (map->next != NULL) {
        if (map->next->key != NULL && ((!wildcard && !strcmp(map->next->key, realKey)) ||
                                       (wildcard && !strncmp(map->next->key, realKey, keyLength)))) {
            linkedlist_t *tmp = map->next;
            map->next = map->next->next;

            free(tmp->key);
            if (tmp->value != NULL) {
                free(tmp->value);
            }
            free(tmp);

            if (!wildcard) {
                return;
            }
        }
        map = map->next;
    }

    if (wildcard) {
        free(realKey);
    }
#ifdef DEBUG
    printf("hash_unset([void *], %s)... DONE\n", key);
#endif
    return;
}

char *hash_get(void *data, char *key) {
#ifdef DEBUG
    printf("hash_get([void *], %s)...\n", key);
#endif

    linkedlist_t *map = (linkedlist_t *) data;
    if (key == NULL) {
#ifdef DEBUG
        printf("hash_get([void *], %s)... DONE\n", key);
#endif
        return NULL;
    }
    while (map != NULL) {
        if (map->key != NULL && !strcmp(map->key, key)) {
            return map->value;
        }
        map = map->next;
    }
#ifdef DEBUG
    printf("hash_get([void *], %s)... DONE\n", key);
#endif
    return NULL;
}