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
#include "romsmania.h"
#include "mapping.h"
#include "../../download/curlling.h"
#include "../results.h"
#include "../../helper/utils.h"
#include "../urlhandling.h"
#include "../../helper/regex.h"
#include "../../helper/path.h"

#define SHORTNAME "_TEMPALTE_"
#define URL_TEMPLATE "https://template/%system%/%query%&page=%page%"

static engine_t *engine = NULL;

engine_t *_template_getEngine() {
    if (engine == NULL) {
        engine = calloc(1, sizeof(engine_t));
        engine->search = search;
        engine->download = download;
        engine->name = SHORTNAME;
        engine->active = 0;
        engine->fullname = "https://template";
    }
    return engine;
}

static searchresult_t *search(void *app, system_t *system, char *searchString) {
    return NULL;
}

static void download(void *app, searchresult_t *item) {
    callback(app);
}
