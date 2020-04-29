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

#include "enginehandler.h"
#include "romsmania/romsmania.h"
#include "romsmode/romsmode.h"
#include "wowroms/wowroms.h"
#include "romsdownload/romsdownload.h"
#include "romsemulator/romsemulator.h"

static void determineSearchEngine(app_t *app);

static searchresult_t *(*searchFunction)(app_t *app, system_t *system, char *searchString);

static void (*downloadFunction)(app_t *app, searchresult_t *item, void (*callback)(app_t *app));

searchresult_t *enginehandler_search(app_t *app, system_t *system, char *searchString) {
    determineSearchEngine(app);
    if (searchFunction != NULL) {
        return searchFunction(app, system, searchString);
    }
    return NULL;
}

void enginehandler_download(app_t *app, searchresult_t *item, void (*callback)(app_t *app)) {
    determineSearchEngine(app);
    if (downloadFunction != NULL) {
        downloadFunction(app, item, callback);
    }
}

static void determineSearchEngine(app_t *app) {
    switch (app->search.engine) {
        case romsmania:
            searchFunction = &romsmania_search;
            downloadFunction = &romsmania_download;
            break;
        case romsmode:
            searchFunction = &romsmode_search;
            downloadFunction = &romsmode_download;
            break;
        case wowroms:
            searchFunction = &wowroms_search;
            downloadFunction = &wowroms_download;
            break;
        case romsdownload:
            searchFunction = &romsdownload_search;
            downloadFunction = &romsdownload_download;
            break;
        case romsemulator:
            searchFunction = &romsemulator_search;
            downloadFunction = &romsemulator_download;
    }
}