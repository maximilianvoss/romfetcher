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

#include "linkedlist.h"
#include "../../common/utils.h"

static void addElement(sqlite3 *db, char *tableName, char *name, int active);

static int loadActivitiesCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

void databaselinkedlist_init(sqlite3 *db, char *tableName) {
    char *err_msg = 0;
    char *queryTmpl = "CREATE TABLE %tablename% (id INT PRIMARY KEY, name TEXT, active INT)";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    free(query);
}


void databaselinkedlist_persist(sqlite3 *db, char *tableName, linkedlist_t *list) {
    char *err_msg = 0;
    char *queryTmpl = "DELETE FROM %tablename%";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    while (list != NULL) {
        addElement(db, tableName, list->name, list->active);
        list = list->next;
    }
    free(query);
}

void databaselinkedlist_loadActivities(sqlite3 *db, char *tableName, linkedlist_t *list) {
    char *errmsg;
    char *queryTmpl = "SELECT name, active FROM %tablename%";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    int ret = sqlite3_exec(db, query, loadActivitiesCallback, list, &errmsg);
    if (ret != SQLITE_OK) {
        printf("Error in select statement %s [%s].\n", query, errmsg);
        free(query);
        return;
    }
}

static void addElement(sqlite3 *db, char *tableName, char *name, int active) {
    char *queryTmpl = "INSERT INTO %tablename% (name, active) VALUES (@name, @active)";
    char *query = str_replace(queryTmpl, "%tablename%", tableName);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@name");
        sqlite3_bind_text(stmt, idx, name, strlen(name), NULL);

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
    free(query);
}

static int loadActivitiesCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
    linkedlist_t *element = linkedlist_findElementByName(p_data, p_fields[0]);
    if (element != NULL) {
        if (!strcmp(p_fields[1], "1")) {
            element->active = 1;
        } else {
            element->active = 0;
        }
    }
    return 0;
}