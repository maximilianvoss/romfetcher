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

#ifndef ENUMS_H
#define ENUMS_H

typedef enum {
    window_search,
    window_system,
    window_keyboard,
    window_download,
    window_config,
    window_config_engine,
    window_config_systems
} window_t;

typedef enum {
    config_engine,
    config_systems
} config_t;

typedef enum {
    searchactivity_system,
    searchactivity_field,
    searchactivity_button,
    searchactivity_results,
    searchactivity_config
} searchactivity_t;

typedef enum {
    downloadActivity_cancel,
    downloadActivity_start,
    downloadActivity_done
} downloadactivity_t;

#endif
