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

#include "wowroms.h"
#include "mapping.h"
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../../download/utils.h"
#include "../../helper/md5.h"
#include "../../helper/regex.h"
#include "../urlhandling.h"

#define URL_TEMPLATE "https://wowroms.com/en/roms/list/%system%?search=%query%&page=%page%"
#define URL_PREFIX "https://wowroms.com"

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadServlet(char *response);

static char *fetchHiddenField(char *text, char *fieldname);

static char *fetchDownloadLink(char *response);

searchresult_t *wowroms_search(app_t *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &wowroms_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        resultCount = getResultListCount(resultList);

        char *response = fetchURL(url);
        resultList = fetchingResultItems(system, resultList, response);
        free(response);
        free(url);

        page++;
    } while (resultCount != getResultListCount(resultList) && resultCount % 30 != 0);

    return resultList;
}

void wowroms_download(app_t *app, searchresult_t *item, void (*callback)(app_t *app)) {
    if (item == NULL) {
        return;
    }

    char timestamp[20];
    sprintf(timestamp, "%u", (unsigned) time(NULL));
    char *timestampMD5 = md5_str2md5(timestamp);
    char timeToken[255];
    snprintf(timeToken, 255, "?k=%s&t=%s", timestamp, timestampMD5);

    char *detailPageResponse = fetchURL(item->url);
    char *linkDownloadPageRelative = fetchDownloadPageLink(detailPageResponse);
    char *linkDownloadPage = str_concat(URL_PREFIX, linkDownloadPageRelative);

    char *downloadPageResponse = fetchURL(linkDownloadPage);
    char *downloadServletRel = fetchDownloadServlet(downloadPageResponse);
    char *downloadServlet = str_concat(URL_PREFIX, downloadServletRel);

    char *filename = fetchHiddenField(downloadPageResponse, "file");
    char *id = fetchHiddenField(downloadPageResponse, "id");
    char *emuid = fetchHiddenField(downloadPageResponse, "emuid");
    char *downloadServletUrl = str_concat(downloadServlet, timeToken);

    char *downloadServletResponse = fetchURLPost(downloadServletUrl, "");
    char *downloadLink = fetchDownloadLink(downloadServletResponse);
    char *decodedDownloadLink = str_quoteDecode(downloadLink);

    char *payload = str_concat("emuid=", emuid);
    char *tmp = payload;
    payload = str_concat(payload, "&id=");
    free(tmp);
    tmp = payload;
    payload = str_concat(payload, id);
    free(tmp);
    tmp = payload;
    payload = str_concat(payload, "&file=");
    free(tmp);
    tmp = payload;
    payload = str_concat(payload, filename);
    free(tmp);

    char *decodedFilename = str_urlDecode(filename);
    char *downloadPath = download_targetPath(item->system, decodedFilename);

    downloadURLPost(app, decodedDownloadLink, payload, downloadPath);

    free(decodedFilename);
    free(downloadPath);
    free(payload);
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

    callback(app);
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

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<a class=\"title-5 heighta\" title=\"([^\"]+)\" href=\"([^\"]+)\">[^<]+</a>";

    regexMatches_t *matches = regex_getMatches(response, regexString, 2);
    regexMatches_t *ptr = matches;

    while (ptr != NULL) {
        searchresult_t *item = newResultItem(system);
        item->system = system;

        char *title = str_htmlDecode(ptr->groups[0]);
        setTitle(item, title);
        free(title);

        char *url = str_concat(URL_PREFIX, ptr->groups[1]);
        setUrl(item, url);
        free(url);

        resultList = addResultItemIntoList(resultList, item);
        ptr = ptr->next;
    }
    regex_destroyMatches(matches);
    return resultList;
}