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
#include "../romfetcher.h"
#include "../../common/utils.h"

static void freeFields(void *ptr);

result_t *result_newItem(system_t *system, hoster_t *hoster) {
    result_t *resultList = (result_t *) calloc(1, sizeof(result_t));
    resultList->title = NULL;
    resultList->active = 1;
    resultList->url = NULL;
    resultList->hoster = hoster;
    resultList->system = system;
    resultList->downloads = 0;
    resultList->rating = 0;
    resultList->fileSize = NULL;
    resultList->prev = NULL;
    resultList->next = NULL;
    return resultList;
}

void result_setTitle(result_t *resultList, char *title) {
    title = str_trim(title);
    LOG_DEBUG("Title: %s", title);
    if (resultList == NULL || title == NULL) {
        return;
    }
    resultList->title = str_clone(title);
}

void result_setUrl(result_t *resultList, char *url) {
    url = str_trim(url);
    LOG_DEBUG("URL: %s", url);
    if (resultList == NULL || url == NULL) {
        return;
    }
    resultList->url = str_clone(url);
}

void result_setDownloads(result_t *resultList, char *downloads) {
    downloads = str_trim(downloads);
    LOG_DEBUG("DOWNLOADS: %s", downloads);
    if (downloads == NULL) {
        return;
    }
    resultList->downloads = atoi(downloads);
    LOG_DEBUG("Calc'd Downloads: %d", resultList->downloads);
}

void result_setRating(result_t *resultList, char *rating, uint8_t maxRating) {
    rating = str_trim(rating);
    LOG_DEBUG("RATING: %s", rating);
    if (rating == NULL) {
        return;
    }
    resultList->rating = (maxRating == 10) ? 1.0f * atof(rating) : 2.0f * atof(rating);
    LOG_DEBUG("Calc'd Rating: %2.1f", resultList->rating);
}

void result_setFileSize(result_t *resultList, char *fileSize) {
    fileSize = str_trim(fileSize);
    LOG_DEBUG("FILE SIZE: %s", fileSize);
    if (fileSize == NULL) {
        return;
    }
    resultList->fileSize = str_clone(fileSize);
}

void result_freeList(result_t *resultList) {
    ll_free(resultList, &freeFields);
}


static void freeFields(void *ptr) {
    result_t *resultList = (result_t *) ptr;
    FREENOTNULL(resultList->title);
    FREENOTNULL(resultList->url);
    FREENOTNULL(resultList->fileSize);
}