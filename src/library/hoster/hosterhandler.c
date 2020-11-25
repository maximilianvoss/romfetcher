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

#include <pthread.h>
#include "hosterhandler.h"
#include "romhustler/romhustler.h"
#include "romsdownload/romsdownload.h"
#include "romsemulator/romsemulator.h"
#include "romsmania/romsmania.h"
#include "wowroms/wowroms.h"
#include "freeroms/freeroms.h"

static void *executeThread(void *searchPtr);

typedef struct {
    hoster_t *hoster;
    system_t *system;
    char *searchString;
    pthread_t thread;
    result_t *result;
} search_t;

hoster_t *hosterhandler_init(cache_t *cacheHandler) {
    hoster_t *hosters = NULL;
    hosters = ll_append(hosters, freeroms_getHoster(cacheHandler));
    hosters = ll_append(hosters, romhustler_getHoster(cacheHandler));
    hosters = ll_append(hosters, romsdownload_getHoster(cacheHandler));
    hosters = ll_append(hosters, romsemulator_getHoster(cacheHandler));
    hosters = ll_append(hosters, romsmania_getHoster(cacheHandler));
    hosters = ll_append(hosters, wowroms_getHoster(cacheHandler));
    return hosters;
}

result_t *hosterhandler_search(hoster_t *hosters, system_t *system, char *searchString) {
    result_t *result = NULL;
    hoster_t *ptr = hosters;

    uint32_t count = ll_count(ptr);
    search_t *searches = (search_t *) calloc(count, sizeof(search_t));

    int activeNumber = 0;
    while (ptr != NULL) {
        if (ptr->active) {
            searches[activeNumber].hoster = ptr;
            searches[activeNumber].system = system;
            searches[activeNumber].searchString = searchString;
            searches[activeNumber].result = NULL;
            pthread_create(&searches[activeNumber].thread, NULL, executeThread, &searches[activeNumber]);
            activeNumber++;
        }
        ptr = ptr->next;
    }

    for (int i = 0; i < activeNumber; i++) {
        pthread_join(searches[i].thread, NULL);
        result = ll_append(result, searches[i].result);
    }

    return ll_sort(result);
}

void hosterhandler_download(result_t *item, downloadCallback_t downloadCallbackFunction, void *appData) {
    item->hoster->download(item, downloadCallbackFunction, appData);
}

void hosterhandler_destroy(hoster_t *hoster) {
    ll_free(hoster, NULL);
}

static void *executeThread(void *searchPtr) {
    search_t *search = (search_t *) searchPtr;
    LOG_DEBUG("Call search on: %s\n", search->hoster->name);
    search->result = search->hoster->search(search->system, search->searchString);
    LOG_DEBUG("Done with search on: %s\n", search->hoster->name);
    return NULL;
}
