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

#ifndef _LIST_LIST_H
#define _LIST_LIST_H

#include "../application.h"

void ll_processUp(app_t *app);

void ll_processDown(app_t *app);

void ll_processLeft(app_t *app);

void ll_processRight(app_t *app);

void ll_processSelect(app_t *app);

void ll_processBack(app_t *app);

void ll_processOtherButton(app_t *app, controller_t *state);

void ll_processOtherKey(app_t *app, SDL_Scancode scancode);

void ll_renderDefault(app_t *app);

void ll_renderList(app_t *app, int offset);

void ll_dbInit(sqlite3 *db, char *tableName);

void ll_dbPersist(sqlite3 *db, char *tableName, acll_t *list);

void ll_dbLoad(sqlite3 *db, char *tableName, acll_t *list);

int ll_filterActive(void *payload, void *input);

#endif
