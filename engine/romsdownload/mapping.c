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

char *romsdownload_deviceMapping(system_t *system) {
    char *deviceString = NULL;

    if (!strcmp(system->name, "3do")) {
//         deviceString = "3do";
    } else if (!strcmp(system->name, "amiga")) {
        deviceString = "6";
    } else if (!strcmp(system->name, "atari2600")) {
        deviceString = "13";
    } else if (!strcmp(system->name, "atari7800")) {
        deviceString = "15";
    } else if (!strcmp(system->name, "atarijaguar")) {
        deviceString = "17";
    } else if (!strcmp(system->name, "atarilynx")) {
        deviceString = "18";
    } else if (!strcmp(system->name, "atarist")) {
        deviceString = "19";
    } else if (!strcmp(system->name, "c64")) {
        deviceString = "31";
    } else if (!strcmp(system->name, "dreamcast")) {
//        deviceString = "dreamcast";
    } else if (!strcmp(system->name, "fba")) {
//        deviceString = "Final Burn Alpha";
    } else if (!strcmp(system->name, "fds")) {
        deviceString = "77";
    } else if (!strcmp(system->name, "gamegear")) {
        deviceString = "46";
    } else if (!strcmp(system->name, "gb")) {
        deviceString = "47";
    } else if (!strcmp(system->name, "gba")) {
        deviceString = "48";
    } else if (!strcmp(system->name, "gbc")) {
        deviceString = "49";
    } else if (!strcmp(system->name, "gc")) {
        deviceString = "50";
    } else if (!strcmp(system->name, "genesis")) {
        deviceString = "98";
    } else if (!strcmp(system->name, "gw")) {
//        deviceString = "Game and Watch";
    } else if (!strcmp(system->name, "love")) {
//        deviceString = "LÖVE";
    } else if (!strcmp(system->name, "mame")) {
        deviceString = "61";
    } else if (!strcmp(system->name, "mastersystem")) {
        deviceString = "99";
    } else if (!strcmp(system->name, "megadrive")) {
        deviceString = "98";
    } else if (!strcmp(system->name, "msx")) {
        deviceString = "67";
    } else if (!strcmp(system->name, "n64")) {
        deviceString = "75";
    } else if (!strcmp(system->name, "nds")) {
        deviceString = "76";
    } else if (!strcmp(system->name, "neogeo")) {
        deviceString = "70";
    } else if (!strcmp(system->name, "nes")) {
        deviceString = "73";
    } else if (!strcmp(system->name, "ngp")) {
        deviceString = "71";
    } else if (!strcmp(system->name, "ngpc")) {
        deviceString = "72";
    } else if (!strcmp(system->name, "pc")) {
//        deviceString = "PC (x86)";
    } else if (!strcmp(system->name, "pcengine")) {
        deviceString = "115";
    } else if (!strcmp(system->name, "pcfx")) {
//        deviceString = "PC-FX";
    } else if (!strcmp(system->name, "ps2")) {
        deviceString = "89";
    } else if (!strcmp(system->name, "psp")) {
        deviceString = "91";
    } else if (!strcmp(system->name, "psx")) {
        deviceString = "88";
    } else if (!strcmp(system->name, "saturn")) {
//        deviceString = "Sega Saturn";
    } else if (!strcmp(system->name, "scummvm")) {
//        deviceString = "ScummVM";
    } else if (!strcmp(system->name, "sega32x")) {
        deviceString = "95";
    } else if (!strcmp(system->name, "segacd")) {
        deviceString = "98";
    } else if (!strcmp(system->name, "sg-1000")) {
        deviceString = "102";
    } else if (!strcmp(system->name, "snes")) {
        deviceString = "110";
    } else if (!strcmp(system->name, "vectrex")) {
        deviceString = "52";
    } else if (!strcmp(system->name, "videopac")) {
        deviceString = "87";
    } else if (!strcmp(system->name, "virtualboy")) {
        deviceString = "79";
    } else if (!strcmp(system->name, "wii")) {
        deviceString = "80";
    } else if (!strcmp(system->name, "wiiu")) {
//        deviceString = "Nintendo Wii U";
    } else if (!strcmp(system->name, "wonderswan")) {
        deviceString = "121";
    } else if (!strcmp(system->name, "wonderswancolor")) {
//        deviceString = "Wonderswan Color";
    } else if (!strcmp(system->name, "zxspectrum")) {
        deviceString = "124";
    }

    return deviceString;
}