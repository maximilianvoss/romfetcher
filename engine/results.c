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

#include "results.h"

searchresult_t *newResultItem(system_t *system) {
    searchresult_t *resultList = (searchresult_t *) malloc(sizeof(searchresult_t));
    resultList->title = NULL;
    resultList->url = NULL;
    resultList->system = system;
    resultList->prev = NULL;
    resultList->next = NULL;
    return resultList;
}

searchresult_t *addResultItemIntoList(searchresult_t *resultList, searchresult_t *newItem) {
    if (resultList == NULL) {
        return newItem;
    }

    searchresult_t *tmp = resultList;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = newItem;
    newItem->prev = tmp;
    return resultList;
}

void setTitle(searchresult_t *resultList, char *title) {
    if (resultList == NULL || title == NULL) {
        return;
    }
    int length = strlen(title) + 1;
    resultList->title = (char *) malloc(sizeof(char) * length);
    memcpy(resultList->title, title, length);
}

void setUrl(searchresult_t *resultList, char *url) {
    if (resultList == NULL || url == NULL) {
        return;
    }
    int length = strlen(url) + 1;
    resultList->url = (char *) malloc(sizeof(char) * length);
    memcpy(resultList->url, url, length);
}

void freeResultList(searchresult_t *resultList) {
    if (resultList == NULL) {
        return;
    }
    searchresult_t *next = resultList->next;
    if (next != NULL) {
        freeResultList(next);
    }
    if (resultList->title != NULL) {
        free(resultList->title);
    }
    if (resultList->url != NULL) {
        free(resultList->url);
    }
    free(resultList);
}

uint32_t getResultListCount(searchresult_t *resultList) {
    uint32_t count = 0;
    while (resultList != NULL) {
        count++;
        resultList = resultList->next;
    }
    return count;
}