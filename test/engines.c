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

#include "engines.h"
#include "../src/engine/enginehandler.h"
#include "../src/engine/romsmania/romsmania.h"
#include "../src/engine/romsmode/romsmode.h"
#include "../src/engine/wowroms/wowroms.h"
#include "../src/engine/romsdownload/romsdownload.h"
#include "../src/engine/romsemulator/romsemulator.h"
#include "../src/engine/romhustler/romhustler.h"
#include "../src/engine/progameroms/progameroms.h"
#include "../src/engine/freeroms/freeroms.h"

static engine_t *createEngine(char *name, char *fullname) {
    engine_t *engine = (engine_t *) calloc(1, sizeof(engine_t));
    engine->prev = NULL;
    engine->next = NULL;
    engine->active = 1;
    engine->name = name;
    engine->fullname = fullname;
    enginehandler_doMapping(engine);
    return engine;
}

void testengines_init() {
    romsmania = createEngine(romsmania_shortname(), "https://www.romsmania.cc");
    romsmode = createEngine(romsmode_shortname(), "https://www.romsmode.com");
    wowroms = createEngine(wowroms_shortname(), "https://www.wowroms.com");
    romsdownload = createEngine(romsdownload_shortname(), "https://www.roms-download.com");
    romsemulator = createEngine(romsemulator_shortname(), "https://www.romsemulator.net");
    romhustler = createEngine(romhustler_shortname(), "https://romhustler.org");
    progameroms = createEngine(progameroms_shortname(), "https://progameroms.com");
    freeroms = createEngine(freeroms_shortname(), "https://freeroms.com");
}