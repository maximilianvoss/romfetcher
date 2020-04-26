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

#include "curlling.h"
#include <curl/curl.h>
#include <stdlib.h>
#include <memory.h>

struct url_data {
    size_t size;
    char *data;
};

static size_t writeDataToString(void *ptr, size_t size, size_t nmemb, struct url_data *data);

static size_t writeDataToFile(void *ptr, size_t size, size_t nmemb, void *stream);

static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

#if LIBCURL_VERSION_NUM < 0x072000
static int older_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow);
#endif

char *fetchURL(char *url) {
    CURL *curl;
    CURLcode res;

    struct url_data data;
    data.size = 0;
    data.data = malloc(16386);
    data.data[0] = '\0';

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataToString);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    return data.data;
}

int downloadURL(app_t *app, char *url, char *filename) {
    CURL *curl_handle;
    FILE *pagefile;
    CURLcode res = CURLE_OK;

    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, writeDataToFile);

#if LIBCURL_VERSION_NUM >= 0x072000
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFOFUNCTION, xferinfo);
    curl_easy_setopt(curl_handle, CURLOPT_XFERINFODATA, app);
#else
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, app);
#endif

    pagefile = fopen(filename, "wb");
    if (pagefile) {
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
        res = curl_easy_perform(curl_handle);
        fclose(pagefile);
    }
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
    return res;
}

static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    app_t *app = (app_t *) p;
    app->download.current = dlnow;
    app->download.total = dltotal;
    return 0;
}

#if LIBCURL_VERSION_NUM < 0x072000
static int older_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow) {
    return xferinfo(p, (curl_off_t) dltotal, (curl_off_t) dlnow, (curl_off_t) ultotal, (curl_off_t) ulnow);
}
#endif

static size_t writeDataToFile(void *ptr, size_t size, size_t nmemb, void *stream) {
    size_t written = fwrite(ptr, size, nmemb, (FILE *) stream);
    return written;
}

static size_t writeDataToString(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char *tmp;

    data->size += (size * nmemb);

    tmp = realloc(data->data, data->size + 1);
    if (tmp) {
        data->data = tmp;
    } else {
        if (data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}