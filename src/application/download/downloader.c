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

#include "downloader.h"
#include "downloadercurl.h"
#include "downloaderinternal.h"

void downloader_init(app_t *app) {
    if (app->config.advanced.externalCurl) {
        downloadercurl_init(app);
    } else {
        downloaderinternal_init(app);
    }
}

void downloader_destroy(app_t *app) {
    if (app->config.advanced.externalCurl) {
        downloadercurl_destroy(app);
    } else {
        downloaderinternal_destroy(app);
    }
}

void downloader_addToQueue(app_t *app, system_t *system, char *title, char *url, char *data, char *filename,
                           httpmethod_t method) {
    if (app->config.advanced.externalCurl) {
        downloadercurl_addToQueue(app, system, title, url, data, filename, method);
    } else {
        downloaderinternal_addToQueue(app, system, title, url, data, filename, method);
    }
}

void downloader_cancel(app_t *app, download_t *download) {
    if (app->config.advanced.externalCurl) {
        downloadercurl_cancel(app, download);
    } else {
        downloaderinternal_cancel(app, download);
    }
}

uint8_t downloader_isActive(app_t *app) {
    if (app->config.advanced.externalCurl) {
        return downloadercurl_isActive(app);
    } else {
        return downloaderinternal_isActive(app);
    }
}

void downloader_cancelAllDownloads(app_t *app) {
    if (app->config.advanced.externalCurl) {
        downloadercurl_cancelAllDownloads(app);
    } else {
        downloaderinternal_cancelAllDownloads(app);
    }
}