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

#include "dynamic.h"
#include <dlfcn.h>
#include <romlibrary.h>
#include "../constants.h"

#define load(handler, func, name)\
    func = dlsym(handler, name); \
    errormsg = dlerror();\
    if ( func == NULL || errormsg != NULL)\
    LOG_ERROR("Not able to load: %s\nError: %s", name, errormsg);

void loadLibraryFunctions() {
#ifdef RL_DYNAMIC
    void *libHandler;
    char *errormsg;

    LOG_INFO("Loading library: %s", RL_DYNAMIC_LIBRARY);
    libHandler = dlopen(RL_DYNAMIC_LIBRARY, RTLD_LAZY);
    if (libHandler == NULL) {
        LOG_ERROR("Error during dlopen(): %s\n", dlerror());
        exit(1);
    }

    load(libHandler, rl_getVersion, "rl_getVersion");
    load(libHandler, rl_getVersionNumber, "rl_getVersionNumber");

    // system
    load(libHandler, rl_systems_init, "rl_systems_init");
    load(libHandler, rl_system_free, "rl_system_free");
    load(libHandler, rl_system_findByFullname, "rl_system_findByFullname");

    // hoster
    load(libHandler, rl_hoster_init, "rl_hoster_init");
    load(libHandler, rl_search, "rl_search");
    load(libHandler, rl_download, "rl_download");
    load(libHandler, rl_hoster_free, "rl_hoster_free");
    load(libHandler, rl_hoster_findByFullname, "rl_hoster_findByFullname");
    load(libHandler, rl_hoster_findByName, "rl_hoster_findByName");

    // results
    load(libHandler, rl_result_create, "rl_result_create");
    load(libHandler, rl_result_setTitle, "rl_result_setTitle");
    load(libHandler, rl_result_setUrl, "rl_result_setUrl");
    load(libHandler, rl_result_setDownloads, "rl_result_setDownloads");
    load(libHandler, rl_result_setRating, "rl_result_setRating");
    load(libHandler, rl_result_setFileSize, "rl_result_setFileSize");
    load(libHandler, rl_results_free, "rl_results_free");
    load(libHandler, rl_results_sort, "rl_results_sort");
#endif
}
