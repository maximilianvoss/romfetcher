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
#include <string.h>

void *linkedlist_appendElement(void *list, void *elementVoid) {
    linkedlist_t *ptr = list;
    linkedlist_t *element = elementVoid;

    if (element == NULL) {
        return ptr;
    }

    if (ptr == NULL) {
        return element;
    }

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = element;
    element->prev = ptr;
    return list;
}

void *linkedlist_removeElement(void *list, void *element) {
    linkedlist_t *ptr = element;
    linkedlist_t *tmp;

    if (ptr == NULL) {
        return NULL;
    }

    if (ptr->prev == NULL) {
        tmp = ptr->next;
        ptr->next = NULL;
        return tmp;
    }

    if (ptr->next == NULL) {
        tmp = ptr->prev;
        tmp->next = NULL;
        ptr->prev = NULL;
        return list;
    }

    tmp = ptr;
    ((linkedlist_t *) ptr->prev)->next = tmp->next;
    ((linkedlist_t *) ptr->next)->prev = tmp->prev;
    ptr->prev = NULL;
    ptr->next = NULL;
    return list;
}

void *linkedlist_deleteElement(void *list, void *element, void (*callback)(void *)) {
    linkedlist_t *ptr = element;
    linkedlist_t *tmp;

    if (ptr == NULL) {
        return NULL;
    }

    if (ptr->prev == NULL) {
        tmp = ptr->next;
        if (callback != NULL) {
            callback(ptr);
        }
        free(ptr);
        if (tmp != NULL) {
            tmp->prev = NULL;
        }
        return tmp;
    }

    if (ptr->next == NULL) {
        tmp = ptr->prev;
        if (callback != NULL) {
            callback(ptr);
        }
        free(ptr);
        tmp->next = NULL;
        return list;
    }

    tmp = ptr;
    ((linkedlist_t *) ptr->prev)->next = tmp->next;
    ((linkedlist_t *) ptr->next)->prev = tmp->prev;
    if (callback != NULL) {
        callback(tmp);
    }
    free(tmp);
    return list;
}

void *linkedlist_pop(void *list, void **element) {
    if (list == NULL) {
        *element = NULL;
        return NULL;
    }
    linkedlist_t *listPtr = (linkedlist_t *) list;
    linkedlist_t *tmp = listPtr->next;
    if (tmp != NULL) {
        tmp->prev = NULL;
    }
    listPtr->prev = NULL;
    listPtr->next = NULL;
    *element = listPtr;
    return tmp;
}

void linkedlist_freeList(void *list, void (*callback)(void *)) {
    if (list == NULL) {
        return;
    }
    linkedlist_t *next = ((linkedlist_t *) list)->next;
    linkedlist_freeList(next, callback);
    
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

void *linkedlist_clone(void *ptr, size_t size, void (*callback)(void *)) {
    if (ptr == NULL) {
        return NULL;
    }
    linkedlist_t *list = ptr;
    linkedlist_t *clone = NULL;

    while (list != NULL) {
        linkedlist_t *clonedElement = calloc(1, size);
        memcpy(clonedElement, list, size);
        clonedElement->next = NULL;
        clonedElement->prev = NULL;
        if (callback != NULL) {
            callback(clone);
        }
        clone = linkedlist_appendElement(clone, clonedElement);
        list = list->next;
    }
    return clone;
}

void *linkedlist_getNextActive(void *ptr) {
    linkedlist_t *list = ptr;
    if (list == NULL) {
        return NULL;
    }
    list = list->next;
    while (list != NULL) {
        if (list->active) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

void *linkedlist_getPrevActive(void *ptr) {
    linkedlist_t *list = ptr;
    if (list == NULL) {
        return NULL;
    }
    list = list->prev;
    while (list != NULL) {
        if (list->active) {
            return list;
        }
        list = list->prev;
    }
    return NULL;
}


void *linkedlist_getFirstActive(void *ptr) {
    linkedlist_t *list = ptr;
    while (list != NULL) {
        if (list->active) {
            return list;
        }
        list = list->next;
    }
    return NULL;
}

void *linkedlist_push(void *list, void *element) {
    if (element == NULL) {
        return list;
    }
    if (list == NULL) {
        return element;
    }
    ((linkedlist_t *) element)->next = list;
    return element;
}

uint8_t linkedlist_isElementInList(void *list, void *element) {
    if (element == NULL) {
        return 0;
    }
    if (list == NULL) {
        return 0;
    }

    linkedlist_t *ptr = list;
    while (ptr != NULL) {
        if (ptr == element) {
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}

void *linkedlist_getLastElement(void *list) {
    linkedlist_t *ptr = list;
    if (ptr == NULL) {
        return NULL;
    }
    if (ptr->next == NULL) {
        return ptr;
    }

    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    return ptr;
}