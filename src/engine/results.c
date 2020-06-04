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

#include <csafestring.h>
#include "results.h"
#include "../helper/utils.h"

static void freeFields(void *ptr);

searchresult_t *result_newItem(system_t *system, engine_t *engine) {
    searchresult_t *resultList = (searchresult_t *) calloc(1, sizeof(searchresult_t));
    resultList->title = NULL;
    resultList->active = 1;
    resultList->url = NULL;
    resultList->engine = engine;
    resultList->system = system;
    resultList->prev = NULL;
    resultList->next = NULL;
    return resultList;
}

void result_setTitle(searchresult_t *resultList, char *title) {
    SDL_Log("Title: %s\n", title);
    if (resultList == NULL || title == NULL) {
        return;
    }
    resultList->title = str_clone(title);
}

void result_setUrl(searchresult_t *resultList, char *url) {
    SDL_Log("URL: %s\n", url);
    if (resultList == NULL || url == NULL) {
        return;
    }
    resultList->url = str_clone(url);
}

void result_freeList(searchresult_t *resultList) {
    linkedlist_freeList(resultList, &freeFields);
}

static void freeFields(void *ptr) {
    searchresult_t *resultList = (searchresult_t *) ptr;
    FREENOTNULL(resultList->title);
    FREENOTNULL(resultList->url);
}