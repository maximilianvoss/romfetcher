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

char *wowroms_deviceMapping(system_t *system) {
    char *deviceString = NULL;

    if (!strcmp(system->name, "amiga")) {
        deviceString = "amiga";
    } else if (!strcmp(system->name, "atari2600")) {
        deviceString = "atari%2B2600";
    } else if (!strcmp(system->name, "atari7800")) {
        deviceString = "atari-%2B7800";
    } else if (!strcmp(system->name, "atarijaguar")) {
        deviceString = "atari%2Bjaguar";
    } else if (!strcmp(system->name, "atarilynx")) {
        deviceString = "atari%2Blynx";
    } else if (!strcmp(system->name, "atarist")) {
        deviceString = "atari%2Bst";
    } else if (!strcmp(system->name, "c64")) {
        deviceString = "commodore%2B64";
    } else if (!strcmp(system->name, "fds")) {
        deviceString = "nintendo%2Bfamicom%2Bdisk%2Bsystem";
    } else if (!strcmp(system->name, "gamegear")) {
        deviceString = "sega%2Bgame%2Bgear";
    } else if (!strcmp(system->name, "gb")) {
        deviceString = "nintendo%2Bgameboy";
    } else if (!strcmp(system->name, "gba")) {
        deviceString = "nintendo%2Bgameboy%2Badvanced";
    } else if (!strcmp(system->name, "gbc")) {
        deviceString = "nintendo%2Bgameboy%2Bcolor";
    } else if (!strcmp(system->name, "genesis") || !strcmp(system->name, "megadrive") ||
               !strcmp(system->name, "segacd")) {
        deviceString = "sega%2Bgenesis%252Fmegadrive";
    } else if (!strcmp(system->name, "mame")) {
        deviceString = "mame%2B0.37b5";
    } else if (!strcmp(system->name, "mastersystem")) {
        deviceString = "sega%2Bmaster%2Bsystem";
    } else if (!strcmp(system->name, "megadrive")) {
        deviceString = "sega%2Bgenesis%252Fmegadrive";
    } else if (!strcmp(system->name, "n64")) {
        deviceString = "nintendo%2B64";
    } else if (!strcmp(system->name, "nds")) {
        deviceString = "nintendo%2Bds";
    } else if (!strcmp(system->name, "neogeo")) {
        deviceString = "neo%2Bgeo";
    } else if (!strcmp(system->name, "nes")) {
        deviceString = "nintendo%2Bentertainment%2Bsystem";
    } else if (!strcmp(system->name, "ngp")) {
        deviceString = "neo%2Bgeo%2Bpocket";
    } else if (!strcmp(system->name, "ngpc")) {
        deviceString = "neo%2Bgeo%2Bpocket%2Bcolor";
    } else if (!strcmp(system->name, "pcengine")) {
        deviceString = "pc%2Bengine%252Fturbografx%2B16";
    } else if (!strcmp(system->name, "ps2")) {
        deviceString = "playstation%2B2";
    } else if (!strcmp(system->name, "psp")) {
        deviceString = "playstation%2Bportable";
    } else if (!strcmp(system->name, "psx")) {
        deviceString = "playstation";
    } else if (!strcmp(system->name, "sega32x")) {
        deviceString = "sega%2B32x";
    } else if (!strcmp(system->name, "sg-1000")) {
        deviceString = "sega%2Bsg1000";
    } else if (!strcmp(system->name, "snes")) {
        deviceString = "super%2Bnintendo";
    } else if (!strcmp(system->name, "virtualboy")) {
        deviceString = "nintendo%2Bvirtual%2Bboy";
    } else if (!strcmp(system->name, "wonderswan")) {
        deviceString = "bandai%2Bwonderswan";
    } else if (!strcmp(system->name, "wonderswancolor")) {
        deviceString = "bandai%2Bwonderswan%2Bcolor";
    } else if (!strcmp(system->name, "zxspectrum")) {
        deviceString = "zx%2Bspectrum";
    }

    return deviceString;
}