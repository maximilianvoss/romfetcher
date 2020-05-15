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
    if (callback != NULL) {
        callback(list);
    }
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

void *linkedlist_findElementByName(void *ptr, char *name) {
    linkedlist_t *list = (linkedlist_t *) ptr;
    if (list == NULL || name == NULL) {
        return NULL;
    }

    while (list != NULL) {
        if (list->name != NULL && !strcmp(list->name, name)) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

void *linkedlist_sort(void *ptr) {
    if (ptr == NULL) {
        return NULL;
    }

    linkedlist_t *list = (linkedlist_t *) ptr;
    while (list->prev != NULL) {
        list = list->prev;
    }

    linkedlist_t *cur = list->next;
    while (cur != NULL) {

        linkedlist_t *check = cur->prev;
        while (check != NULL) {
            if (strcmp(check->name, cur->name) < 0) {
                break;
            }
            check = check->prev;
        }

        if (check == NULL) {
            linkedlist_t *prev = cur->prev;
            linkedlist_t *next = cur->next;

            if (prev != NULL) {
                prev->next = next;
            }
            if (next != NULL) {
                next->prev = prev;
            }

            if (cur != list) {
                cur->next = list;
            }
            cur->prev = NULL;
            list->prev = cur;
            list = cur;

            cur = next;
        } else {
            linkedlist_t *prev = cur->prev;
            linkedlist_t *next = cur->next;

            if (cur->prev != check) {

                cur->next = check->next;
                if (cur->next != NULL) {
                    ((linkedlist_t *) cur->next)->prev = cur;
                }

                check->next = cur;
                cur->prev = check;

                if (prev != NULL) {
                    prev->next = next;
                }
                if (next != NULL) {
                    next->prev = prev;
                }
            }

            cur = next;
        }
    }


    while (list->prev != NULL) {
        list = list->prev;
    }
    return list;
}