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
#include "init.h"
#include "../config.h"
#include "config.h"
#include "enginecache.h"
#include "linkedlist.h"
#include "postprocess.h"
#include "download.h"
#include "../path.h"

static uint8_t doesTableExist(sqlite3 *db, char *tableName);

static void dropAllTables(sqlite3 *db);

static csafestring_t *buildDBPath();

void database_init(app_t *app) {
    csafestring_t *dbPath = buildDBPath();
    if (sqlite3_open_v2(dbPath->data, &app->database.db,
                        SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        printf("Could not initialize database %s: %s\n", dbPath->data, sqlite3_errmsg(app->database.db));
        sqlite3_close(app->database.db);
        safe_destroy(dbPath);
        exit(1);
    }

    safe_destroy(dbPath);
    database_initTables(app->database.db);
}

void database_destroy(app_t *app) {
    if (sqlite3_close(app->database.db) != SQLITE_OK) {
        printf("Could not destruct database: %s\n", sqlite3_errmsg(app->database.db));
    }
}

static csafestring_t *buildDBPath() {
    csafestring_t *path = path_romfetchersHome();
    safe_strcat(path, DATABASE_FILE);
    return path;
}

void database_initTables(sqlite3 *db) {
    if (!doesTableExist(db, "config")) {
        database_configInitTable(db);
    } else if (database_configCheckVersion(db)) {
        dropAllTables(db);
        database_configInitTable(db);
    }

    if (!doesTableExist(db, DATABASE_TABLE_SYSTEMS)) {
        databaselinkedlist_init(db, DATABASE_TABLE_SYSTEMS);
    }

    if (!doesTableExist(db, DATABASE_TABLE_ENGINES)) {
        databaselinkedlist_init(db, DATABASE_TABLE_ENGINES);
    }

    if (!doesTableExist(db, "enginecache")) {
        enginecache_init(db);
    }

    if (!doesTableExist(db, "enginecachestate")) {
        enginecache_initstate(db);
    }

    if (!doesTableExist(db, "postprocessors")) {
        databasepostprocess_init(db);
    }

    if (!doesTableExist(db, "downloads")) {
        download_init(db);
    }
}

static void dropAllTables(sqlite3 *db) {
    char *err_msg = 0;
    char *query = "DROP TABLE IF EXISTS systems; DROP TABLE IF EXISTS config; DROP TABLE IF EXISTS enginecache; DROP TABLE IF EXISTS enginecachestate; DROP TABLE IF EXISTS engines; DROP TABLE IF EXISTS postprocessors";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
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