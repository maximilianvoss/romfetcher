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

#include <dirent.h>
#include <sys/stat.h>
#include "path.h"
#include "../constants.h"


csafestring_t *path_romfetchersHome() {
    LOG_TRACE("path_romfetchersHome start");
    csafestring_t *path = safe_create(getenv("HOME"));
    safe_strchrappend(path, '/');
    safe_strcat(path, HOMEDIR_PATH);
    safe_strchrappend(path, '/');
    LOG_TRACE("path_romfetchersHome done. (return=%s)", path->data);
    return path;
}

void path_initRomfetchersHome() {
    LOG_TRACE("path_initRomfetchersHome start");
    csafestring_t *path = path_romfetchersHome();
    struct stat st = {0};
    if (stat(path->data, &st) == -1) {
        mkdir(path->data, 0700);
    }
    safe_destroy(path);
    LOG_TRACE("path_initRomfetchersHome done");
}

csafestring_t *path_downloadTarget(rl_system *system, char *filename) {
    LOG_TRACE("path_downloadTarget start (system=%s, filename=%s)", system->fullname, filename);
    csafestring_t *path = safe_create(getenv("HOME"));
    safe_strchrappend(path, '/');
    safe_strcat(path, ROM_BASE_DIR);
    safe_strcat(path, system->path);
    safe_strchrappend(path, '/');
    safe_strcat(path, filename);
    LOG_TRACE("path_downloadTarget done (system=%s, filename=%s, return=%s)", system->fullname, filename, path->data);
    return path;
}