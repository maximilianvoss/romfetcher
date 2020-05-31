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
#include "../src/helper/linkedlist.h"
#include "test.h"

int test_linkedList_deleteElement0(void *data) {
    linkedlist_t *list = NULL;
    list = linkedList_removeElement(list, list, NULL);
    ASSERTNULL(list);
    return 0;
}

int test_linkedList_deleteElement1(void *data) {
    linkedlist_t *list = NULL;
    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element1";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element3";

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTSTR("Element3", ((linkedlist_t *) ((linkedlist_t *) list->next)->next)->name);

    list = linkedList_removeElement(list, list, NULL);

    ASSERTSTR("Element2", list->name);
    ASSERTSTR("Element3", ((linkedlist_t *) list->next)->name);

    return 0;
}

int test_linkedList_deleteElement2(void *data) {
    linkedlist_t *list = NULL;
    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element1";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element3";

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTSTR("Element3", ((linkedlist_t *) ((linkedlist_t *) list->next)->next)->name);

    list = linkedList_removeElement(list, list->next, NULL);

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element3", ((linkedlist_t *) list->next)->name);

    return 0;
}

int test_linkedList_deleteElement3(void *data) {
    linkedlist_t *list = NULL;
    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element1";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element3";

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTSTR("Element3", ((linkedlist_t *) ((linkedlist_t *) list->next)->next)->name);

    list = linkedList_removeElement(list, (linkedlist_t *) ((linkedlist_t *) list->next)->next, NULL);

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTNULL((linkedlist_t *) ((linkedlist_t *) list->next)->next);

    return 0;
}

int test_linkedList_deleteElement4(void *data) {
    linkedlist_t *list = calloc(1, sizeof(linkedlist_t));
    list->name = "Element1";
    list->next = NULL;
    list->prev = NULL;
    list = linkedList_removeElement(list, list, NULL);
    ASSERTNULL(list);
    return 0;
}

int test_linkedList_sortElements(void *data) {
    linkedlist_t *list = NULL;
    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element3";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element1";

    ASSERTSTR("Element3", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTSTR("Element1", ((linkedlist_t *) ((linkedlist_t *) list->next)->next)->name);

    list = linkedlist_sort(list);

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTSTR("Element3", ((linkedlist_t *) ((linkedlist_t *) list->next)->next)->name);

    return 0;
}

int test_linkedList_findElementFound(void *data) {
    linkedlist_t *list = NULL;
    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element1";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element3";

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTSTR("Element3", ((linkedlist_t *) ((linkedlist_t *) list->next)->next)->name);

    list = linkedlist_findElementByName(list, "Element2");

    ASSERTSTR("Element2", list->name);

    return 0;
}

int test_linkedList_findElementNotFound(void *data) {
    linkedlist_t *list = NULL;
    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element1";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element3";

    ASSERTSTR("Element1", list->name);
    ASSERTSTR("Element2", ((linkedlist_t *) list->next)->name);
    ASSERTSTR("Element3", ((linkedlist_t *) ((linkedlist_t *) list->next)->next)->name);

    list = linkedlist_findElementByName(list, "Not Found");

    ASSERTNULL(list);

    return 0;
}

int test_linkedList_getElementCountNull(void *data) {
    linkedlist_t *list = NULL;
    int count = linkedlist_getElementCount(list);
    ASSERTINT(0, count);
    return 0;
}

int test_linkedList_getElementCountNotNull(void *data) {
    linkedlist_t *list = NULL;

    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element1";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element3";

    int count = linkedlist_getElementCount(list);
    ASSERTINT(3, count);

    return 0;
}

int test_linkedList_cloneListNull(void *data) {
    linkedlist_t *list = NULL;
    list = linkedlist_clone(list, sizeof(linkedlist_t), NULL);
    ASSERTNULL(list);
    return 0;
}

int test_linkedList_cloneListNotNull(void *data) {
    linkedlist_t *list = NULL;
    linkedlist_t *element1 = calloc(1, sizeof(linkedlist_t));
    element1->name = "Element1";
    list = linkedlist_appendElement(list, element1);
    linkedlist_t *element2 = calloc(1, sizeof(linkedlist_t));
    element2->name = "Element2";
    list = linkedlist_appendElement(list, element2);
    linkedlist_t *element3 = calloc(1, sizeof(linkedlist_t));
    list = linkedlist_appendElement(list, element3);
    element3->name = "Element3";

    linkedlist_t *clonedList = linkedlist_clone(list, sizeof(linkedlist_t), NULL);

    ASSERTNOTNULL(clonedList);

    ASSERTSTR("Element1", clonedList->name);
    ASSERTSTR("Element2", ((linkedlist_t *) clonedList->next)->name);
    ASSERTSTR("Element3", ((linkedlist_t *) ((linkedlist_t *) clonedList->next)->next)->name);

    ASSERTNULL(list->prev);
    ASSERTNULL(clonedList->prev);
    ASSERTPTRNOTEQUAL(list, clonedList);
    list = list->next;
    clonedList = clonedList->next;

    ASSERTPTRNOTEQUAL(list, clonedList);
    ASSERTPTRNOTEQUAL(list->prev, clonedList->prev);
    ASSERTPTRNOTEQUAL(list->next, clonedList->next);

    return 0;
}
