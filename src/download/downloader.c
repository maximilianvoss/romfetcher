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
#include "downloader.h"
#include "curlling.h"
#include "../helper/path.h"
#include "../helper/utils.h"
#include "../state/statesearch.h"
#include "../state/statedownload.h"
#include "../database/postprocess.h"

typedef struct {
    app_t *app;
    system_t *system;
    char *url;
    char *data;
    char *filename;
    httpmethod_t method;

    void (*callback)(void *app);
} data_t;

static void performDownload(app_t *app, system_t *system, char *url, char *data, char *filename, httpmethod_t method,
                            void (*callback)(void *app));

static void modalDownload(void *data);

static void modalCancel(void *data);

static void postProcess(app_t *app, char *file);

void downloader_init(app_t *app) {

}

void downloader_download(app_t *app, system_t *system, char *url, char *data, char *filename, httpmethod_t method,
                         void (*callback)(void *app)) {

    csafestring_t *downloadPath = path_downloadTarget(system, filename);
    if (file_exists(downloadPath->data)) {
        data_t *input = calloc(1, sizeof(data_t));
        input->app = app;
        input->system = system;
        input->method = method;
        input->callback = callback;
        input->url = str_clone(url);
        input->data = str_clone(data);
        input->filename = str_clone(filename);

        app->modal.displayed = 1;
        app->modal.headline = "File already exists";
        app->modal.text = "The file already exists, do you want to overwrite it?";
        app->modal.actionButton = "Yes";
        app->modal.cancelButton = "No";
        app->modal.cursorPos = 0;
        app->modal.callbackData = input;
        app->modal.callbackAction = &modalDownload;
        app->modal.callbackCancel = &modalCancel;
    } else {
        performDownload(app, system, url, data, filename, method, callback);
    }
    safe_destroy(downloadPath);
}

static void modalDownload(void *data) {
    data_t *input = (data_t *) data;
    input->app->modal.displayed = 0;
    performDownload(input->app, input->system, input->url, input->data, input->filename, input->method,
                    input->callback);
    FREENOTNULL(input->filename);
    FREENOTNULL(input->url);
    FREENOTNULL(input->data);
    free(input);
}

static void modalCancel(void *data) {
    data_t *input = (data_t *) data;
    input->app->modal.displayed = 0;
    statedownload_persist(input->app);
    statesearch_init(input->app);
    FREENOTNULL(input->filename);
    FREENOTNULL(input->url);
    FREENOTNULL(input->data);
    free(input);
}

static void performDownload(app_t *app, system_t *system, char *url, char *data, char *filename, httpmethod_t method,
                            void (*callback)(void *app)) {
    csafestring_t *downloadPath = path_downloadTarget(system, filename);
    curlling_download(app, url, data, method, downloadPath->data);

    postProcess(app, downloadPath->data);

    safe_destroy(downloadPath);
    callback(app);
}

static void postProcess(app_t *app, char *file) {
    char *suffix = file_suffix(file);

    char *commandTemplate = databasepostprocess_getCommand(app, suffix);
    if (commandTemplate != NULL) {
        char *directory = file_parentDirectory(file);
        char *tmp = str_replace(commandTemplate, "%file%", file);
        char *command = str_replace(tmp, "%target%", directory);

        int status = system(command);
        SDL_Log("%s exited with: %d\n", command, status);

        free(directory);
        free(commandTemplate);
        free(command);
        free(tmp);
    }
}