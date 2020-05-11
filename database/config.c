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

#include "config.h"
#include "../config.h"

static void fillStandardValues(sqlite3 *db);

void database_configInitTable(sqlite3 *db) {
    char *err_msg = 0;
    char *query = "CREATE TABLE config (version INT, engine INT);";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    fillStandardValues(db);
}

uint8_t database_configCheckVersion(sqlite3 *db) {
    char *query = "SELECT version FROM config";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) < DATABASE_SCHEMA_SUPPORTED_LEGACY_VERSION) {
            sqlite3_finalize(stmt);
            return 1;
        }
    }
    sqlite3_finalize(stmt);
    return 0;
}

void database_configLoad(app_t *app) {
    char *query = "SELECT engine FROM config";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        app->engine.active = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
}

void database_configPersist(app_t *app) {
    char *query = "UPDATE config SET engine=@engine";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@engine");
        sqlite3_bind_int(stmt, idx, app->engine.active);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        fprintf(stderr, "update statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(app->database.db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}

static void fillStandardValues(sqlite3 *db) {
    char *query = "INSERT INTO config (version, engine) VALUES (@version, 1)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@version");
        sqlite3_bind_int(stmt, idx, DATABASE_SCHEMA_VERSION);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}
