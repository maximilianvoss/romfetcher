//
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

static system_t *createSystem(char *name, char *fullname, char *path) {
    system_t *system = calloc(1, sizeof(system_t));
    system->prev = NULL;
    system->next = NULL;
    system->active = 1;
    system->name = name;
    system->fullname = fullname;
    system->path = path;
    return system;
}

void testsystems_init() {
    gb = createSystem("gb", "Game Boy", "/gb");
    gbc = createSystem("gbc", "Game Boy Color", "/gbc");
    gba = createSystem("gba", "Game Boy Advance", "/gba");
    nes = createSystem("nes", "Nintendo Entertainment System", "/nes");
    snes = createSystem("snes", "Super Nintendo Entertainment System", "/snes");
    gc = createSystem("gc", "Nintendo GameCube", "/gc");
    nds = createSystem("nds", "Nintendo DS", "/nds");
    n64 = createSystem("n64", "Nintendo 64", "/n64");
    wii = createSystem("wii", "Nintendo Wii", "/wii");
    wiiu = createSystem("wiiu", "Nintendo Wii U", "/wiiu");
    gw = createSystem("gw", "Game and Watch", "/gw");
    fds = createSystem("fds", "Famicom Disk System", "/fds");
    dreamcast = createSystem("dreamcast", "Sega Dreamcast", "/dreamcast");
    gamegear = createSystem("gamegear", "Sega Gamegear", "/gamegear");
    genesis = createSystem("genesis", "Sega Genesis", "/genesis");
    mastersystem = createSystem("mastersystem", "Sega Master System", "/mastersystem");
    megadrive = createSystem("megadrive", "Sega Mega Drive", "/megadrive");
    saturn = createSystem("saturn", "Sega Saturn", "/saturn");
    sega32x = createSystem("sega32x", "Sega 32x", "/sega32x");
    segacd = createSystem("segacd", "Sega CD", "/segacd");
    sg1000 = createSystem("sg-1000", "Sega GS - 1000", "/sg-1000");
    neogeo = createSystem("neogeo", "Neo Geo", "/neogeo");
    ngp = createSystem("ngp", "Neo Geo Pocket", "/ngp");
    ngpc = createSystem("ngpc", "Neo Geo Pocket Color", "/ngpc");
    wonderswan = createSystem("wonderswan", "Wonderswan", "/wonderswan");
    wonderswancolor = createSystem("wonderswancolor", "Wonderswan Color", "/wonderswancolor");
    psx = createSystem("psx", "Playstation", "/psx");
    ps2 = createSystem("ps2", "Playstation 2", "/ps2");
    psp = createSystem("psp", "Playstation Portable", "/psp");
    atari2600 = createSystem("atari2600", "Atari 2600", "/atari2600");
    atari7800 = createSystem("atari7800", "Atari 7800 Prosystem", "/atari7800");
    atarijaguar = createSystem("atarijaguar", "Atari Jaguar", "/atarijaguar");
    atarilynx = createSystem("atarilynx", "Atari Lynx", "/atarilynx");
    atarist = createSystem("atarist", "Atari ST, STE, Falcon", "/atarist");
    c64 = createSystem("c64", "Commodore 64", "/c64");
    amiga = createSystem("amiga", "Amiga", "/amiga");
    msx = createSystem("msx", "MSX", "/msx");
    zxspectrum = createSystem("zxspectrum", "ZX Spectrum", "/zxspectrum");
    pc = createSystem("pc", "PC (x86)", "/pc");
    fba = createSystem("fba", "Final Burn Alpha", "/fba");
    vectrex = createSystem("vectrex", "Vectrex", "/vectrex");
    threeDo = createSystem("3do", "3do", "/3do");
    pcengine = createSystem("pcengine", "TurboGrafx 16 (PC Engine)", "/pcengine");
    pcfx = createSystem("pcfx", "PC-FX", "/pcfx");
    videopac = createSystem("videopac", "Odyssey 2 / Videopac", "/videopac");
    virtualboy = createSystem("virtualboy", "Virtual Boy", "/virtualboy");
    love = createSystem("love", "LOVE", "/love");
    mame = createSystem("mame", "MAME", "/mame");
    scummvm = createSystem("scummvm", "ScummVM", "/scummvm");
}