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

struct url_data {
    size_t size;
    char *data;
};

struct state_s {
    curl_off_t *current;
    curl_off_t *total;
};

static size_t writeDataToString(void *ptr, size_t size, size_t nmemb, struct url_data *data);

static size_t writeDataToFile(void *ptr, size_t size, size_t nmemb, void *stream);

static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow);

#if LIBCURL_VERSION_NUM < 0x072000
static int older_progress(void *p, double dltotal, double dlnow, double ultotal, double ulnow);
#endif

char *curlling_fetch(char *url, char *postData, httpmethod_t method) {
    CURL *curl;
    CURLcode res;

    SDL_Log("Fetching: %s", url);

    struct url_data data;
    data.size = 0;
    data.data = calloc(16386, sizeof(char));

    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk,
                                  "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36");

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataToString);
        curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        if (method == POST) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
        }

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
    return data.data;
}

int
curlling_download(char *url, char *data, httpmethod_t method, char *filename, curl_off_t *current, curl_off_t *total) {
    CURL *curl;
    FILE *pagefile;
    CURLcode res = CURLE_OK;

    SDL_Log("Downloading: %s to %s", url, filename);

    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk,
                                  "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.138 Safari/537.36");

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeDataToFile);
        if (method == POST) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        }

        struct state_s state;
        state.current = current;
        state.total = total;

#if LIBCURL_VERSION_NUM >= 0x072000
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &state);
#else
        curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, older_progress);
        curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &state);
#endif
    }

    pagefile = fopen(filename, "wb");
    if (pagefile) {
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, pagefile);
        res = curl_easy_perform(curl);
        fclose(pagefile);
    }
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    SDL_Log("Download of %s to %s completed", url, filename);
    return res;
}

static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    struct state_s *state = (struct state_s *) p;
    *state->current = dlnow;
    *state->total = dltotal;
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