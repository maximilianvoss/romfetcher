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

#include "init.h"
#include "../config.h"
#include "sytems.h"

static void initTables(sqlite3 *db);

static uint8_t doesTableExist(sqlite3 *db, char *tableName);

void database_init(app_t *app) {
    if (sqlite3_open(DATABASE_FILE, &app->database.db) != SQLITE_OK) {
        printf("Could not initialize database: %s\n", sqlite3_errmsg(app->database.db));
        sqlite3_close(app->database.db);
        exit(1);
    }
    initTables(app->database.db);
}

void database_destroy(app_t *app) {
    if (sqlite3_close(app->database.db) != SQLITE_OK) {
        printf("Could not destruct database: %s\n", sqlite3_errmsg(app->database.db));
    }
}

static void initTables(sqlite3 *db) {
    if (!doesTableExist(db, "systems")) {
        database_systemsInitTable(db);
    }
}

static uint8_t doesTableExist(sqlite3 *db, char *tableName) {
    char *query = "SELECT name FROM sqlite_master WHERE type='table' AND name=@name";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx = sqlite3_bind_parameter_index(stmt, "@name");
        sqlite3_bind_text(stmt, idx, tableName, strlen(tableName), NULL);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return 1;
    }

    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    return 0;
}