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

#include "statekeyboard.h"

window_t statekeyboard_target(app_t *app, uint8_t isSelectButton) {
    return window_search;
}

void statekeyboard_persist(app_t *app) {
    memcpy(app->search.searchText, app->keyboard.text, 256);
}

void statekeyboard_init(app_t *app) {
    app->win = window_keyboard;
    strcpy(app->keyboard.text, app->search.searchText);
}