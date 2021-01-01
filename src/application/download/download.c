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

#include "download.h"
#include "downloader.h"

void download_init(sqlite3 *db) {
    char *err_msg = 0;

    char *query = "CREATE TABLE downloads (title TEXT, system TEXT, url TEXT, data TEXT, filename TEXT, method INT)";
    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to create table - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }
}

void download_load(app_t *app) {
    char *query = "SELECT title, system, url, data, filename, method FROM downloads";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(app->database.db));
    }

    int ret = sqlite3_step(stmt);
    while (ret == SQLITE_ROW) {
        char *title = (char *) sqlite3_column_text(stmt, 0);
        char *systemMarker = (char *) sqlite3_column_text(stmt, 1);
        char *url = (char *) sqlite3_column_text(stmt, 2);
        char *data = (char *) sqlite3_column_text(stmt, 3);
        char *filename = (char *) sqlite3_column_text(stmt, 4);
        int methodMarker = sqlite3_column_int(stmt, 5);

        system_t *system = linkedlist_findElementByName(app->systems.all, systemMarker);
        httpmethod_t method = (methodMarker == 1) ? POST : GET;

        downloader_addToQueue(app, system, title, url, data, filename, method);

        ret = sqlite3_step(stmt);
    }
    if (ret == SQLITE_ERROR) {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(app->database.db));
    }

    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);

    char *err_msg = 0;
    query = "DELETE FROM downloads";

    rc = sqlite3_exec(app->database.db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to create table - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }
}

void download_persistDownload(app_t *app, download_t *download) {
    char *query = "INSERT INTO downloads (title, system, url, data, filename, method) VALUES (@title, @system, @url, @data, @filename, @method)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@title");
        sqlite3_bind_text(stmt, idx, download->title, strlen(download->title), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@system");
        sqlite3_bind_text(stmt, idx, download->system->fullname, strlen(download->system->fullname), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@url");
        sqlite3_bind_text(stmt, idx, download->url, strlen(download->url), NULL);

        if (download->data != NULL) {
            idx = sqlite3_bind_parameter_index(stmt, "@data");
            sqlite3_bind_text(stmt, idx, download->data, strlen(download->data), NULL);
        }

        idx = sqlite3_bind_parameter_index(stmt, "@filename");
        sqlite3_bind_text(stmt, idx, download->filename, strlen(download->filename), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@method");
        sqlite3_bind_int(stmt, idx, download->method == POST ? 1 : 0);
    } else {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(app->database.db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        LOG_ERROR("insert statement didn't return DONE (%i): %s", rc, sqlite3_errmsg(app->database.db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
}