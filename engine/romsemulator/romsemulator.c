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
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../urlhandling.h"
#include "../../helper/regex.h"
#include "../../helper/path.h"

#define URL_TEMPLATE "https://romsemulator.net/roms/%system%/page/%page%/?s=%query%"

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchHiddenField(char *response, char *fieldname, int variant);

searchresult_t *romsemulator_search(app_t *app, system_t *system, char *searchString) {
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

        resultCount = result_getListCount(resultList);

        char *response = curlling_fetchURL(url);
        resultList = fetchingResultItems(system, resultList, response);
        free(response);
        free(url);

        page++;
    } while (resultCount != result_getListCount(resultList) && resultCount % 10 == 0);

    return resultList;
}

void romsemulator_download(app_t *app, searchresult_t *item, void (*callback)(app_t *app)) {
    if (item == NULL) {
        return;
    }
    char *detailPageResponse = curlling_fetchURL(item->url);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse);

    char *downloadPageResponse = curlling_fetchURL(linkDownloadPage);

    char *pid = fetchHiddenField(downloadPageResponse, "pid", 0);
    char *roms = fetchHiddenField(downloadPageResponse, "roms_download_file_nonce_field", 1);
    char *referer = fetchHiddenField(downloadPageResponse, "_wp_http_referer", 0);

    csafestring_t *payload = safe_create("action=roms_download_file&pid=");
    safe_strcat(payload, pid);
    safe_strcat(payload, "&roms_download_file_nonce_field=");
    safe_strcat(payload, roms);
    safe_strcat(payload, "&_wp_http_referer=");
    safe_strcat(payload, referer);

    csafestring_t *filename = safe_create(item->url);
    safe_strcat(filename, ".zip");

    char *decodeFilename = file_name(filename->data);
    csafestring_t *downloadPath = path_downloadTarget(item->system, decodeFilename);

    curlling_downloadURLPost(app, linkDownloadPage, payload->data, downloadPath->data);

    free(pid);
    free(roms);
    free(referer);
    free(downloadPageResponse);
    free(linkDownloadPage);
    free(detailPageResponse);
    safe_destroy(filename);
    safe_destroy(payload);
    safe_destroy(downloadPath);

    callback(app);
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
        searchresult_t *item = result_newItem(system);
        item->system = system;
        result_setUrl(item, ptr->groups[0]);

        char *title = str_htmlDecode(ptr->groups[1]);
        result_setTitle(item, title);
        free(title);

        resultList = result_addItemToList(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}