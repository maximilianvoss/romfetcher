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
#include "progameroms.h"
#include "../../database/enginecache.h"
#include "../urlhandling.h"
#include "mapping.h"
#include "../curlling.h"
#include "../../helper/regex.h"
#include "../enginehandler.h"
#include "../../helper/utils.h"
#include "../../helper/path.h"

#define URL_TEMPLATE_NUM "https://progameroms.com/%system%/Numbers/Numbers.html"
#define URL_TEMPLATE_CHAR "https://progameroms.com/%system%/%query%/%query%_section.html"

static void fillCache(app_t *app, system_t *system);

static void *executeThread(void *ptr);

static void extractLink(app_t *app, system_t *system, char *response);

struct s_download_filter {
    char start;
    char end;
    app_t *app;
    system_t *system;
};

searchresult_t *progameroms_search(void *app, system_t *system, char *searchString) {
    if (!enginecache_isCacheValid(app, progameroms_shortname(), system)) {
        enginecache_clear(app, progameroms_shortname(), system);
        fillCache(app, system);
        enginecache_updateTimestamp(app, progameroms_shortname(), system);
    }
    return enginecache_getSearchResults(app, enginehandler_findEngine(app, progameroms_shortname()),
                                        system, searchString);
}

void progameroms_download(void *app, searchresult_t *item, void (*callback)(void *app)) {
    if (item == NULL) {
        return;
    }
    char *filename = file_name(item->url);
    csafestring_t *downloadPath = path_downloadTarget(item->system, filename);

    curlling_downloadURL(app, item->url, downloadPath->data);
    safe_destroy(downloadPath);
    callback(app);
}

char *progameroms_shortname() {
    return "PRO";
}


static void fillCache(app_t *app, system_t *system) {
    pthread_t thread[3];
    struct s_download_filter filter[3];
    filter[0].start = '@';
    filter[0].end = 'I';
    filter[0].app = app;
    filter[0].system = system;
    filter[1].start = 'J';
    filter[1].end = 'S';
    filter[1].app = app;
    filter[1].system = system;
    filter[2].start = 'T';
    filter[2].end = 'Z';
    filter[2].app = app;
    filter[2].system = system;

    for (int i = 0; i < 3; i++) {
        pthread_create(&thread[i], NULL, executeThread, &filter[i]);
    }
    for (int i = 2; i >= 0; i--) {
        pthread_join(thread[i], NULL);
    }
}


static void *executeThread(void *ptr) {
    struct s_download_filter *filter = (struct s_download_filter *) ptr;

    for (char chr = filter->start; chr <= filter->end; chr++) {
        char *url;
        char *response;
        if (chr == '@') {
            url = urlhandling_substitudeVariables(URL_TEMPLATE_NUM, filter->system, &progameroms_deviceMapping, "", 0);
            response = curlling_fetchURL(url);
            extractLink(filter->app, filter->system, response);
        } else {
            char str[2] = {0, 0};
            str[0] = chr;
            url = urlhandling_substitudeVariables(URL_TEMPLATE_CHAR, filter->system, &progameroms_deviceMapping, str,
                                                  0);
            response = curlling_fetchURL(url);
            extractLink(filter->app, filter->system, response);
        }
        free(response);
        free(url);
    }

    return NULL;
}

static void extractLink(app_t *app, system_t *system, char *response) {
    char *regexString = "<a href=\"([^\"]+.rar)\" target=\"_blank\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        enginecache_addEntry(app, progameroms_shortname(), system, ptr->groups[1], ptr->groups[0]);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
}