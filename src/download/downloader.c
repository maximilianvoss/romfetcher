/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
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
#include <unistd.h>
#include "downloader.h"
#include "../config/config.h"
#include "../helper/path.h"
#include "postprocess.h"
#include "../helper/utils.h"

static void spawnThreads(app_t *app);

static void *downloadThreadExecution(void *appPtr);

static void modalDownload(void *app, void *data);

static void addDownload(app_t *app, download_t *download);

static void destroyDownload(void *ptr);

static int filterQueuedDownload(void *payload, void *input);

static int filterActiveDownload(void *payload, void *input);

static int downloadComparator(void *payload1, void *payload2);

static inline uint8_t downloadIsQueued(download_t *download);

static void downloadCurl(app_t *app, download_t *download);

static void downloadInternal(app_t *app, download_t *download);

static char *postProcess(app_t *app, char *file, uint8_t noExecute);

pthread_mutex_t mutexQueue;

void downloader_init(app_t *app) {
    if (pthread_mutex_init(&mutexQueue, NULL) != 0) {
        LOG_ERROR("Mutex init failed");
        exit(1);
    }
    spawnThreads(app);
}

void downloader_destroy(app_t *app) {
    pthread_mutex_lock(&mutexQueue);

    acll_free(app->download.all, &destroyDownload);
    app->download.all = NULL;

    pthread_mutex_unlock(&mutexQueue);
    pthread_mutex_destroy(&mutexQueue);
}

uint8_t downloader_addToQueue(void *appPtr, rl_system *system, char *title, char *url, char *data, char *filename,
                              chttp_method method) {
    app_t *app = appPtr;
    download_t *download = calloc(1, sizeof(download_t));
    download->title = str_clone(title);
    download->active = 0;
    download->system = system;
    download->url = str_clone(url);
    download->data = str_clone(data);
    download->filename = str_clone(filename);
    download->method = method;

    csafestring_t *downloadPath = path_downloadTarget(system, filename);
    if (file_exists(downloadPath->data)) {
        app->modal.displayed = 1;
        app->modal.headline = "File already exists";
        app->modal.text = "The file already exists, do you want to overwrite it?";
        app->modal.actionButton = "Yes";
        app->modal.cancelButton = "No";
        app->modal.cursorPos = 0;
        app->modal.app = app;
        app->modal.callbackData = download;
        app->modal.callbackAction = &modalDownload;
        app->modal.callbackCancel = NULL;
    } else {
        addDownload(app, download);
    }
    safe_destroy(downloadPath);
    return 0;
}

void downloader_cancel(app_t *app, acll_t *element) {
    if (element == NULL) {
        return;
    }
    download_t *download = getDownload(element);
    download->cancelled = 1;
    download->active = 0;

    pthread_mutex_lock(&mutexQueue);
    app->download.all = acll_sort(app->download.all, downloadComparator);
    pthread_mutex_unlock(&mutexQueue);
}

uint8_t downloader_isActive(app_t *app) {
    return (acll_find(app->download.all, filterActiveDownload, NULL) != NULL);
}

void downloader_cancelAllDownloads(app_t *app) {
    pthread_mutex_lock(&mutexQueue);
    acll_t *element = acll_first(app->download.all);
    while (element != NULL) {
        getDownload(element)->cancelled = 1;
        getDownload(element)->active = 0;
        element = element->next;
    }
    pthread_mutex_unlock(&mutexQueue);
}

static void spawnThreads(app_t *app) {
    pthread_t downloadThreads[DOWNLOADER_THREADS];
    for (int i = 0; i < DOWNLOADER_THREADS; i++) {
        if (pthread_create(&downloadThreads[i], NULL, downloadThreadExecution, app)) {
            LOG_ERROR("Error creating thread");
            return;
        }
        pthread_detach(downloadThreads[i]);
    }
}

static void *downloadThreadExecution(void *appPtr) {
    app_t *app = (app_t *) appPtr;
    while (!app->quit) {
        pthread_mutex_lock(&mutexQueue);

        acll_t *element = acll_firstFilter(app->download.all, filterQueuedDownload, NULL);
        if (element != NULL) {
            getDownload(element)->active = 1;
        }
        pthread_mutex_unlock(&mutexQueue);

        if (element != NULL) {
            if (config_get(app->config.advanced.active, advancedConfig_externalCurl)) {
                downloadCurl(app, getDownload(element));
            } else {
                downloadInternal(app, getDownload(element));
            }
            getDownload(element)->finished = 1;
            getDownload(element)->active = 0;

            pthread_mutex_lock(&mutexQueue);
            app->download.all = acll_sort(app->download.all, downloadComparator);
            pthread_mutex_unlock(&mutexQueue);
        }
        sleep(3);
    }
    return NULL;
}

static void modalDownload(void *app, void *data) {
    ((app_t *) app)->modal.displayed = 0;
    addDownload(app, data);
}

static void addDownload(app_t *app, download_t *download) {
    pthread_mutex_lock(&mutexQueue);
    app->download.all = acll_append(app->download.all, download);
    app->download.all = acll_sort(app->download.all, downloadComparator);
    pthread_mutex_unlock(&mutexQueue);
}

static void destroyDownload(void *ptr) {
    download_t *download = (download_t *) ptr;
    FREENOTNULL(download->title);
    FREENOTNULL(download->url);
    FREENOTNULL(download->data);
    FREENOTNULL(download->filename);
}

static int filterQueuedDownload(void *payload, void *input) {
    download_t *download = payload;
    (void) input;
    return (!download->active && !download->cancelled && !download->finished);
}

static int filterActiveDownload(void *payload, void *input) {
    download_t *download = payload;
    (void) input;
    return download->active;
}

static int downloadComparator(void *payload1, void *payload2) {
    download_t *download1 = payload1;
    download_t *download2 = payload2;

    // active
    if (download1->active && download2->active) {
        return 0;
    }
    if (download1->active) {
        return -1;
    }
    if (!download1->active) {
        return 1;
    }

    // queued
    if (downloadIsQueued(download1) && downloadIsQueued(download2)) {
        return 0;
    }
    if (downloadIsQueued(download1)) {
        return -1;
    }
    if (downloadIsQueued(download2)) {
        return 1;
    }

    // finished
    if (download1->finished && download2->finished) {
        return 0;
    }
    if (download1->finished) {
        return -1;
    }
    if (download2->finished) {
        return 1;
    }

    // cancelled
    if (download1->cancelled && download2->cancelled) {
        return 0;
    }
    if (download1->cancelled) {
        return -1;
    }
    if (download2->cancelled) {
        return 1;
    }
    LOG_ERROR("This end of function should never be reached");
    return 0;
}

static inline uint8_t downloadIsQueued(download_t *download) {
    if (!download->finished && !download->cancelled && !download->active) {
        return 1;
    }
    return 0;
}

static void downloadCurl(app_t *app, download_t *download) {
    csafestring_t *fullCmd = safe_create("$(nohup ");

    csafestring_t *downloadPath = path_downloadTarget(download->system, download->filename);
    csafestring_t *curlCmd = safe_create("curl -s -X ");
    if (download->method == GET) {
        safe_strcat(curlCmd, "GET ");
    } else {
        safe_strcat(curlCmd, "POST ");
    }

    if (download->data != NULL) {
        safe_strcat(curlCmd, "--data-raw \"");
        safe_strcat(curlCmd, download->data);
        safe_strcat(curlCmd, "\" ");
    }

    safe_strcat(curlCmd,
                "-H \"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36)\" ");

    safe_strcat(curlCmd, "-g ");
    safe_strcat(curlCmd, "-o \"");
    safe_strcat(curlCmd, downloadPath->data);
    safe_strcat(curlCmd, "\" ");

    safe_strcat(curlCmd, "\"");
    safe_strcat(curlCmd, download->url);
    safe_strcat(curlCmd, "\" ");

    LOG_DEBUG("CURL Command: %s", curlCmd->data);
    safe_strcat(fullCmd, curlCmd->data);
    safe_destroy(curlCmd);

    char *postCmd = postProcess(app, downloadPath->data, 1);
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

static void downloadInternal(app_t *app, download_t *download) {
    csafestring_t *downloadPath = path_downloadTarget(download->system, download->filename);
    chttp_download(download->url, download->data, download->method, downloadPath->data, &download->current,
                   &download->total, &download->cancelled);

    if (download->cancelled) {
        LOG_DEBUG("Deleting cancelled file: %s", downloadPath->data);
        unlink(downloadPath->data);
        safe_destroy(downloadPath);
        return;
    }
    postProcess(app, downloadPath->data, 0);
    safe_destroy(downloadPath);
}

static char *postProcess(app_t *app, char *file, uint8_t noExecute) {
    char *suffix = file_suffix(file);
    char *command = NULL;

    char *commandTemplate = databasepostprocess_getCommand(app, suffix);
    if (commandTemplate != NULL) {
        char *directory = file_parentDirectory(file);
        char *tmp = str_replace(commandTemplate, "%file%", file);
        command = str_replace(tmp, "%target%", directory);

        if (noExecute) {
            int status = system(command);
            LOG_INFO("%s exited with: %d", command, status);
            free(command);
            command = NULL;
        }

        free(directory);
        free(commandTemplate);
        free(tmp);
    }
    return command;
}