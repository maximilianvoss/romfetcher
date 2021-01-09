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

#include "systems.h"

static system_t *createSystem(char *name, char *fullname, char *path, int active);

system_t *systems_init() {
    system_t *systems = NULL;
    systems = linkedlist_appendElement(systems, createSystem("gb", "Game Boy", "/gb", 1));
    systems = linkedlist_appendElement(systems, createSystem("gba", "Game Boy Advance", "/gba", 1));
    systems = linkedlist_appendElement(systems, createSystem("gbc", "Game Boy Color", "/gbc", 1));
    systems = linkedlist_appendElement(systems, createSystem("nes", "Nintendo Entertainment System", "/nes", 1));
    systems = linkedlist_appendElement(systems,
                                       createSystem("snes", "Super Nintendo Entertainment System", "/snes", 1));
    systems = linkedlist_appendElement(systems, createSystem("gc", "Nintendo GameCube", "/gc", 0));
    systems = linkedlist_appendElement(systems, createSystem("nds", "Nintendo DS", "/nds", 1));
    systems = linkedlist_appendElement(systems, createSystem("n64", "Nintendo 64", "/n64", 0));
    systems = linkedlist_appendElement(systems, createSystem("wii", "Nintendo Wii", "/wii", 1));
    systems = linkedlist_appendElement(systems, createSystem("wiiu", "Nintendo Wii U", "/wiiu", 0));
    systems = linkedlist_appendElement(systems, createSystem("gw", "Game and Watch", "/gw", 0));
    systems = linkedlist_appendElement(systems, createSystem("fds", "Famicom Disk System", "/fds", 0));

    systems = linkedlist_appendElement(systems, createSystem("dreamcast", "Sega Dreamcast", "/dreamcast", 1));
    systems = linkedlist_appendElement(systems, createSystem("gamegear", "Sega Gamegear", "/gamegear", 1));
    systems = linkedlist_appendElement(systems, createSystem("genesis", "Sega Genesis", "/genesis", 1));
    systems = linkedlist_appendElement(systems, createSystem("mastersystem", "Sega Master System", "/mastersystem", 1));
    systems = linkedlist_appendElement(systems, createSystem("megadrive", "Sega Mega Drive", "/megadrive", 1));
    systems = linkedlist_appendElement(systems, createSystem("saturn", "Sega Saturn", "/saturn", 1));
    systems = linkedlist_appendElement(systems, createSystem("sega32x", "Sega 32x", "/sega32x", 1));
    systems = linkedlist_appendElement(systems, createSystem("segacd", "Sega CD", "/segacd", 1));
    systems = linkedlist_appendElement(systems, createSystem("sg-1000", "Sega GS - 1000", "/sg-1000", 1));

    systems = linkedlist_appendElement(systems, createSystem("neogeo", "Neo Geo", "/neogeo", 1));
    systems = linkedlist_appendElement(systems, createSystem("ngp", "Neo Geo Pocket", "/ngp", 1));
    systems = linkedlist_appendElement(systems, createSystem("ngpc", "Neo Geo Pocket Color", "/ngpc", 1));
    systems = linkedlist_appendElement(systems, createSystem("wonderswan", "Wonderswan", "/wonderswan", 1));
    systems = linkedlist_appendElement(systems,
                                       createSystem("wonderswancolor", "Wonderswan Color", "/wonderswancolor", 1));

    systems = linkedlist_appendElement(systems, createSystem("psx", "Playstation", "/psx", 1));
    systems = linkedlist_appendElement(systems, createSystem("ps2", "Playstation 2", "/ps2", 1));
    systems = linkedlist_appendElement(systems, createSystem("psp", "Playstation Portable", "/psp", 1));

    systems = linkedlist_appendElement(systems, createSystem("atari2600", "Atari 2600", "/atari2600", 0));
    systems = linkedlist_appendElement(systems, createSystem("atari7800", "Atari 7800 Prosystem", "/atari7800", 0));
    systems = linkedlist_appendElement(systems, createSystem("atarijaguar", "Atari Jaguar", "/atarijaguar", 0));
    systems = linkedlist_appendElement(systems, createSystem("atarilynx", "Atari Lynx", "/atarilynx", 0));
    systems = linkedlist_appendElement(systems, createSystem("atarist", "Atari ST, STE, Falcon", "/atarist", 0));

    systems = linkedlist_appendElement(systems, createSystem("c64", "Commodore 64", "/c64", 1));
    systems = linkedlist_appendElement(systems, createSystem("amiga", "Amiga", "/amiga", 1));
    systems = linkedlist_appendElement(systems, createSystem("msx", "MSX", "/msx", 0));
    systems = linkedlist_appendElement(systems, createSystem("zxspectrum", "ZX Spectrum", "/zxspectrum", 0));
    systems = linkedlist_appendElement(systems, createSystem("pc", "PC (x86)", "/pc", 0));

    systems = linkedlist_appendElement(systems, createSystem("fba", "Final Burn Alpha", "/fba", 0));
    systems = linkedlist_appendElement(systems, createSystem("vectrex", "Vectrex", "/vectrex", 1));

    systems = linkedlist_appendElement(systems, createSystem("3do", "3do", "/3do", 0));
    systems = linkedlist_appendElement(systems, createSystem("pcengine", "TurboGrafx 16 (PC Engine)", "/pcengine", 0));
    systems = linkedlist_appendElement(systems, createSystem("pcfx", "PC-FX", "/pcfx", 0));
    systems = linkedlist_appendElement(systems, createSystem("videopac", "Odyssey 2 / Videopac", "/videopac", 0));
    systems = linkedlist_appendElement(systems, createSystem("virtualboy", "Virtual Boy", "/virtualboy", 0));

    systems = linkedlist_appendElement(systems, createSystem("love", "LOVE", "/love", 0));
    systems = linkedlist_appendElement(systems, createSystem("mame", "MAME", "/mame", 0));
    systems = linkedlist_appendElement(systems, createSystem("scummvm", "ScummVM", "/scummvm", 0));

    systems = linkedlist_sort(systems);

    return systems;
}

void systems_destroy(system_t *systems) {
    linkedlist_freeList(systems, NULL);
}

static system_t *createSystem(char *name, char *fullname, char *path, int active) {
    system_t *system = calloc(1, sizeof(system_t));
    system->name = name;
    system->fullname = fullname;
    system->active = active;
    system->path = path;
    return system;
}