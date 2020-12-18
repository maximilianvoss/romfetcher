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

#include "hoster.h"
#include "../src/application/enginecache/enginecache.h"

cache_t cacheHandler;

void testengines_init(app_t *app) {

    cacheHandler.appData = app;
    cacheHandler.isValid = enginecache_isCacheValid;
    cacheHandler.clear = enginecache_clear;
    cacheHandler.add = enginecache_addEntry;
    cacheHandler.touch = enginecache_updateTimestamp;
    cacheHandler.get = enginecache_getSearchResults;

    hoster_t *hosters = loadHosters(&cacheHandler);

    romhustler = ll_findByName(hosters, "https://romhustler.org");
    freeroms = ll_findByName(hosters, "https://freeroms.com");

//
//    romsmania = ll_findByName(hosters, )romsmania_getEngine();
//    wowroms = wowroms_getEngine();
//    romsdownload = romsdownload_getEngine();
//    romsemulator = romsemulator_getEngine();
//    romhustler = romhustler_getEngine();
//    freeroms = freeroms_getEngine();
}