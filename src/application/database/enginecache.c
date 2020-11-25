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
#include <hoster/results.h>
#include "enginecache.h"
#include "../../library/helper/utils.h"

static void deleteTimestamp(app_t *app, hoster_t *hoster, system_t *system);

static void insertTimestamp(app_t *app, hoster_t *hoster, system_t *system);

void enginecache_init(sqlite3 *db) {
    char *err_msg = 0;

    char *query = "CREATE TABLE enginecache (hosters TEXT, system TEXT, title TEXT, link TEXT)";
    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    query = "CREATE INDEX enginecache_idx ON enginecache (hosters, title)";
    rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void enginecache_initstate(sqlite3 *db) {
    char *err_msg = 0;

    char *query = "CREATE TABLE enginecachestate (hosters TEXT, system TEXT, time INT64)";
    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

void enginecache_clear(hoster_t *hoster, system_t *system, void *appPtr) {
    app_t *app = appPtr;
    char *query = "DELETE FROM enginecache WHERE hosters=@hosters AND system=@system";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@hosters");
        sqlite3_bind_text(stmt, idx, hoster->name, strlen(hoster->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@system");
        sqlite3_bind_text(stmt, idx, system->name, strlen(system->name), NULL);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        fprintf(stderr, "delete statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(app->database.db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}

uint8_t enginecache_isCacheValid(hoster_t *hoster, system_t *system, void *appPtr) {
    app_t *app = appPtr;
    char *query = "SELECT time FROM enginecachestate WHERE hosters=@hosters AND system=@system";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@hosters");
        sqlite3_bind_text(stmt, idx, hoster->name, strlen(hoster->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@system");
        sqlite3_bind_text(stmt, idx, system->name, strlen(system->name), NULL);
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        int timer = sqlite3_column_int(stmt, 0);
        if (timer > time(NULL) - 3600 * 24 * 30) {
            sqlite3_finalize(stmt);
            return 1;
        }
        SDL_Log("Search cache for %s / %s invalidated", hoster->name, system->name);
        sqlite3_finalize(stmt);
        return 0;
    }
    sqlite3_finalize(stmt);
    return 0;
}

void enginecache_addEntry(hoster_t *hoster, system_t *system, char *title, char *link, void *appPtr) {
    app_t *app = appPtr;
    char *query = "INSERT INTO enginecache (hosters, system, title, link) VALUES (@hosters, @system, @title, @link)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@hosters");
        sqlite3_bind_text(stmt, idx, hoster->name, strlen(hoster->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@system");
        sqlite3_bind_text(stmt, idx, system->name, strlen(system->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@title");
        sqlite3_bind_text(stmt, idx, title, strlen(title), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@link");
        sqlite3_bind_text(stmt, idx, link, strlen(link), NULL);

    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(app->database.db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}

void enginecache_updateTimestamp(hoster_t *hoster, system_t *system, void *app) {
    deleteTimestamp(app, hoster, system);
    insertTimestamp(app, hoster, system);
}

result_t *enginecache_getSearchResults(hoster_t *hoster, system_t *system, char *searchString, void *appPtr) {
    app_t *app = appPtr;
    char *query = "SELECT title, link FROM enginecache WHERE hosters=@hosters AND system=@system AND UPPER(title) LIKE @searchString";

    sqlite3_stmt *stmt;
    char *tmp = NULL;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@hosters");
        sqlite3_bind_text(stmt, idx, hoster->name, strlen(hoster->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@system");
        sqlite3_bind_text(stmt, idx, system->name, strlen(system->name), NULL);

        tmp = (char *) calloc(strlen(searchString) + 3, sizeof(char));
        tmp[0] = '%';
        strcpy(&tmp[1], searchString);
        tmp[strlen(tmp)] = '%';

        idx = sqlite3_bind_parameter_index(stmt, "@searchString");
        sqlite3_bind_text(stmt, idx, tmp, strlen(tmp), NULL);

    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    result_t *resultList = NULL;
    int ret = sqlite3_step(stmt);
    while (ret == SQLITE_ROW) {
        char *title = (char *) sqlite3_column_text(stmt, 0);
        char *link = (char *) sqlite3_column_text(stmt, 1);

        result_t *item = result_newItem(system, hoster);
        result_setTitle(item, title);
        result_setUrl(item, link);
        resultList = linkedlist_appendElement(resultList, item);
        ret = sqlite3_step(stmt);
    }
    if (ret == SQLITE_ERROR) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    FREENOTNULL(tmp);
    return resultList;
}

static void deleteTimestamp(app_t *app, hoster_t *hoster, system_t *system) {
    char *query = "DELETE FROM enginecachestate WHERE hosters=@hosters AND system=@system";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@hosters");
        sqlite3_bind_text(stmt, idx, hoster->name, strlen(hoster->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@system");
        sqlite3_bind_text(stmt, idx, system->name, strlen(system->name), NULL);

    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        fprintf(stderr, "delete statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(app->database.db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}

static void insertTimestamp(app_t *app, hoster_t *hoster, system_t *system) {
    char *query = "INSERT INTO enginecachestate (hosters, system, time) VALUES (@hosters, @system, @time)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@hosters");
        sqlite3_bind_text(stmt, idx, hoster->name, strlen(hoster->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@system");
        sqlite3_bind_text(stmt, idx, system->name, strlen(system->name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@time");
        sqlite3_bind_int64(stmt, idx, time(NULL));
    } else {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        fprintf(stderr, "insert statement didn't return DONE (%i): %s\n", rc, sqlite3_errmsg(app->database.db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}