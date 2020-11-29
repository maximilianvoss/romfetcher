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
#include "../themes/loading.h"
#include "../config/advanced.h"

static void fillStandardValues(sqlite3 *db);

void database_configInitTable(sqlite3 *db) {
    char *err_msg = 0;
    char *query = "CREATE TABLE config (version INT, theme TEXT, fullscreen INT, opengl INT, highdpi INT, downloadqueue INT, externalcurl INT, resolution TEXT);";

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
    char *query = "SELECT theme, fullscreen, opengl, highdpi, downloadqueue, externalcurl, resolution FROM config";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(app->database.db));
    }

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        char *themePath = (char *) sqlite3_column_text(stmt, 0);
        app->themes.active = themes_getByFileReference(app, themePath);
        int fullscreen = sqlite3_column_int(stmt, 1);
        int opengl = sqlite3_column_int(stmt, 2);
        int highdpi = sqlite3_column_int(stmt, 3);
        int downloadQueue = sqlite3_column_int(stmt, 4);
        int externalCurl = sqlite3_column_int(stmt, 5);
        char *resolution = (char *) sqlite3_column_text(stmt, 6);
        app->config.resolution.active = linkedlist_findElementByName(app->config.resolution.all, resolution);

        configadvanced_setConfig(app, advancedConfig_fullscreen, fullscreen);
        configadvanced_setConfig(app, advancedConfig_openGL, opengl);
        configadvanced_setConfig(app, advancedConfig_highDPI, highdpi);
        configadvanced_setConfig(app, advancedConfig_downloadQueue, downloadQueue);
        configadvanced_setConfig(app, advancedConfig_externalCurl, externalCurl);
    }
    if (app->themes.active == NULL) {
        app->themes.active = app->themes.all;
    }
    if (app->config.resolution.active == NULL) {
        app->config.resolution.active = app->config.resolution.all;
    }
    sqlite3_finalize(stmt);
}

void database_configPersist(app_t *app) {
    char *query = "UPDATE config SET theme=@theme, fullscreen=@fullscreen, opengl=@opengl, highdpi=@highdpi, resolution=@resolution, downloadqueue=@downloadqueue, externalcurl=@externalcurl";

    sqlite3_stmt *stmt;
    configadvanced_listToSettings(app);
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@theme");
        sqlite3_bind_text(stmt, idx, app->themes.active->fileReference, strlen(app->themes.active->fileReference),
                          NULL);
        idx = sqlite3_bind_parameter_index(stmt, "@fullscreen");
        sqlite3_bind_int(stmt, idx, app->config.advanced.fullscreen);
        idx = sqlite3_bind_parameter_index(stmt, "@opengl");
        sqlite3_bind_int(stmt, idx, app->config.advanced.opengl);
        idx = sqlite3_bind_parameter_index(stmt, "@highdpi");
        sqlite3_bind_int(stmt, idx, app->config.advanced.highdpi);
        idx = sqlite3_bind_parameter_index(stmt, "@downloadqueue");
        sqlite3_bind_int(stmt, idx, app->config.advanced.downloadQueue);
        idx = sqlite3_bind_parameter_index(stmt, "@externalcurl");
        sqlite3_bind_int(stmt, idx, app->config.advanced.externalCurl);
        idx = sqlite3_bind_parameter_index(stmt, "@resolution");
        sqlite3_bind_text(stmt, idx, app->config.resolution.active->name, strlen(app->config.resolution.active->name),
                          NULL);
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
    char *query = "INSERT INTO config (version, fullscreen, downloadqueue) VALUES (@version, 1, 1)";

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