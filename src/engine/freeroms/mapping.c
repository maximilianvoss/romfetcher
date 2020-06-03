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

char *freeroms_deviceMapping(system_t *system) {
    char *deviceString = NULL;

    if (!strcmp(system->name, "amiga")) {
        deviceString = "amiga";
    } else if (!strcmp(system->name, "atari2600")) {
        deviceString = "atari";
    } else if (!strcmp(system->name, "atari7800")) {
        deviceString = "atari";
    } else if (!strcmp(system->name, "atarijaguar")) {
        deviceString = "atari_jaguar";
    } else if (!strcmp(system->name, "atarilynx")) {
        deviceString = "atari_lynx";
    } else if (!strcmp(system->name, "dreamcast")) {
        deviceString = "sega_dreamcast";
    } else if (!strcmp(system->name, "gamegear")) {
        deviceString = "game_gear";
    } else if (!strcmp(system->name, "gb")) {
        deviceString = "gameboy_color";
    } else if (!strcmp(system->name, "gba")) {
        deviceString = "gba";
    } else if (!strcmp(system->name, "gbc")) {
        deviceString = "gameboy_color";
    } else if (!strcmp(system->name, "gc")) {
        deviceString = "nintendo_gamecube";
    } else if (!strcmp(system->name, "genesis")) {
        deviceString = "sega_genesis_32x";
    } else if (!strcmp(system->name, "mame")) {
        deviceString = "mame";
    } else if (!strcmp(system->name, "mastersystem")) {
        deviceString = "sega_master_system";
    } else if (!strcmp(system->name, "megadrive")) {
        deviceString = "sega_genesis_32x";
    } else if (!strcmp(system->name, "n64")) {
        deviceString = "n64";
    } else if (!strcmp(system->name, "nds")) {
        deviceString = "nds";
    } else if (!strcmp(system->name, "neogeo")) {
        deviceString = "neogeo";
    } else if (!strcmp(system->name, "nes")) {
        deviceString = "nes";
    } else if (!strcmp(system->name, "ngp")) {
        deviceString = "neogeo_pocket";
    } else if (!strcmp(system->name, "ngpc")) {
        deviceString = "neogeo_pocket";
    } else if (!strcmp(system->name, "psp")) {
        deviceString = "psp";
    } else if (!strcmp(system->name, "psx")) {
        deviceString = "psx";
    } else if (!strcmp(system->name, "snes")) {
        deviceString = "snes";
    } else if (!strcmp(system->name, "wonderswan")) {
        deviceString = "wonderswan";
    }

    return deviceString;
}