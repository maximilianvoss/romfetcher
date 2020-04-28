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

#include "romsemulator.h"
#include "mapping.h"
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../../download/utils.h"
#include "../urlhandling.h"
#include "../../helper/regex.h"

#define URL_TEMPLATE "https://romsemulator.net/roms/%system%/page/%page%/?s=%query%"

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadLink(char *response);

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

        resultCount = getResultListCount(resultList);

        char *response = fetchURL(url);
        resultList = fetchingResultItems(system, resultList, response);
        free(response);
        free(url);

        page++;
    } while (resultCount != getResultListCount(resultList) && resultCount % 10 == 0);

    return resultList;
}

void romsemulator_download(app_t *app, searchresult_t *item, void (*callback)(app_t *app)) {
    if (item == NULL) {
        return;
    }
    char *detailPageResponse = fetchURL(item->url);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse);

    char *downloadPageResponse = fetchURL(linkDownloadPage);

    char *pid = fetchHiddenField(downloadPageResponse, "pid", 0);
    char *roms = fetchHiddenField(downloadPageResponse, "roms_download_file_nonce_field", 1);
    char *referer = fetchHiddenField(downloadPageResponse, "_wp_http_referer", 0);

    char *payload = str_concat("action=roms_download_file&pid=", pid);
    char *tmp = payload;
    payload = str_concat(payload, "&roms_download_file_nonce_field=");
    free(tmp);
    tmp = payload;
    payload = str_concat(payload, roms);
    free(tmp);
    tmp = payload;
    payload = str_concat(payload, "&_wp_http_referer=");
    free(tmp);
    tmp = payload;
    payload = str_concat(payload, referer);
    free(tmp);

    char *filename = calloc(sizeof(char), strlen(item->url) + 4);
    strcpy(filename, item->url);
    printf("old name: %s\n", filename);
    tmp = filename;
    while (*tmp != '\0') {
        tmp++;
    }
    tmp--;
    strcpy(tmp, ".zip");

    char *decodeFilename = file_name(filename);
    char *downloadPath = download_targetPath(item->system, decodeFilename);

    downloadURLPost(app, linkDownloadPage, payload, downloadPath);

    free(pid);
    free(roms);
    free(referer);
    free(payload);
    free(filename);
    free(downloadPageResponse);
    free(linkDownloadPage);
    free(detailPageResponse);
    free(downloadPath);

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

static char *fetchDownloadLink(char *response) {
    char *regexString = "<a class=\"wait__link\" href=\"([^\"]+)\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
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
        searchresult_t *item = newResultItem(system);
        item->system = system;
        setUrl(item, ptr->groups[0]);

        char *title = str_htmlDecode(ptr->groups[1]);
        setTitle(item, title);
        free(title);

        resultList = addResultItemIntoList(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}