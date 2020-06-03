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

#include "mapping.h"

char *romsmode_deviceMapping(system_t *system) {
    char *deviceString = NULL;

    if (!strcmp(system->name, "amiga")) {
        deviceString = "amiga-500";
    } else if (!strcmp(system->name, "atari2600")) {
        deviceString = "atari-2600";
    } else if (!strcmp(system->name, "atari7800")) {
        deviceString = "atari-7800";
    } else if (!strcmp(system->name, "atarijaguar")) {
        deviceString = "atari-jaguar";
    } else if (!strcmp(system->name, "atarilynx")) {
        deviceString = "atari-lynx";
    } else if (!strcmp(system->name, "atarist")) {
        deviceString = "atari-st";
    } else if (!strcmp(system->name, "c64")) {
        deviceString = "commodore-64";
    } else if (!strcmp(system->name, "dreamcast")) {
        deviceString = "dreamcast";
    } else if (!strcmp(system->name, "fds")) {
        deviceString = "nintendo";
    } else if (!strcmp(system->name, "gamegear")) {
        deviceString = "game-gear";
    } else if (!strcmp(system->name, "gb")) {
        deviceString = "gameboy";
    } else if (!strcmp(system->name, "gba")) {
        deviceString = "gameboy-advanced";
    } else if (!strcmp(system->name, "gbc")) {
        deviceString = "gameboy-color";
    } else if (!strcmp(system->name, "gc")) {
        deviceString = "gamecube";
    } else if (!strcmp(system->name, "genesis")) {
        deviceString = "sega-genesis";
    } else if (!strcmp(system->name, "mame")) {
        deviceString = "mame";
    } else if (!strcmp(system->name, "mastersystem")) {
        deviceString = "sega-master-system";
    } else if (!strcmp(system->name, "megadrive")) {
        deviceString = "sega-genesis";
    } else if (!strcmp(system->name, "msx")) {
        deviceString = "msx-computer";
    } else if (!strcmp(system->name, "n64")) {
        deviceString = "nintendo-64";
    } else if (!strcmp(system->name, "nds")) {
        deviceString = "nintendo-ds";
    } else if (!strcmp(system->name, "neogeo")) {
        deviceString = "neo-geo";
    } else if (!strcmp(system->name, "nes")) {
        deviceString = "nintendo";
    } else if (!strcmp(system->name, "ngp")) {
        deviceString = "neo-geo-pocket";
    } else if (!strcmp(system->name, "ngpc")) {
        deviceString = "neo-geo-pocket-color";
    } else if (!strcmp(system->name, "pcengine")) {
        deviceString = "turbografx-16";
    } else if (!strcmp(system->name, "ps2")) {
        deviceString = "playstation-2";
    } else if (!strcmp(system->name, "psp")) {
        deviceString = "playstation-portable";
    } else if (!strcmp(system->name, "psx")) {
        deviceString = "playstation";
    } else if (!strcmp(system->name, "sega32x")) {
        deviceString = "sega-32x";
    } else if (!strcmp(system->name, "segacd")) {
        deviceString = "sega-genesis";
    } else if (!strcmp(system->name, "sg-1000")) {
        deviceString = "sega-sg1000";
    } else if (!strcmp(system->name, "snes")) {
        deviceString = "super-nintendo";
    } else if (!strcmp(system->name, "vectrex")) {
        deviceString = "gce-vectrex";
    } else if (!strcmp(system->name, "videopac")) {
        deviceString = "philips-videopac";
    } else if (!strcmp(system->name, "virtualboy")) {
        deviceString = "nintendo-virtual-boy";
    } else if (!strcmp(system->name, "wii")) {
        deviceString = "nintendo-wii";
    } else if (!strcmp(system->name, "wonderswan")) {
        deviceString = "wonderswan";
    } else if (!strcmp(system->name, "zxspectrum")) {
        deviceString = "zx-spectrum";
    }

    return deviceString;
}