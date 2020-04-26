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

#include "romsmania.h"
#include "../curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../../systems.h"
#include "mapping.h"

#include <stdio.h>
#include <regex.h>
#include <string.h>

static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response);

static char *fetchDownloadPageLink(char *response);

static char *fetchDownloadLink(char *response);

searchresult_t *romsmania_search(app_t *app, system_t *system, char *searchString) {
    uint32_t resultCount = 0;
    uint32_t page = 1;
    char *urlTemplate = "https://romsmania.cc/roms/%system%/search?name=%query%&genre=&region=&orderBy=name&orderAsc=1&page=%page%";
    char *deviceString = romsmania_deviceMapping(system);

    urlTemplate = str_replace(urlTemplate, "%system%", deviceString);
    urlTemplate = str_replace(urlTemplate, "%query%", searchString);

    searchresult_t *resultList = NULL;
    do {
        resultCount = getResultListCount(resultList);

        char str[11];
        sprintf(str, "%d", page);
        char *url = str_replace(urlTemplate, "%page%", str);

        char *response = fetchURL(url);
        resultList = fetchingResultItems(system, resultList, response);
        free(response);
        page++;
    } while (resultCount != getResultListCount(resultList));

    return resultList;
}

void romsmania_download(app_t *app, searchresult_t *item, void (*callback)(app_t *app)) {
    if (item == NULL) {
        return;
    }
    char *detailPageResponse = fetchURL(item->url);
    char *linkDownloadPage = fetchDownloadPageLink(detailPageResponse);
    free(detailPageResponse);

    char *downloadPageResponse = fetchURL(linkDownloadPage);
    char *linkDownloadArtifact = fetchDownloadLink(downloadPageResponse);
    free(downloadPageResponse);
    free(linkDownloadPage);

    char *filename = file_name(linkDownloadArtifact);
    char *decodedFilename = str_urlDecode(filename);

    char *downloadPath = getRomDownloadPath(item->system, decodedFilename);
    free(decodedFilename);

    downloadURL(app, linkDownloadArtifact, downloadPath);
    free(linkDownloadArtifact);
    free(downloadPath);

    callback(app);
}


static char *fetchDownloadLink(char *response) {
    char *regexString = "<a class=\"wait__link\" href=\"([^[:space:]]+)\">";
    size_t maxGroups = 2;
    char *link = NULL;

    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    char *cursor;

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
        printf("Could not compile regular expression.\n");
        return NULL;
    };

    cursor = response;
    while (1) {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0)) {
            break;  // No more matches
        }

        unsigned int g = 0;
        unsigned int offset = 0;
        for (g = 0; g < maxGroups; g++) {
            if (groupArray[g].rm_so == (size_t) -1) {
                break;  // No more groups
            }

            if (g == 0) {
                offset = groupArray[g].rm_eo;
            }

            char cursorCopy[strlen(cursor) + 1];
            strcpy(cursorCopy, cursor);
            cursorCopy[groupArray[g].rm_eo] = 0;

            if (g == 1) {
                int length = strlen(cursorCopy + groupArray[g].rm_so) + 1;
                link = (char *) malloc(sizeof(char) * length);
                strncpy(link, cursorCopy + groupArray[g].rm_so, length);
            }
        }
        cursor += offset;
    }
    regfree(&regexCompiled);
    return link;
}

static char *fetchDownloadPageLink(char *response) {
    char *regexString = "<a href=\"([^[:space:]]+)\" rel=\"nofollow\" id=\"download_link\" class=\"btn is-with-ico\">";
    size_t maxGroups = 2;
    char *link = NULL;

    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    char *cursor;

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
        printf("Could not compile regular expression.\n");
        return NULL;
    };

    cursor = response;
    while (1) {
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0)) {
            break;  // No more matches
        }

        unsigned int g = 0;
        unsigned int offset = 0;
        for (g = 0; g < maxGroups; g++) {
            if (groupArray[g].rm_so == (size_t) -1) {
                break;  // No more groups
            }

            if (g == 0) {
                offset = groupArray[g].rm_eo;
            }

            char cursorCopy[strlen(cursor) + 1];
            strcpy(cursorCopy, cursor);
            cursorCopy[groupArray[g].rm_eo] = 0;

            if (g == 1) {
                int length = strlen(cursorCopy + groupArray[g].rm_so) + 1;
                link = (char *) malloc(sizeof(char) * length);
                strncpy(link, cursorCopy + groupArray[g].rm_so, length);
            }
        }
        cursor += offset;
    }
    regfree(&regexCompiled);
    return link;
}


static searchresult_t *fetchingResultItems(system_t *system, searchresult_t *resultList, char *response) {
    char *regexString = "<a href=\"([^[:space:]]+)\">([^<]+)</a>";
    size_t maxGroups = 3;

    regex_t regexCompiled;
    regmatch_t groupArray[maxGroups];
    char *cursor;

    if (regcomp(&regexCompiled, regexString, REG_EXTENDED)) {
        printf("Could not compile regular expression.\n");
        return NULL;
    };

    cursor = response;
    while (1) {
        searchresult_t *item = newResultItem(system);
        if (regexec(&regexCompiled, cursor, maxGroups, groupArray, 0)) {
            break;  // No more matches
        }

        unsigned int g = 0;
        unsigned int offset = 0;
        for (g = 0; g < maxGroups; g++) {
            if (groupArray[g].rm_so == (size_t) -1) {
                break;  // No more groups
            }

            if (g == 0) {
                offset = groupArray[g].rm_eo;
            }

            char cursorCopy[strlen(cursor) + 1];
            strcpy(cursorCopy, cursor);
            cursorCopy[groupArray[g].rm_eo] = 0;
            char *title;
            switch (g) {
                case 1:
                    setUrl(item, cursorCopy + groupArray[g].rm_so);
                    break;
                case 2:
                    title = str_htmlDecode(cursorCopy + groupArray[g].rm_so);
                    setTitle(item, title);
                    free(title);
                    break;
                default:
                    break;
            }
        }
        cursor += offset;
        resultList = addResultItemIntoList(resultList, item);
    }
    regfree(&regexCompiled);
    return resultList;
}