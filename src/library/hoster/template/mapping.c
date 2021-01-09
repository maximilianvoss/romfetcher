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

#include "mapping.h"

char *_template_deviceMapping(system_t *system) {
    char *deviceString = NULL;

    if (!strcmp(system->name, "3do")) {
        deviceString = "3do";
    } else if (!strcmp(system->name, "amiga")) {
        deviceString = "Amiga";
    } else if (!strcmp(system->name, "atari2600")) {
        deviceString = "Atari 2600";
    } else if (!strcmp(system->name, "atari7800")) {
        deviceString = "Atari 7800";
    } else if (!strcmp(system->name, "atarijaguar")) {
        deviceString = "Atari Jaguar";
    } else if (!strcmp(system->name, "atarilynx")) {
        deviceString = "Atari Lynx";
    } else if (!strcmp(system->name, "atarist")) {
        deviceString = "Atari ST";
    } else if (!strcmp(system->name, "c64")) {
        deviceString = "Commodore 64";
    } else if (!strcmp(system->name, "dreamcast")) {
        deviceString = "Sega Dreamcast";
    } else if (!strcmp(system->name, "fba")) {
        deviceString = "Final Burn Alpha";
    } else if (!strcmp(system->name, "gamegear")) {
        deviceString = "Sega Game Gear";
    } else if (!strcmp(system->name, "gb")) {
        deviceString = "Nintendo Gameboy";
    } else if (!strcmp(system->name, "gba")) {
        deviceString = "Nintendo Gameboy Advanced";
    } else if (!strcmp(system->name, "gbc")) {
        deviceString = "Nintendo Gameboy Color";
    } else if (!strcmp(system->name, "gc")) {
        deviceString = "Nintendo GameCube";
    } else if (!strcmp(system->name, "genesis") || !strcmp(system->name, "megadrive") ||
               !strcmp(system->name, "segacd")) {
        deviceString = "Sega Genesis";
    } else if (!strcmp(system->name, "gw")) {
        deviceString = "Nintendo Game and Watch";
    } else if (!strcmp(system->name, "love")) {
        deviceString = "LÖVE";
    } else if (!strcmp(system->name, "mame")) {
        deviceString = "MAME";
    } else if (!strcmp(system->name, "mastersystem")) {
        deviceString = "Sega Master System";
    } else if (!strcmp(system->name, "msx")) {
        deviceString = "MSX Computer";
    } else if (!strcmp(system->name, "n64")) {
        deviceString = "Nintendo 64";
    } else if (!strcmp(system->name, "nds")) {
        deviceString = "Nintendo DS";
    } else if (!strcmp(system->name, "neogeo")) {
        deviceString = "Neo Geo";
    } else if (!strcmp(system->name, "nes") || !strcmp(system->name, "fds")) {
        deviceString = "Nintendo Entertainment System || Famicom Disk Station";
    } else if (!strcmp(system->name, "ngp")) {
        deviceString = "Neo Geo Pocket";
    } else if (!strcmp(system->name, "ngpc")) {
        deviceString = "Neo Geo Pocket Color";
    } else if (!strcmp(system->name, "pc")) {
        deviceString = "PC (x86)";
    } else if (!strcmp(system->name, "pcengine")) {
        deviceString = "PCEngine / Turbografx 16";
    } else if (!strcmp(system->name, "pcfx")) {
        deviceString = "PC-FX";
    } else if (!strcmp(system->name, "ps2")) {
        deviceString = "Playstation 2";
    } else if (!strcmp(system->name, "psp")) {
        deviceString = "Playstation Portable";
    } else if (!strcmp(system->name, "psx")) {
        deviceString = "Playstation";
    } else if (!strcmp(system->name, "saturn")) {
        deviceString = "Sega Saturn";
    } else if (!strcmp(system->name, "scummvm")) {
        deviceString = "ScummVM";
    } else if (!strcmp(system->name, "sega32x")) {
        deviceString = "Sega-32x";
    } else if (!strcmp(system->name, "sg-1000")) {
        deviceString = "Sega SG1000";
    } else if (!strcmp(system->name, "snes")) {
        deviceString = "Super Nintendo";
    } else if (!strcmp(system->name, "vectrex")) {
        deviceString = "GCE Vectrex";
    } else if (!strcmp(system->name, "videopac")) {
        deviceString = "Philips Videopac";
    } else if (!strcmp(system->name, "virtualboy")) {
        deviceString = "Nintendo Virtual Boy";
    } else if (!strcmp(system->name, "wii")) {
        deviceString = "Nintendo Wii";
    } else if (!strcmp(system->name, "wiiu")) {
        deviceString = "Nintendo Wii U";
    } else if (!strcmp(system->name, "wonderswan")) {
        deviceString = "Wonderswan";
    } else if (!strcmp(system->name, "wonderswancolor")) {
        deviceString = "Wonderswan Color";
    } else if (!strcmp(system->name, "zxspectrum")) {
        deviceString = "ZX Spectrum";
    }

    return deviceString;
}