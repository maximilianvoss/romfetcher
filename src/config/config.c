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

#include "config.h"
#include "../database/database.h"
#include "../constants.h"
#include "../list/list.h"
#include "../themes/loading.h"

static void initConfigTable(sqlite3 *db);

static uint8_t checkDBVersion(sqlite3 *db);

static void fillStdConfig(sqlite3 *db);

static int filterResolutionName(void *payload, void *input);

void
config_load(sqlite3 *db, acll_t **configMenu, acll_t **advancedConfigs,
            acll_t **resolutionConfigs, acll_t **themes,
            acll_t *hosters, acll_t *systems) {
    LOG_TRACE("config_load start");

    // set initial states
    if (!database_tableExists(db, "config")) {
        LOG_DEBUG("Database config doesn't exist");
        initConfigTable(db);
    } else if (checkDBVersion(db)) {
        LOG_DEBUG("DB Version outdated, recreating tables");
        database_dropAllTables(db);
        initConfigTable(db);
    }
    if (!database_tableExists(db, DATABASE_TABLE_SYSTEMS)) {
        LOG_DEBUG("Table: %s did not exist", DATABASE_TABLE_SYSTEMS);
        ll_dbInit(db, DATABASE_TABLE_SYSTEMS);
    }
    if (!database_tableExists(db, DATABASE_TABLE_HOSTER)) {
        LOG_DEBUG("Table: %s did not exist", DATABASE_TABLE_HOSTER);
        ll_dbInit(db, DATABASE_TABLE_HOSTER);
    }

    *configMenu = configMenu_init();
    *advancedConfigs = advancedConfig_init();
    *resolutionConfigs = resolutionConfig_init();

    // load configs from DB, same as revert
    config_revert(db, advancedConfigs, resolutionConfigs, themes);

    // load the hosters & systems settings
    ll_dbLoad(db, DATABASE_TABLE_HOSTER, hosters);
    ll_dbLoad(db, DATABASE_TABLE_SYSTEMS, systems);

    LOG_TRACE("config_load done");
}

void config_revert(sqlite3 *db, acll_t **advancedConfigs,
                   acll_t **resolutionConfigs, acll_t **themes) {
    LOG_TRACE("config_revert start");

    // updating config with DB values
    char *query = "SELECT theme, fullscreen, opengl, highdpi, downloadqueue, externalcurl, resolution FROM config";
    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        char *themePath = (char *) sqlite3_column_text(stmt, 0);
        *themes = themes_getByFileReference(*themes, themePath);

        int fullscreen = sqlite3_column_int(stmt, 1);
        int opengl = sqlite3_column_int(stmt, 2);
        int highdpi = sqlite3_column_int(stmt, 3);
        int downloadQueue = sqlite3_column_int(stmt, 4);
        int externalCurl = sqlite3_column_int(stmt, 5);
        char *resolutionStr = (char *) sqlite3_column_text(stmt, 6);

        *resolutionConfigs = acll_find(*resolutionConfigs, filterResolutionName, resolutionStr);

        config_set(*advancedConfigs, advancedConfig_fullscreen, fullscreen);
        config_set(*advancedConfigs, advancedConfig_openGL, opengl);
        config_set(*advancedConfigs, advancedConfig_highDPI, highdpi);
        config_set(*advancedConfigs, advancedConfig_downloadQueue, downloadQueue);
        config_set(*advancedConfigs, advancedConfig_externalCurl, externalCurl);
    }
    sqlite3_finalize(stmt);
    LOG_TRACE("config_revert done");
}

void
config_persist(sqlite3 *db, acll_t *advancedConfigs, acll_t *resolutionConfigs, acll_t *theme,
               acll_t *hosters, acll_t *systems) {
    LOG_TRACE("config_persist start");
    char *query = "UPDATE config SET theme=@theme, fullscreen=@fullscreen, opengl=@opengl, highdpi=@highdpi, resolution=@resolution, downloadqueue=@downloadqueue, externalcurl=@externalcurl";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@theme");
        sqlite3_bind_text(stmt, idx, getTheme(theme)->fileReference, strlen(getTheme(theme)->fileReference), NULL);
        idx = sqlite3_bind_parameter_index(stmt, "@fullscreen");
        sqlite3_bind_int(stmt, idx, config_get(advancedConfigs, advancedConfig_fullscreen));
        idx = sqlite3_bind_parameter_index(stmt, "@opengl");
        sqlite3_bind_int(stmt, idx, config_get(advancedConfigs, advancedConfig_openGL));
        idx = sqlite3_bind_parameter_index(stmt, "@highdpi");
        sqlite3_bind_int(stmt, idx, config_get(advancedConfigs, advancedConfig_highDPI));
        idx = sqlite3_bind_parameter_index(stmt, "@downloadqueue");
        sqlite3_bind_int(stmt, idx, config_get(advancedConfigs, advancedConfig_downloadQueue));
        idx = sqlite3_bind_parameter_index(stmt, "@externalcurl");
        sqlite3_bind_int(stmt, idx, config_get(advancedConfigs, advancedConfig_externalCurl));
        idx = sqlite3_bind_parameter_index(stmt, "@resolution");
        sqlite3_bind_text(stmt, idx, getResolutionConfig(resolutionConfigs)->name,
                          strlen(getResolutionConfig(resolutionConfigs)->name), NULL);
    } else {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        LOG_ERROR("update statement didn't return DONE (%i): %s", rc, sqlite3_errmsg(db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);

    ll_dbPersist(db, DATABASE_TABLE_HOSTER, hosters);
    ll_dbPersist(db, DATABASE_TABLE_SYSTEMS, systems);
    LOG_TRACE("config_persist done");
}

void
config_destroy(acll_t *configMenu, acll_t *advancedConfigs, acll_t *resolutionConfigs) {
    LOG_TRACE("config_destroy start");
    configMenu_destroy(configMenu);
    advancedConfig_destroy(advancedConfigs);
    resolutionConfig_destroy(resolutionConfigs);
    LOG_TRACE("config_destroy done");
}

void config_set(acll_t *advancedConfigs, advancedConfigSetting_t setting, uint8_t value) {
    LOG_TRACE("config_set start (setting=%d, value=%d)", setting, value);
    acll_t *ptr = advancedConfigs;
    while (ptr != NULL) {
        if (getAdvancedConfig(ptr)->setting == setting) {
            getAdvancedConfig(ptr)->active = value;
            return;
        }
        ptr = ptr->next;
    }
    LOG_TRACE("config_set done (setting=%d, value=%d)", setting, value);
}

uint8_t config_get(acll_t *advancedConfigs, advancedConfigSetting_t setting) {
    LOG_TRACE("config_get start (setting=%d)", setting);
    acll_t *ptr = acll_first(advancedConfigs);
    while (ptr != NULL) {
        if (getAdvancedConfig(ptr)->setting == setting) {
            LOG_TRACE("config_get done (setting=%d, return=%d)", setting, getAdvancedConfig(ptr)->active);
            return getAdvancedConfig(ptr)->active;
        }
        ptr = ptr->next;
    }
    LOG_TRACE("config_get done (setting=%d, return=255)", setting);
    return 255;
}

static void initConfigTable(sqlite3 *db) {
    LOG_TRACE("initConfigTable start");
    char *err_msg = 0;
    char *query = "CREATE TABLE config (version INT, theme TEXT, fullscreen INT, opengl INT, highdpi INT, downloadqueue INT, externalcurl INT, resolution TEXT);";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to create table - SQL error: %s", err_msg);
        sqlite3_free(err_msg);
    }
    fillStdConfig(db);
    LOG_TRACE("initConfigTable done");
}

static uint8_t checkDBVersion(sqlite3 *db) {
    LOG_TRACE("checkDBVersion start");
    char *query = "SELECT version FROM config";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(db));
    }

    int step = sqlite3_step(stmt);
    if (step == SQLITE_ROW) {
        if (sqlite3_column_int(stmt, 0) < DATABASE_SCHEMA_SUPPORTED_LEGACY_VERSION) {
            sqlite3_finalize(stmt);
            LOG_TRACE("checkDBVersion done (return=1)");
            return 1;
        }
    }
    sqlite3_finalize(stmt);
    LOG_TRACE("checkDBVersion done (return=0)");
    return 0;
}

static void fillStdConfig(sqlite3 *db) {
    LOG_TRACE("fillStdConfig start");
    char *query = "INSERT INTO config (version, fullscreen, downloadqueue) VALUES (@version, 1, 1)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@version");
        sqlite3_bind_int(stmt, idx, DATABASE_SCHEMA_VERSION);
    } else {
        LOG_ERROR("Failed to execute statement: %s", sqlite3_errmsg(db));
    }

    rc = sqlite3_step(stmt);
    if (SQLITE_DONE != rc) {
        LOG_ERROR("insert statement didn't return DONE (%i): %s", rc, sqlite3_errmsg(db));
    }
    sqlite3_clear_bindings(stmt);
    sqlite3_finalize(stmt);
    LOG_TRACE("fillStdConfig done");
}

static int filterResolutionName(void *payload, void *input) {
    LOG_TRACE("filterResolutionName start");
    resolutionConfig_t *resolutionConfig = payload;
    char *name = input;
    LOG_TRACE("resolutionConfig->name=%s", resolutionConfig->name);
    LOG_TRACE("name=%s", name);
    LOG_TRACE("filterResolutionName done");
    return !strcmp(resolutionConfig->name, name);
}