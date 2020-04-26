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

#include "systems.h"
#include "config.h"

static system_t *systems = NULL;

static system_t *addSystemToList(system_t *list, system_t *item);

static system_t *createSystemItem(char *name, char *fullname, char *path);

static void freeSystemList(system_t *system);

void initSystems() {
#ifdef ENABLE_NINTENDO
    systems = addSystemToList(systems, createSystemItem("gb", "Game Boy", "/gb"));
    systems = addSystemToList(systems, createSystemItem("gbc", "Game Boy Color", "/gbc"));
    systems = addSystemToList(systems, createSystemItem("gba", "Game Boy Advance", "/gba"));
    systems = addSystemToList(systems, createSystemItem("nes", "Nintendo Entertainment System", "/nes"));
    systems = addSystemToList(systems, createSystemItem("snes", "Super Nintendo Entertainment System", "/snes"));
    systems = addSystemToList(systems, createSystemItem("gc", "Nintendo GameCube", "/gc"));
    systems = addSystemToList(systems, createSystemItem("nds", "Nintendo DS", "/nds"));
    systems = addSystemToList(systems, createSystemItem("n64", "Nintendo 64", "/n64"));
    systems = addSystemToList(systems, createSystemItem("wii", "Nintendo Wii", "/wii"));
    systems = addSystemToList(systems, createSystemItem("wiiu", "Nintendo Wii U", "/wiiu"));
    systems = addSystemToList(systems, createSystemItem("gw", "Game and Watch", "/gw"));
    systems = addSystemToList(systems, createSystemItem("fds", "Famicom Disk System", "/fds"));
#endif
#ifdef ENABLE_SEGA
    systems = addSystemToList(systems, createSystemItem("Dreamcast", "Sega Dreamcast", "/dreamcast"));
    systems = addSystemToList(systems, createSystemItem("gamegear", "Sega Gamegear", "/gamegear"));
    systems = addSystemToList(systems, createSystemItem("genesis", "Sega Genesis", "/genesis"));
    systems = addSystemToList(systems, createSystemItem("mastersystem", "Sega Master System", "/mastersystem"));
    systems = addSystemToList(systems, createSystemItem("megadrive", "Sega Mega Drive", "/megadrive"));
    systems = addSystemToList(systems, createSystemItem("saturn", "Sega Saturn", "/saturn"));
    systems = addSystemToList(systems, createSystemItem("sega32x", "Sega 32x", "/sega32x"));
    systems = addSystemToList(systems, createSystemItem("segacd", "Sega CD", "/segacd"));
    systems = addSystemToList(systems, createSystemItem("sg-1000", "Sega GS - 1000", "/sg-1000"));
#endif
#ifdef ENABLE_OTHER_HANDHELDS
    systems = addSystemToList(systems, createSystemItem("neogeo", "Neo Geo", "/neogeo"));
    systems = addSystemToList(systems, createSystemItem("ngp", "Neo Geo Pocket", "/ngp"));
    systems = addSystemToList(systems, createSystemItem("ngpc", "Neo Geo Pocket Color", "/ngpc"));
    systems = addSystemToList(systems, createSystemItem("wonderswan", "Wonderswan", "/wonderswan"));
    systems = addSystemToList(systems, createSystemItem("wonderswancolor", "Wonderswan Color", "/wonderswancolor"));
#endif
#ifdef ENABLE_PLAYSTATION
    systems = addSystemToList(systems, createSystemItem("psx", "Playstation", "/psx"));
    systems = addSystemToList(systems, createSystemItem("ps2", "Playstation 2", "/ps2"));
    systems = addSystemToList(systems, createSystemItem("psp", "Playstation Portable", "/psp"));
#endif
#ifdef ENABLE_ATARI
    systems = addSystemToList(systems, createSystemItem("atari2600", "Atari 2600", "/atari2600"));
    systems = addSystemToList(systems, createSystemItem("atari7800", "Atari 7800 Prosystem", "/atari7800"));
    systems = addSystemToList(systems, createSystemItem("atarijaguar", "Atari Jaguar", "/atarijaguar"));
    systems = addSystemToList(systems, createSystemItem("atarilynx", "Atari Lynx", "/atarilynx"));
    systems = addSystemToList(systems, createSystemItem("atarist", "Atari ST, STE, Falcon", "/atarist"));
#endif
#ifdef ENABLE_PCLIKE
    systems = addSystemToList(systems, createSystemItem("c64", "Commodore 64", "/c64"));
    systems = addSystemToList(systems, createSystemItem("amiga", "Amiga", "/amiga"));
    systems = addSystemToList(systems, createSystemItem("msx", "MSX", "/msx"));
    systems = addSystemToList(systems, createSystemItem("zxspectrum", "ZX Spectrum", "/zxspectrum"));
    systems = addSystemToList(systems, createSystemItem("pc", "PC (x86)", "/pc"));
#endif
#ifdef ENABLE_ARCADE
    systems = addSystemToList(systems, createSystemItem("fba", "Final Burn Alpha", "/fba"));
    systems = addSystemToList(systems, createSystemItem("vectrex", "Vectrex", "/vectrex"));
#endif
#ifdef ENABLE_OTHER_CONSOLES
    systems = addSystemToList(systems, createSystemItem("3do", "3do", "/3do"));
    systems = addSystemToList(systems, createSystemItem("pcengine", "TurboGrafx 16 (PC Engine)", "/pcengine"));
    systems = addSystemToList(systems, createSystemItem("pcfx", "PC-FX", "/pcfx"));
    systems = addSystemToList(systems, createSystemItem("videopac", "Odyssey 2 / Videopac", "/videopac"));
    systems = addSystemToList(systems, createSystemItem("virtualboy", "Virtual Boy", "/virtualboy"));
#endif
#ifdef ENABLE_FRAMEWORKS
    systems = addSystemToList(systems, createSystemItem("love", "LOVE", "/love"));
    systems = addSystemToList(systems, createSystemItem("mame", "MAME", "/mame"));
    systems = addSystemToList(systems, createSystemItem("scummvm", "ScummVM", "/scummvm"));
#endif
}

system_t *getSystemList() {
    return systems;
}

char *getRomDownloadPath(system_t *system, char *filename) {
    int arrayLength = strlen(ROM_BASE_DIR) + strlen(system->path) + 1 + strlen(filename) + 1;
    char *filepath = malloc(sizeof(char) * arrayLength);
    memset(filepath, '\0', arrayLength);
    strcat(filepath, ROM_BASE_DIR);
    strcat(filepath, system->path);
    strcat(filepath, "/");
    strcat(filepath, filename);

    return filepath;
}

void freeSystems() {
    freeSystemList(systems);
}

static system_t *createSystemItem(char *name, char *fullname, char *path) {
    system_t *system = malloc(sizeof(system_t));
    system->name = name;
    system->fullname = fullname;
    system->path = path;
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
    free(system);
}

