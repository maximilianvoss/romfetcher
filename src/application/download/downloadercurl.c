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
#include "downloadercurl.h"
#include "../helper/path.h"
#include "../../common/utils.h"
#include "../database/postprocess.h"

static char *postProcess(app_t *app, char *file);

void downloadercurl_init(app_t *app) {}

void downloadercurl_destroy(app_t *app) {}

void downloadercurl_addToQueue(app_t *app, system_t *systemt, char *title, char *url, char *data, char *filename,
                               httpmethod_t method) {
    csafestring_t *fullCmd = safe_create("$(nohup ");

    csafestring_t *downloadPath = path_downloadTarget(systemt, filename);
    csafestring_t *curlCmd = safe_create("curl -s -X ");
    if (method == GET) {
        safe_strcat(curlCmd, "GET ");
    } else {
        safe_strcat(curlCmd, "POST ");
    }

    if (data != NULL) {
        safe_strcat(curlCmd, "--data-raw \"");
        safe_strcat(curlCmd, data);
        safe_strcat(curlCmd, "\" ");
    }

    safe_strcat(curlCmd,
                "-H \"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36)\" ");

    safe_strcat(curlCmd, "-g ");
    safe_strcat(curlCmd, "-o \"");
    safe_strcat(curlCmd, downloadPath->data);
    safe_strcat(curlCmd, "\" ");

    safe_strcat(curlCmd, "\"");
    safe_strcat(curlCmd, url);
    safe_strcat(curlCmd, "\" ");

    LOG_DEBUG("CURL Command: %s", curlCmd->data);
    safe_strcat(fullCmd, curlCmd->data);
    safe_destroy(curlCmd);

    char *postCmd = postProcess(app, downloadPath->data);
    if (postCmd != NULL) {
        LOG_DEBUG("Post Process Command: %s", postCmd);
        safe_strcat(fullCmd, " && ");
        safe_strcat(fullCmd, postCmd);

    }
    FREENOTNULL(postCmd);

    safe_strcat(fullCmd, ") &");
    LOG_DEBUG("Full download command: %s", fullCmd->data);

    system(fullCmd->data);

    safe_destroy(fullCmd);
}

void downloadercurl_cancel(app_t *app, download_t *download) {}

uint8_t downloadercurl_isActive(app_t *app) {
    return 0;
}

void downloadercurl_cancelAllDownloads(app_t *app) {}


static char *postProcess(app_t *app, char *file) {
    char *suffix = file_suffix(file);
    char *command = NULL;

    char *commandTemplate = databasepostprocess_getCommand(app, suffix);
    if (commandTemplate != NULL) {
        char *directory = file_parentDirectory(file);
        char *tmp = str_replace(commandTemplate, "%file%", file);
        command = str_replace(tmp, "%target%", directory);

        free(directory);
        free(commandTemplate);
        free(tmp);
    }
    return command;
}