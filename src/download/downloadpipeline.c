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

#include "downloadpipeline.h"

acll_t *downloadpipeline_getNext(app_t *app, acll_t *element) {
    if (element == NULL) {
        return NULL;
    }
    if (element->next != NULL) {
        return element->next;
    }

    uint8_t isInActive = acll_in(app->download.active, element);
    if (isInActive) {
        if (app->download.queue != NULL) {
            return app->download.queue;
        }
        if (app->download.done != NULL) {
            return app->download.done;
        }
        return NULL;
    }
    uint8_t isInQueue = acll_in(app->download.queue, element);
    if (isInQueue) {
        if (app->download.done != NULL) {
            return app->download.done;
        }
    }
    return NULL;
}

acll_t *downloadpipeline_getPrev(app_t *app, acll_t *element) {
    if (element == NULL) {
        return NULL;
    }
    if (element->prev != NULL) {
        return element->prev;
    }

    uint8_t isInDone = acll_in(app->download.done, element);
    if (isInDone) {
        if (app->download.queue != NULL) {
            return acll_last(app->download.queue);
        }
        if (app->download.active != NULL) {
            return acll_last(app->download.active);
        }
        return NULL;
    }
    uint8_t isInQueue = acll_in(app->download.queue, element);
    if (isInQueue) {
        if (app->download.active != NULL) {
            return acll_last(app->download.active);
        }
    }
    return NULL;
}