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
#include <semaphore.h>
#include <pthread.h>
#include <zconf.h>
#include "downloader.h"
#include "curlling.h"
#include "../helper/path.h"
#include "../helper/utils.h"
#include "../state/statesearch.h"
#include "../state/statedownload.h"
#include "../database/postprocess.h"
#include "../config.h"

static void destroyDownload(void *ptr);

static void addDownload(app_t *app, download_t *download);

static void modalDownload(void *app, void *data);

static void modalCancel(void *app, void *data);

static void postProcess(app_t *app, char *file);

static void *downloadThreadExecution(void *appPtr);

static void spawnThreads(app_t *app);

pthread_mutex_t lockQueue;
pthread_mutex_t lockActive;
pthread_mutex_t lockDone;

void downloader_init(app_t *app) {
    if (pthread_mutex_init(&lockActive, NULL) != 0) {
        fprintf(stderr, "Mutex init failed\n");
        exit(1);
    }
    if (pthread_mutex_init(&lockQueue, NULL) != 0) {
        fprintf(stderr, "Mutex init failed\n");
        exit(1);
    }
    if (pthread_mutex_init(&lockDone, NULL) != 0) {
        fprintf(stderr, "Mutex init failed\n");
        exit(1);
    }
    spawnThreads(app);
}

void downloader_destroy(app_t *app) {
    pthread_mutex_lock(&lockActive);
    pthread_mutex_lock(&lockQueue);
    pthread_mutex_lock(&lockDone);

    linkedlist_freeList(app->download.active, &destroyDownload);
    app->download.active = NULL;
    linkedlist_freeList(app->download.queue, &destroyDownload);
    app->download.done = NULL;
    linkedlist_freeList(app->download.done, &destroyDownload);
    app->download.queue = NULL;

    pthread_mutex_unlock(&lockActive);
    pthread_mutex_unlock(&lockQueue);
    pthread_mutex_unlock(&lockDone);

    pthread_mutex_destroy(&lockActive);
    pthread_mutex_destroy(&lockQueue);
    pthread_mutex_destroy(&lockDone);
}

void downloader_addToQueue(app_t *app, system_t *system, char *title, char *url, char *data, char *filename,
                           httpmethod_t method) {

    download_t *download = calloc(1, sizeof(download_t));
    download->title = str_clone(title);
    download->active = 1;
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
        app->modal.callbackCancel = &modalCancel;
    } else {
        addDownload(app, download);
    }
    safe_destroy(downloadPath);
}

void downloader_cancel(app_t *app, download_t *download) {
    pthread_mutex_lock(&lockActive);
    pthread_mutex_lock(&lockQueue);
    pthread_mutex_lock(&lockDone);
    if (linkedlist_isElementInList(app->download.active, download)) {
        app->download.active = linkedlist_removeElement(app->download.active, download);
        download->next = NULL;
        download->prev = NULL;
        download->cancelled = 1;
        app->download.done = linkedlist_push(app->download.done, download);
    } else if (linkedlist_isElementInList(app->download.queue, download)) {
        app->download.queue = linkedlist_removeElement(app->download.queue, download);
        download->next = NULL;
        download->prev = NULL;
        download->cancelled = 1;
        app->download.done = linkedlist_push(app->download.done, download);
    }
    pthread_mutex_unlock(&lockActive);
    pthread_mutex_unlock(&lockQueue);
    pthread_mutex_unlock(&lockDone);
}

uint8_t downloader_isActive(app_t *app) {
    pthread_mutex_lock(&lockActive);
    uint32_t count = linkedlist_getElementCount(app->download.active);
    pthread_mutex_unlock(&lockActive);
    return count > 0;
}

static void destroyDownload(void *ptr) {
    download_t *download = (download_t *) ptr;
    FREENOTNULL(download->title);
    FREENOTNULL(download->url);
    FREENOTNULL(download->data);
    FREENOTNULL(download->filename);
}

static void modalDownload(void *app, void *data) {
    ((app_t *) app)->modal.displayed = 0;
    addDownload(app, data);
}

static void modalCancel(void *app, void *data) {
    app_t *appPtr = (app_t *) app;
    appPtr->modal.displayed = 0;
    statedownload_persist(appPtr);
    statesearch_init(appPtr);
    destroyDownload(data);
    free(data);
}

static void addDownload(app_t *app, download_t *download) {
    pthread_mutex_lock(&lockQueue);
    app->download.queue = linkedlist_appendElement(app->download.queue, download);
    pthread_mutex_unlock(&lockQueue);
}

static void spawnThreads(app_t *app) {
    pthread_t downloadThreads[DOWNLOADER_THREADS];
    for (int i = 0; i < DOWNLOADER_THREADS; i++) {
        if (pthread_create(&downloadThreads[i], NULL, downloadThreadExecution, app)) {
            SDL_Log("Error creating thread\n");
            return;
        }
        pthread_detach(downloadThreads[i]);
    }
}

static void *downloadThreadExecution(void *appPtr) {
    app_t *app = (app_t *) appPtr;
    download_t *download = NULL;
    while (!app->quit) {

        pthread_mutex_lock(&lockQueue);
        app->download.queue = linkedlist_pop(app->download.queue, (void *) &download);
        pthread_mutex_unlock(&lockQueue);

        if (download != NULL) {
            pthread_mutex_lock(&lockActive);
            app->download.active = linkedlist_appendElement(app->download.active, download);
            pthread_mutex_unlock(&lockActive);

            csafestring_t *downloadPath = path_downloadTarget(download->system, download->filename);
            curlling_download(download->url, download->data, download->method, downloadPath->data, &download->current,
                              &download->total, &download->cancelled);

            if (download->cancelled) {
                unlink(downloadPath->data);
                continue;
            }

            postProcess(app, downloadPath->data);

            safe_destroy(downloadPath);

            pthread_mutex_lock(&lockActive);
            app->download.active = linkedlist_removeElement(app->download.active, download);
            pthread_mutex_unlock(&lockActive);

            pthread_mutex_lock(&lockDone);
            app->download.done = linkedlist_push(app->download.done, download);
            pthread_mutex_unlock(&lockDone);
        } else {
            pthread_mutex_unlock(&lockQueue);
        }
        sleep(5);
    }
    return NULL;
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