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

#include <stdio.h>
#include <string.h>
#include "sytems.h"

static void addDefaultSystems(sqlite3 *db);

static system_t *addSystemToList(system_t *list, system_t *item);

static system_t *createSystemItem(char *name, char *fullname, char *path, int active);

static void freeSystemList(system_t *system);

static int database_systemListCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names);

void database_systemsInitTable(sqlite3 *db) {
    char *err_msg = 0;
    char *query = "CREATE TABLE systems (id INT PRIMARY KEY, name TEXT, fullname TEXT, path TEXT, active INT)";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to create table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    addDefaultSystems(db);
}

void database_systemAdd(sqlite3 *db, char *name, char *fullname, char *path, int active) {
    char *query = "INSERT INTO systems (name, fullname, path, active) VALUES (@name, @fullname, @path, @active)";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, 0);
    if (rc == SQLITE_OK) {
        int idx;
        idx = sqlite3_bind_parameter_index(stmt, "@name");
        sqlite3_bind_text(stmt, idx, name, strlen(name), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@fullname");
        sqlite3_bind_text(stmt, idx, fullname, strlen(fullname), NULL);

        idx = sqlite3_bind_parameter_index(stmt, "@path");
        sqlite3_bind_text(stmt, idx, path, strlen(path), NULL);

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

void database_systemStore(sqlite3 *db, system_t *systems) {
    char *err_msg = 0;
    char *query = "DELETE FROM systems";

    int rc = sqlite3_exec(db, query, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Failed to delete table\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    while (systems != NULL) {
        database_systemAdd(db, systems->name, systems->fullname, systems->path, systems->active);
        systems = systems->next;
    }
}

static int database_systemListCallback(void *p_data, int num_fields, char **p_fields, char **p_col_names) {
    system_t *system = createSystemItem(p_fields[0], p_fields[1], p_fields[2], atoi(p_fields[3]));
    system_t **systemPtr = (system_t **) p_data;
    *systemPtr = addSystemToList(*systemPtr, system);
    return 0;
}

system_t *database_systemList(app_t *app, uint8_t active) {
    char *errmsg;
    char *query;
    if (active) {
        query = "SELECT name, fullname, path, active FROM systems WHERE active=1";
    } else {
        query = "SELECT name, fullname, path, active FROM systems";
    }

    system_t *systems = NULL;
    int ret = sqlite3_exec(app->database.db, query, database_systemListCallback, &systems, &errmsg);
    if (ret != SQLITE_OK) {
        printf("Error in select statement %s [%s].\n", query, errmsg);
        return NULL;
    }
    return systems;
}

void database_systemsDestroy(system_t *systems) {
    if (systems != NULL) {
        freeSystemList(systems);
    }
}

static void addDefaultSystems(sqlite3 *db) {
    database_systemAdd(db, "gb", "Game Boy", "/gb", 1);
    database_systemAdd(db, "gbc", "Game Boy Color", "/gbc", 1);
    database_systemAdd(db, "gba", "Game Boy Advance", "/gba", 1);
    database_systemAdd(db, "nes", "Nintendo Entertainment System", "/nes", 1);
    database_systemAdd(db, "snes", "Super Nintendo Entertainment System", "/snes", 1);
    database_systemAdd(db, "gc", "Nintendo GameCube", "/gc", 0);
    database_systemAdd(db, "nds", "Nintendo DS", "/nds", 1);
    database_systemAdd(db, "n64", "Nintendo 64", "/n64", 0);
    database_systemAdd(db, "wii", "Nintendo Wii", "/wii", 1);
    database_systemAdd(db, "wiiu", "Nintendo Wii U", "/wiiu", 0);
    database_systemAdd(db, "gw", "Game and Watch", "/gw", 0);
    database_systemAdd(db, "fds", "Famicom Disk System", "/fds", 0);

    database_systemAdd(db, "dreamcast", "Sega Dreamcast", "/dreamcast", 1);
    database_systemAdd(db, "gamegear", "Sega Gamegear", "/gamegear", 1);
    database_systemAdd(db, "genesis", "Sega Genesis", "/genesis", 1);
    database_systemAdd(db, "mastersystem", "Sega Master System", "/mastersystem", 1);
    database_systemAdd(db, "megadrive", "Sega Mega Drive", "/megadrive", 1);
    database_systemAdd(db, "saturn", "Sega Saturn", "/saturn", 1);
    database_systemAdd(db, "sega32x", "Sega 32x", "/sega32x", 1);
    database_systemAdd(db, "segacd", "Sega CD", "/segacd", 1);
    database_systemAdd(db, "sg-1000", "Sega GS - 1000", "/sg-1000", 1);

    database_systemAdd(db, "neogeo", "Neo Geo", "/neogeo", 1);
    database_systemAdd(db, "ngp", "Neo Geo Pocket", "/ngp", 1);
    database_systemAdd(db, "ngpc", "Neo Geo Pocket Color", "/ngpc", 1);
    database_systemAdd(db, "wonderswan", "Wonderswan", "/wonderswan", 1);
    database_systemAdd(db, "wonderswancolor", "Wonderswan Color", "/wonderswancolor", 1);

    database_systemAdd(db, "psx", "Playstation", "/psx", 1);
    database_systemAdd(db, "ps2", "Playstation 2", "/ps2", 1);
    database_systemAdd(db, "psp", "Playstation Portable", "/psp", 1);

    database_systemAdd(db, "atari2600", "Atari 2600", "/atari2600", 0);
    database_systemAdd(db, "atari7800", "Atari 7800 Prosystem", "/atari7800", 0);
    database_systemAdd(db, "atarijaguar", "Atari Jaguar", "/atarijaguar", 0);
    database_systemAdd(db, "atarilynx", "Atari Lynx", "/atarilynx", 0);
    database_systemAdd(db, "atarist", "Atari ST, STE, Falcon", "/atarist", 0);

    database_systemAdd(db, "c64", "Commodore 64", "/c64", 1);
    database_systemAdd(db, "amiga", "Amiga", "/amiga", 1);
    database_systemAdd(db, "msx", "MSX", "/msx", 0);
    database_systemAdd(db, "zxspectrum", "ZX Spectrum", "/zxspectrum", 0);
    database_systemAdd(db, "pc", "PC (x86)", "/pc", 0);

    database_systemAdd(db, "fba", "Final Burn Alpha", "/fba", 0);
    database_systemAdd(db, "vectrex", "Vectrex", "/vectrex", 1);

    database_systemAdd(db, "3do", "3do", "/3do", 0);
    database_systemAdd(db, "pcengine", "TurboGrafx 16 (PC Engine)", "/pcengine", 0);
    database_systemAdd(db, "pcfx", "PC-FX", "/pcfx", 0);
    database_systemAdd(db, "videopac", "Odyssey 2 / Videopac", "/videopac", 0);
    database_systemAdd(db, "virtualboy", "Virtual Boy", "/virtualboy", 0);

    database_systemAdd(db, "love", "LOVE", "/love", 0);
    database_systemAdd(db, "mame", "MAME", "/mame", 0);
    database_systemAdd(db, "scummvm", "ScummVM", "/scummvm", 0);
}

static system_t *createSystemItem(char *name, char *fullname, char *path, int active) {
    int length;
    system_t *system = calloc(1, sizeof(system_t));
    if (name != NULL) {
        length = strlen(name) + 1;
        system->name = (char *) calloc(length, sizeof(char));
        strcpy(system->name, name);
    }
    if (fullname != NULL) {
        length = strlen(fullname) + 1;
        system->fullname = (char *) calloc(length, sizeof(char));
        strcpy(system->fullname, fullname);
    }
    if (path != NULL) {
        length = strlen(path) + 1;
        system->path = (char *) calloc(length, sizeof(char));
        strcpy(system->path, path);
    }
    system->active = active;
    system->prev = NULL;
    system->next = NULL;
    return system;
}

static system_t *addSystemToList(system_t *list, system_t *item) {
    system_t *ptr = list;
    if (ptr == NULL) {
        return item;
    }
    while (ptr->next != NULL) {
        ptr = ptr->next;
    }
    ptr->next = item;
    item->prev = ptr;
    return list;
}

static void freeSystemList(system_t *system) {
    if (system == NULL) {
        return;
    }
    system_t *next = system->next;
    if (next != NULL) {
        freeSystemList(next);
    }
    free(system->name);
    free(system->fullname);
    free(system->path);
    free(system);
}