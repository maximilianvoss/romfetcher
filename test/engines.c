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
#include "../src/engine/romsmania/romsmania.h"
#include "../src/engine/wowroms/wowroms.h"
#include "../src/engine/romsdownload/romsdownload.h"
#include "../src/engine/romsemulator/romsemulator.h"
#include "../src/engine/romhustler/romhustler.h"
#include "../src/engine/freeroms/freeroms.h"

void testengines_init() {
    romsmania = romsmania_getEngine();
    wowroms = wowroms_getEngine();
    romsdownload = romsdownload_getEngine();
    romsemulator = romsemulator_getEngine();
    romhustler = romhustler_getEngine();
    freeroms = freeroms_getEngine();
}