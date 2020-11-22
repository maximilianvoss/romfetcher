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
#include "../../download/curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../../helper/md5.h"
#include "../../helper/regex.h"
#include "../urlhandling.h"
#include "../../helper/path.h"
#include "../../download/downloader.h"
#include "../../ui/rendering.h"

#define SHORTNAME "WOW"
#define URL_TEMPLATE "https://wowroms.com/en/roms/list/%system%?search=%query%&page=%page%"
#define URL_PREFIX "https://wowroms.com"

static searchresult_t *search(void *app, system_t *system, char *searchString);

static void download(void *app, searchresult_t *item);

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadServlet(char *response);

static char *fetchHiddenField(char *text, char *fieldname);

static char *fetchDownloadLink(char *response);

static SDL_Texture *loadIcon(void *app);

static engine_t *engine = NULL;

static SDL_Texture *icon = NULL;

engine_t *wowroms_getEngine() {
    if (engine == NULL) {
        engine = calloc(1, sizeof(engine_t));
        engine->search = search;
        engine->download = download;
        engine->loadIcon = loadIcon;
        engine->name = SHORTNAME;
        engine->active = 1;
        engine->fullname = "https://www.wowroms.com";
    }
    return engine;
}

static searchresult_t *search(void *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = URL_TEMPLATE;

    searchresult_t *resultList = NULL;
    do {
        char *url = urlhandling_substitudeVariables(urlTemplate, system, &wowroms_deviceMapping, searchString, page);
        if (url == NULL) {
            break;
        }

        curl_response_t *response = curlling_fetch(url, NULL, GET, 1L);
        resultList = fetchingResultItems(system, resultList, response->data);
        curl_freeResponse(response);
        free(url);

        resultCount = linkedlist_getElementCount(resultList);
        page++;
    } while (resultCount != linkedlist_getElementCount(resultList) && resultCount % 30 != 0);

    return resultList;
}

static void download(void *app, searchresult_t *item) {
    if (item == NULL) {
        return;
    }

    char timestamp[20];
    sprintf(timestamp, "%u", (unsigned) time(NULL));
    char *timestampMD5 = md5_str2md5(timestamp);
    char timeToken[255];
    snprintf(timeToken, 255, "?k=%s&t=%s", timestamp, timestampMD5);

    curl_response_t *detailPageResponse = curlling_fetch(item->url, NULL, GET, 1L);
    char *linkDownloadPageRelative = fetchDownloadPageLink(detailPageResponse->data);
    char *linkDownloadPage = str_concat(URL_PREFIX, linkDownloadPageRelative);

    curl_response_t *downloadPageResponse = curlling_fetch(linkDownloadPage, NULL, GET, 1L);
    char *downloadServletRel = fetchDownloadServlet(downloadPageResponse->data);
    char *downloadServlet = str_concat(URL_PREFIX, downloadServletRel);

    char *filename = fetchHiddenField(downloadPageResponse->data, "file");
    char *id = fetchHiddenField(downloadPageResponse->data, "id");
    char *emuid = fetchHiddenField(downloadPageResponse->data, "emuid");
    char *downloadServletUrl = str_concat(downloadServlet, timeToken);

    curl_response_t *downloadServletResponse = curlling_fetch(downloadServletUrl, "", POST, 1L);
    char *downloadLink = fetchDownloadLink(downloadServletResponse->data);
    char *decodedDownloadLink = str_quoteDecode(downloadLink);

    csafestring_t *payload = safe_create("emuid=");
    safe_strcat(payload, emuid);
    safe_strcat(payload, "&id=");
    safe_strcat(payload, id);
    safe_strcat(payload, "&file=");
    safe_strcat(payload, filename);

    char *decodedFilename = str_urlDecode(filename);
    csafestring_t *downloadPath = path_downloadTarget(item->system, decodedFilename);


    char *downloadFilename = str_concat(item->title, file_suffix(fetchHiddenField(downloadPageResponse->data, "file")));
    downloader_addToQueue(app, item->system, item->title, decodedDownloadLink, payload->data, downloadFilename, POST);
    free(downloadFilename);

    free(decodedFilename);
    free(decodedDownloadLink);
    free(downloadServletUrl);
    curl_freeResponse(downloadServletResponse);
    free(filename);
    free(emuid);
    free(id);
    free(timestampMD5);
    free(downloadServletRel);
    curl_freeResponse(detailPageResponse);
    free(linkDownloadPageRelative);
    free(linkDownloadPage);
    curl_freeResponse(downloadPageResponse);
    free(downloadServlet);
    free(downloadLink);
    safe_destroy(payload);
    safe_destroy(downloadPath);
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
    char *regexString = "<a class=\"[^\"]+\" style=\"[^\"]+\" href=\"([^\"]+)\">Download [^<]+</a>";

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
        searchresult_t *item = result_newItem(system, wowroms_getEngine());

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

static SDL_Texture *loadIcon(void *app) {
    if (icon == NULL) {
        curl_response_t *data = curlling_fetch("https://www.wowroms.com/assets/images/favicon.jpg", NULL, GET, 0L);
        icon = rendering_memImage(app, data->data, data->size);
    }
    return icon;
}