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
#include "romsemulator.h"
#include "mapping.h"
#include "../../download/curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../urlhandling.h"
#include "../../helper/regex.h"
#include "../../download/downloader.h"

#define SHORTNAME "REN"
#define URL_TEMPLATE "https://romsemulator.net/roms/%system%/page/%page%/?s=%query%"

static searchresult_t *search(void *app, system_t *system, char *searchString);

static void download(void *app, searchresult_t *item);

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchHiddenField(char *response, char *fieldname, int variant);

static engine_t *engine = NULL;

engine_t *romsemulator_getEngine() {
    if (engine == NULL) {
        engine = calloc(1, sizeof(engine_t));
        engine->search = search;
        engine->download = download;
        engine->name = SHORTNAME;
        engine->active = 0;
        engine->fullname = "https://www.romsemulator.net";
    }
    return engine;
}

static searchresult_t *search(void *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &romsemulator_deviceMapping, searchString,
                                                    page);
        if (url == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(url, NULL, GET, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);
        curl_freeResponse(response);
        free(url);

        resultCount = linkedlist_getElementCount(resultList);
        page++;
    } while (resultCount != linkedlist_getElementCount(resultList) && resultCount % 10 == 0);

    return resultList;
}

static void download(void *app, searchresult_t *item) {
    if (item == NULL) {
        return;
    }
    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse->data);

    curl_response_t *downloadPageResponse = curlling_fetch(linkDownloadPage, NULL, GET, 1L);

    char *pid = fetchHiddenField(downloadPageResponse->data, "pid", 0);
    char *roms = fetchHiddenField(downloadPageResponse->data, "roms_download_file_nonce_field", 1);
    char *referer = fetchHiddenField(downloadPageResponse->data, "_wp_http_referer", 0);

    csafestring_t *payload = safe_create("action=roms_download_file&pid=");
    safe_strcat(payload, pid);
    safe_strcat(payload, "&roms_download_file_nonce_field=");
    safe_strcat(payload, roms);
    safe_strcat(payload, "&_wp_http_referer=");
    safe_strcat(payload, referer);

    char *filename = str_concat(item->title, ".zip");
    downloader_addToQueue(app, item->system, item->title, linkDownloadPage, payload->data, filename, POST);
    free(filename);

    free(pid);
    free(roms);
    free(referer);
    curl_freeResponse(downloadPageResponse);
    free(linkDownloadPage);
    curl_freeResponse(detailPageResponse);
    safe_destroy(payload);
}

static char *fetchHiddenField(char *response, char *fieldname, int variant) {
    char *regexStringTmpl;
    if (variant == 0) {
        regexStringTmpl = "<input type=\"[^\"]+\" name=\"%fieldname%\" value=\"([^\"]+)\"";
    } else {
        regexStringTmpl = "<input type=\"[^\"]+\" id=\"[^\"]+\" name=\"%fieldname%\" value=\"([^\"]+)\"";
    };
    char *regexString = str_replace(regexStringTmpl, "%fieldname%", fieldname);

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    free(regexString);
    return link;
}

static char *fetchDownloadPageLink(char *response) {
    char *regexString = "<a class=\"download\" href=\"([^\"]+)\" rel=\"nofollow\" id=\"download_link\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<td><a href=\"([^\"]+)\">([^<]+)</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = result_newItem(system, romsemulator_getEngine());
        result_setUrl(item, ptr->groups[0]);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = linkedlist_appendElement(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}