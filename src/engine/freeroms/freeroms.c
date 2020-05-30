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
#include <csafestring.h>
#include "freeroms.h"
#include "../../database/enginecache.h"
#include "../urlhandling.h"
#include "mapping.h"
#include "../curlling.h"
#include "../../helper/regex.h"
#include "../enginehandler.h"
#include "../../helper/utils.h"
#include "../../helper/path.h"

#define THREADCOUNT 5
#define URL_TEMPLATE_NUM "https://www.freeroms.com/%system%_roms_NUM.htm"
#define URL_TEMPLATE_CHAR "https://www.freeroms.com/%system%_roms_%query%.htm"
#define URL_TEMPLATE_DOWNLOAD "https://www.freeroms.com/dl_roms/rom_download_tr.php?system=%system%&game_id=%id%"

static void fillCache(app_t *app, system_t *system);

static void *executeThread(void *ptr);

static void extractLink(app_t *app, system_t *system, char *response);

static char *generateDownloadLink(system_t *system, char *id);

struct s_download_filter {
    char start;
    char end;
    app_t *app;
    system_t *system;
};

searchresult_t *freeroms_search(void *app, system_t *system, char *searchString) {
    if (!enginecache_isCacheValid(app, freeroms_shortname(), system)) {
        enginecache_clear(app, freeroms_shortname(), system);
        fillCache(app, system);
        enginecache_updateTimestamp(app, freeroms_shortname(), system);
    }
    return enginecache_getSearchResults(app, enginehandler_findEngine(app, freeroms_shortname()),
                                        system, searchString);
}

void freeroms_download(void *app, searchresult_t *item, void (*callback)(void *app)) {
    if (item == NULL) {
        return;
    }

    char *filename = str_concat(item->title, ".zip");
    csafestring_t *downloadPath = path_downloadTarget(item->system, filename);

    curlling_downloadURL(app, item->url, downloadPath->data);
    free(filename);
    safe_destroy(downloadPath);
    callback(app);
}

char *freeroms_shortname() {
    return "FRE";
}

static void fillCache(app_t *app, system_t *system) {
    pthread_t thread[THREADCOUNT];
    struct s_download_filter filter[THREADCOUNT];

    char chunks = ('Z' - '@') / THREADCOUNT;
    for (int i = 0; i < THREADCOUNT; i++) {
        filter[i].start = (char) '@' + i * chunks;
        filter[i].end = (char) '@' + (i + 1) * chunks - 1;
        filter[i].app = app;
        filter[i].system = system;
    }
    filter[THREADCOUNT - 1].end = 'Z';

    for (int i = 0; i < THREADCOUNT; i++) {
        pthread_create(&thread[i], NULL, executeThread, &filter[i]);
    }
    for (int i = THREADCOUNT - 1; i >= 0; i--) {
        pthread_join(thread[i], NULL);
    }
}


static void *executeThread(void *ptr) {
    struct s_download_filter *filter = (struct s_download_filter *) ptr;

    for (char chr = filter->start; chr <= filter->end; chr++) {
        char *url;
        char *response;
        if (chr == '@') {
            url = urlhandling_substitudeVariables(URL_TEMPLATE_NUM, filter->system, &freeroms_deviceMapping, "", 0);
            if (url == NULL) {
                break;
            }
            response = curlling_fetchURL(url);
            extractLink(filter->app, filter->system, response);
        } else {
            char str[2] = {0, 0};
            str[0] = chr;
            url = urlhandling_substitudeVariables(URL_TEMPLATE_CHAR, filter->system, &freeroms_deviceMapping, str,
                                                  0);
            if (url == NULL) {
                break;
            }
            response = curlling_fetchURL(url);
            extractLink(filter->app, filter->system, response);
        }
        free(response);
        free(url);
    }

    return NULL;
}

static void extractLink(app_t *app, system_t *system, char *response) {
    char *regexString = "<span itemprop=\"name\">([^<]+)[^\']+\'\\/vote.php\\?game_id=([^\']+)";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        enginecache_addEntry(app, freeroms_shortname(), system, ptr->groups[0],
                             generateDownloadLink(system, ptr->groups[1]));
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
}


static char *generateDownloadLink(system_t *system, char *id) {
    char *systemStr = freeroms_deviceMapping(system);
    if (systemStr == NULL) {
        fprintf(stderr, "Found no mapping for system: %s\n", system->fullname);
        return NULL;
    }

    char *result;
    char *tmp;

    result = str_replace(URL_TEMPLATE_DOWNLOAD, "%id%", id);

    tmp = result;
    result = str_replace(tmp, "%system%", systemStr);
    free(tmp);

    return result;
}