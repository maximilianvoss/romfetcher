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

#include <csafestring.h>
#include "database.h"
#include "../constants.h"
#include "../enginecache/enginecache.h"
#include "../list/dblist.h"
#include "../download/postprocess.h"
#include "../download/download.h"
#include "../helper/path.h"

static csafestring_t *buildDBPath();

sqlite3 *database_init() {
    sqlite3 *db;
    csafestring_t *dbPath = buildDBPath();
    if (sqlite3_open_v2(dbPath->data, &db,
                        SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        LOG_ERROR("Could not initialize database %s: %s", dbPath->data, sqlite3_errmsg(db));
        sqlite3_close(db);
        safe_destroy(dbPath);
        exit(1);
    }

    safe_destroy(dbPath);
    database_initTables(db);
    return db;
}

void database_destroy(sqlite3 *db) {
    if (sqlite3_close(db) != SQLITE_OK) {
        LOG_ERROR("Could not destruct database: %s", sqlite3_errmsg(db));
    }
}

static csafestring_t *buildDBPath() {
    csafestring_t *path = path_romfetchersHome();
    safe_strcat(path, DATABASE_FILE);
    return path;
}

void database_initTables(sqlite3 *db) {
    if (!database_tableExists(db, "enginecache")) {
        enginecache_init(db);
    }

    if (!database_tableExists(db, "enginecachestate")) {
        enginecache_initstate(db);
    }

    if (!database_tableExists(db, "postprocessors")) {
        databasepostprocess_init(db);
    }

    if (!database_tableExists(db, "downloads")) {
        download_init(db);
    }
}

void database_dropAllTables(sqlite3 *db) {
    char *err_msg = 0;
    char *query = "DROP TABLE IF EXISTS systems; DROP TABLE IF EXISTS config; DROP TABLE IF EXISTS enginecache; DROP TABLE IF EXISTS enginecachestate; DROP TABLE IF EXISTS engines; DROP TABLE IF EXISTS postprocessors";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to create table - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }
}

uint8_t database_tableExists(sqlite3 *db, char *tableName) {
    char *query = "SELECT name FROM sqlite_master WHERE type='table' AND name=@name";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx = sqlite3_bind_parameter_index(stmt, "@name");
        sqlite3_bind_text(stmt, idx, tableName, strlen(tableName), NULL);
    } else {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(db));
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