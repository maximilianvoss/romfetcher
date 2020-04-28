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

#ifndef DATABASE_SYTEMS_H
#define DATABASE_SYTEMS_H

#include <sqlite3.h>
#include "../structs.h"

void database_systemsInitTable(sqlite3 *db);

void database_systemAdd(sqlite3 *db, char *name, char *fullname, char *path, int active);

system_t *database_systemList(app_t *app);

void database_systemsDestroy();

#endif
