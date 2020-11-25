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
#include <string.h>

char *romhustler_deviceMapping(system_t *system) {
    char *deviceString = NULL;

    if (!strcmp(system->name, "3do")) {
        deviceString = "10";
    } else if (!strcmp(system->name, "atari2600")) {
        deviceString = "2";
    } else if (!strcmp(system->name, "atari7800")) {
        deviceString = "5";
    } else if (!strcmp(system->name, "atarijaguar")) {
        deviceString = "13";
    } else if (!strcmp(system->name, "atarilynx")) {
        deviceString = "15";
    } else if (!strcmp(system->name, "dreamcast")) {
        deviceString = "6";
    } else if (!strcmp(system->name, "gamegear")) {
        deviceString = "12";
    } else if (!strcmp(system->name, "gb")) {
        deviceString = "8";
    } else if (!strcmp(system->name, "gba")) {
        deviceString = "9";
    } else if (!strcmp(system->name, "gbc")) {
        deviceString = "8";
    } else if (!strcmp(system->name, "gc")) {
        deviceString = "39";
    } else if (!strcmp(system->name, "genesis") || !strcmp(system->name, "megadrive")) {
        deviceString = "7";
    } else if (!strcmp(system->name, "mame")) {
        deviceString = "34";
    } else if (!strcmp(system->name, "mastersystem")) {
        deviceString = "29";
    } else if (!strcmp(system->name, "msx")) {
        deviceString = "16";
    } else if (!strcmp(system->name, "n64")) {
        deviceString = "19";
    } else if (!strcmp(system->name, "nds")) {
        deviceString = "21";
    } else if (!strcmp(system->name, "neogeo")) {
        deviceString = "31";
    } else if (!strcmp(system->name, "nes") || !strcmp(system->name, "fds")) {
        deviceString = "36";
    } else if (!strcmp(system->name, "ngp")) {
        deviceString = "30";
    } else if (!strcmp(system->name, "ngpc")) {
        deviceString = "30";
    } else if (!strcmp(system->name, "pcengine")) {
        deviceString = "35";
    } else if (!strcmp(system->name, "ps2")) {
        deviceString = "38";
    } else if (!strcmp(system->name, "psp")) {
        deviceString = "41";
    } else if (!strcmp(system->name, "psx")) {
        deviceString = "22";
    } else if (!strcmp(system->name, "saturn")) {
        deviceString = "23";
    } else if (!strcmp(system->name, "segacd")) {
        deviceString = "27";
    } else if (!strcmp(system->name, "snes")) {
        deviceString = "1";
    } else if (!strcmp(system->name, "vectrex")) {
        deviceString = "25";
    } else if (!strcmp(system->name, "wonderswan")) {
        deviceString = "24";
    } else if (!strcmp(system->name, "wonderswancolor")) {
        deviceString = "24";
    }

    return deviceString;
}