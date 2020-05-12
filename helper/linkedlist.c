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

#include "linkedlist.h"

void *linkedlist_appendElement(void *list, void *element) {
    linkedlist_t *ptr = list;
    if (ptr == NULL) {
        return element;
    }
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = element;
    ((linkedlist_t *) element)->prev = ptr;
    return list;
}

void linkedlist_freeList(void *list, void (*callback)(void *)) {
    if (list == NULL) {
        return;
    }
    linkedlist_t *next = ((linkedlist_t *) list)->next;
    if (next != NULL) {
        linkedlist_freeList(next, callback);
    }
    callback(list);
    free(list);
}

uint32_t linkedlist_getElementCount(void *list) {
    uint32_t count = 0;
    while (list != NULL) {
        count++;
        list = ((linkedlist_t *) list)->next;
    }
    return count;
}