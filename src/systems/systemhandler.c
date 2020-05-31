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

#include "systemhandler.h"
#include "../database/linkedlist.h"
#include "../config.h"

static system_t *createSystem(char *name, char *fullname, char *path, int active);

void systemhandler_init(app_t *app) {
    app->systems.all = NULL;
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("gb", "Game Boy", "/gb", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("gba", "Game Boy Advance", "/gba", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("gbc", "Game Boy Color", "/gbc", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("nes", "Nintendo Entertainment System", "/nes", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("snes", "Super Nintendo Entertainment System", "/snes",
                                                             1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("gc", "Nintendo GameCube", "/gc", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("nds", "Nintendo DS", "/nds", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("n64", "Nintendo 64", "/n64", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("wii", "Nintendo Wii", "/wii", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("wiiu", "Nintendo Wii U", "/wiiu", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("gw", "Game and Watch", "/gw", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("fds", "Famicom Disk System", "/fds", 0));

    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("dreamcast", "Sega Dreamcast", "/dreamcast", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("gamegear", "Sega Gamegear", "/gamegear", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("genesis", "Sega Genesis", "/genesis", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("mastersystem", "Sega Master System", "/mastersystem", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("megadrive", "Sega Mega Drive", "/megadrive", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("saturn", "Sega Saturn", "/saturn", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("sega32x", "Sega 32x", "/sega32x", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("segacd", "Sega CD", "/segacd", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("sg-1000", "Sega GS - 1000", "/sg-1000", 1));

    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("neogeo", "Neo Geo", "/neogeo", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("ngp", "Neo Geo Pocket", "/ngp", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("ngpc", "Neo Geo Pocket Color", "/ngpc", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("wonderswan", "Wonderswan", "/wonderswan", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("wonderswancolor", "Wonderswan Color", "/wonderswancolor",
                                                             1));

    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("psx", "Playstation", "/psx", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("ps2", "Playstation 2", "/ps2", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("psp", "Playstation Portable", "/psp", 1));

    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("atari2600", "Atari 2600", "/atari2600", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("atari7800", "Atari 7800 Prosystem", "/atari7800", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("atarijaguar", "Atari Jaguar", "/atarijaguar", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("atarilynx", "Atari Lynx", "/atarilynx", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("atarist", "Atari ST, STE, Falcon", "/atarist", 0));

    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("c64", "Commodore 64", "/c64", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("amiga", "Amiga", "/amiga", 1));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("msx", "MSX", "/msx", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("zxspectrum", "ZX Spectrum", "/zxspectrum", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("pc", "PC (x86)", "/pc", 0));

    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("fba", "Final Burn Alpha", "/fba", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("vectrex", "Vectrex", "/vectrex", 1));

    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("3do", "3do", "/3do", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("pcengine", "TurboGrafx 16 (PC Engine)", "/pcengine", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("pcfx", "PC-FX", "/pcfx", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("videopac", "Odyssey 2 / Videopac", "/videopac", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all,
                                                createSystem("virtualboy", "Virtual Boy", "/virtualboy", 0));

    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("love", "LOVE", "/love", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("mame", "MAME", "/mame", 0));
    app->systems.all = linkedlist_appendElement(app->systems.all, createSystem("scummvm", "ScummVM", "/scummvm", 0));

    app->systems.all = linkedlist_sort(app->systems.all);
    databaselinkedlist_loadActivities(app->database.db, DATABASE_TABLE_SYSTEMS, (linkedlist_t *) app->systems.all);
    app->systems.active = linkedlist_getFirstActive(app->systems.all);
}

void systemhandler_destroy(app_t *app) {
    linkedlist_freeList(app->systems.all, NULL);
}

static system_t *createSystem(char *name, char *fullname, char *path, int active) {
    system_t *system = calloc(1, sizeof(system_t));
    system->name = name;
    system->fullname = fullname;
    system->active = active;
    system->path = path;
    return system;
}