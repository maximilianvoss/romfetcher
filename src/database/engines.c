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

#include "engines.h"
#include "../helper/utils.h"
#include "../engine/enginehandler.h"
#include "../engine/romsmania/romsmania.h"
#include "../engine/romsemulator/romsemulator.h"
#include "../engine/romsdownload/romsdownload.h"
#include "../engine/wowroms/wowroms.h"
#include "../engine/romsmode/romsmode.h"
#include "../engine/romhustler/romhustler.h"
#include "../engine/progameroms/progameroms.h"

static void addEngine(sqlite3 *db, char *name, char *fullname, int active);

static void addDefaultEngines(sqlite3 *db);

static int engineListCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

static engine_t *createEngineItem(char *name, char *fullname, int active);

static void freeEngineList(void *ptr);

void database_enginesInitTable(sqlite3 *db) {
    char *err_msg = 0;
    char *query = "CREATE TABLE engines (id INT PRIMARY KEY, name TEXT, fullname TEXT, active INT)";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    addDefaultEngines(db);
}

void database_enginesStore(sqlite3 *db, engine_t *engines) {
    char *err_msg = 0;
    char *query = "DELETE FROM engines";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    while (engines != NULL) {
        addEngine(db, engines->shortname(), engines->fullname, engines->active);
        engines = engines->next;
    }
}

engine_t *database_engineList(app_t *app, uint8_t active) {
    char *errmsg;
    char *query;
    if (active) {
        query = "SELECT name, fullname, active FROM engines WHERE active=1";
    } else {
        query = "SELECT name, fullname, active FROM engines";
    }

    engine_t *engines = NULL;
    int ret = sqlite3_exec(app->database.db, query, engineListCallback, &engines, &errmsg);
    if (ret != SQLITE_OK) {
        printf("Error in select statement %s [%s].\n", query, errmsg);
        return NULL;
    }
    enginehandler_doMapping(engines);
    return engines;
}

void database_enginesDestroy(engine_t *engines) {
    linkedlist_freeList(engines, &freeEngineList);
}

static void addDefaultEngines(sqlite3 *db) {
    addEngine(db, romsmania_shortname(), "https://www.romsmania.cc", 1);
    addEngine(db, romsmode_shortname(), "https://www.romsmode.com", 1);
    addEngine(db, wowroms_shortname(), "https://www.wowroms.com", 1);
    addEngine(db, romsdownload_shortname(), "https://www.roms-download.com", 1);
    addEngine(db, romsemulator_shortname(), "https://www.romsemulator.net", 1);
    addEngine(db, romhustler_shortname(), "https://romhustler.org", 1);
    addEngine(db, progameroms_shortname(), "https://progameroms.com", 1);
}

static void addEngine(sqlite3 *db, char *name, char *fullname, int active) {
    char *query = "INSERT INTO engines (name, fullname, active) VALUES (@name, @fullname, @active)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@name");
        sqlite3_bind_text(stmt, idx, name, strlen(name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@fullname");
        sqlite3_bind_text(stmt, idx, fullname, strlen(fullname), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@active");
        sqlite3_bind_int(stmt, idx, active);
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

static int engineListCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
    engine_t *engine = createEngineItem(p_fields[0], p_fields[1], atoi(p_fields[2]));
    engine_t **enginePtr = (engine_t **) p_data;
    *enginePtr = linkedlist_appendElement(*enginePtr, engine);
    return 0;
}

static engine_t *createEngineItem(char *name, char *fullname, int active) {
    int length;
    engine_t *engine = calloc(1, sizeof(engine_t));
    if (name != NULL) {
        length = strlen(name) + 1;
        engine->name = (char *) calloc(length, sizeof(char));
        strcpy(engine->name, name);
    }
    if (fullname != NULL) {
        length = strlen(fullname) + 1;
        engine->fullname = (char *) calloc(length, sizeof(char));
        strcpy(engine->fullname, fullname);
    }
    engine->active = active;
    engine->prev = NULL;
    engine->next = NULL;
    return engine;
}

static void freeEngineList(void *ptr) {
    engine_t *engine = (engine_t *) ptr;
    FREENOTNULL(engine->name);
    FREENOTNULL(engine->fullname);
}
