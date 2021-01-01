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

#ifndef HELPER_LINKEDLIST_H
#define HELPER_LINKEDLIST_H

#include <stdint.h>
#include <stdlib.h>

void *linkedlist_appendElement(void *list, void *element);

void *linkedlist_pop(void *list, void **element);

void *linkedlist_push(void *list, void *element);

void linkedlist_freeList(void *list, void (*callback)(void *));

uint32_t linkedlist_getElementCount(void *list);

void *linkedlist_findElementByName(void *ptr, char *name);

uint8_t linkedlist_isElementInList(void *list, void *element);

void *linkedlist_deleteElement(void *list, void *element, void (*callback)(void *));

void *linkedlist_removeElement(void *list, void *element);

void *linkedlist_sort(void *ptr);

void *linkedlist_clone(void *ptr, size_t size, void (*callback)(void *));

void *linkedlist_getFirstElement(void *ptr);

void *linkedlist_getFirstActive(void *ptr);

void *linkedlist_getPrevActive(void *ptr);

void *linkedlist_getNextActive(void *ptr);

void *linkedlist_getLastElement(void *list);

#endif
