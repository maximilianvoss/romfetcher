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

#include "postprocess.h"
#include "../../common/utils.h"

static void fillDefaultValus(sqlite3 *db);

void databasepostprocess_init(sqlite3 *db) {
    char *err_msg = 0;

    char *query = "CREATE TABLE postprocessors (filesuffix TEXT, command TEXT, active INT)";
    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to create table - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }
    fillDefaultValus(db);
}

char *databasepostprocess_getCommand(app_t *app, char *fileSuffix) {
    char *query = "SELECT command FROM postprocessors WHERE filesuffix=@filesuffix AND active=1";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(app->database.db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@filesuffix");
        sqlite3_bind_text(stmt, idx, fileSuffix, strlen(fileSuffix), NULL);
    } else {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(app->database.db));
    }

    char *command = NULL;
    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        command = str_clone((char *) sqlite3_column_text(stmt, 0));
    }
    sqlite3_finalize(stmt);
    return command;
}

static void fillDefaultValus(sqlite3 *db) {
    char *err_msg = 0;

    char *query = "INSERT INTO postprocessors (filesuffix, command, active) VALUES\
                        ('.zip', 'unzip -o \"%file%\" -d \"%target%\"', 0),\
                        ('.rar', 'unrar -x -f \"%file%\" \"%target%\"', 1),\
                        ('.7z', '7zr -y x \"%file%\" -w \"%target%\"', 1)";
    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to execute query - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }
}