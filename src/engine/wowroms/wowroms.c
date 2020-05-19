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
#include "wowroms.h"
#include "mapping.h"
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../../helper/md5.h"
#include "../../helper/regex.h"
#include "../urlhandling.h"
#include "../../helper/path.h"
#include "../enginehandler.h"

#define URL_TEMPLATE "https://wowroms.com/en/roms/list/%system%?search=%query%&page=%page%"
#define URL_PREFIX "https://wowroms.com"

static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadServlet(char *response);

static char *fetchHiddenField(char *text, char *fieldname);

static char *fetchDownloadLink(char *response);

searchresult_t *wowroms_search(void *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &wowroms_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        char *response = curlling_fetchURL(url);
        resultList = fetchingResultItems(app, system, resultList, response);
        free(response);
        free(url);

        resultCount = linkedlist_getElementCount(resultList);
        page++;
    } while (resultCount != linkedlist_getElementCount(resultList) && resultCount % 30 != 0);

    return resultList;
}

void wowroms_download(void *app, searchresult_t *item, void (*callback)(void *app)) {
    if (item == NULL) {
        return;
    }

    char timestamp[20];
    sprintf(timestamp, "%u", (unsigned) time(NULL));
    char *timestampMD5 = md5_str2md5(timestamp);
    char timeToken[255];
    snprintf(timeToken, 255, "?k=%s&t=%s", timestamp, timestampMD5);

    char *detailPageResponse = curlling_fetchURL(item->url);
    char *linkDownloadPageRelative = fetchDownloadPageLink(detailPageResponse);
    char *linkDownloadPage = str_concat(URL_PREFIX, linkDownloadPageRelative);

    char *downloadPageResponse = curlling_fetchURL(linkDownloadPage);
    char *downloadServletRel = fetchDownloadServlet(downloadPageResponse);
    char *downloadServlet = str_concat(URL_PREFIX, downloadServletRel);

    char *filename = fetchHiddenField(downloadPageResponse, "file");
    char *id = fetchHiddenField(downloadPageResponse, "id");
    char *emuid = fetchHiddenField(downloadPageResponse, "emuid");
    char *downloadServletUrl = str_concat(downloadServlet, timeToken);

    char *downloadServletResponse = curlling_fetchURLPost(downloadServletUrl, "");
    char *downloadLink = fetchDownloadLink(downloadServletResponse);
    char *decodedDownloadLink = str_quoteDecode(downloadLink);


    csafestring_t *payload = safe_create("emuid=");
    safe_strcat(payload, emuid);
    safe_strcat(payload, "&id=");
    safe_strcat(payload, id);
    safe_strcat(payload, "&file=");
    safe_strcat(payload, filename);

    char *decodedFilename = str_urlDecode(filename);
    csafestring_t *downloadPath = path_downloadTarget(item->system, decodedFilename);

    curlling_downloadURLPost(app, decodedDownloadLink, payload->data, downloadPath->data);

    free(decodedFilename);
    free(decodedDownloadLink);
    free(downloadServletUrl);
    free(downloadServletResponse);
    free(filename);
    free(emuid);
    free(id);
    free(timestampMD5);
    free(downloadServletRel);
    free(detailPageResponse);
    free(linkDownloadPageRelative);
    free(linkDownloadPage);
    free(downloadPageResponse);
    free(downloadServlet);
    free(downloadLink);
    safe_destroy(payload);
    safe_destroy(downloadPath);

    callback(app);
}

char *wowroms_shortname() {
    return "WOW";
}

static char *fetchHiddenField(char *response, char *fieldname) {
    char *regexStringTmpl = "<input type=\"hidden\" name=\"%fieldname%\" value=\"([^\"]+)\" />";
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

static char *fetchDownloadServlet(char *response) {
    char *regexString = "var ajaxLinkUrl = \"([^\"]+)\";";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static char *fetchDownloadLink(char *response) {
    char *regexString = "\"link\":\"([^\"]+)\"";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static char *fetchDownloadPageLink(char *response) {
    char *regexString = "<a class=\"btn btn-prev col-md-24 btnDwn\" style=\"font-size: 16px !important;padding-top: 10px;color:#fbfbfb !important\" href=\"([^\"]+)\">";

    regexMatches_t *matches = regex_getMatches(response, regexString, 1);
    if (matches == NULL) {
        return NULL;
    }
    char *link = regex_cpyGroupText(matches, 0);
    regex_destroyMatches(matches);
    return link;
}

static searchresult_t *fetchingResultItems(app_t *app, system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<a class=\"title-5 heighta\" title=\"([^\"]+)\" href=\"([^\"]+)\">[^<]+</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = result_newItem(system, enginehandler_findEngine(app, wowroms_shortname()));

        char *title = str_htmlDecode(ptr->groups[0]);
        result_setTitle(item, title);
        free(title);

        char *url = str_concat(URL_PREFIX, ptr->groups[1]);
        result_setUrl(item, url);
        free(url);

        resultList = linkedlist_appendElement(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}