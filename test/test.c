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

#include "test.h"
#include "engine_freeroms.h"
#include "engine_romhustler.h"
#include "systems.h"
#include "engines.h"
#include "../src/helper/path.h"
#include "../src/database/init.h"

void test_downloadCallback(void *app) {

}

int main(int argc, char **argv) {
    testsystems_init();
    testengines_init();

    app_t app;
    memset(&app, 0, sizeof(app_t));

    path_initRomfetchersHome();
    if (sqlite3_open_v2("file::memory:", &app.database.db,
                        SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) != SQLITE_OK) {
        printf("Could not initialize database in memory: %s\n", sqlite3_errmsg(app.database.db));
        sqlite3_close(app.database.db);
        exit(1);
    }
    database_initTables(app.database.db);

    app.engine.all = freeroms;
    app.engine.enabled = freeroms;
    TESTCALL("test_engine_freeroms_3do", test_engine_freeroms_3do, &app);
    TESTCALL("test_engine_freeroms_amiga", test_engine_freeroms_amiga, &app);
    TESTCALL("test_engine_freeroms_atari2600", test_engine_freeroms_atari2600, &app);
    TESTCALL("test_engine_freeroms_atari7800", test_engine_freeroms_atari7800, &app);
    TESTCALL("test_engine_freeroms_atarijaguar", test_engine_freeroms_atarijaguar, &app);
    TESTCALL("test_engine_freeroms_atarilynx", test_engine_freeroms_atarilynx, &app);
    TESTCALL("test_engine_freeroms_atarist", test_engine_freeroms_atarist, &app);
    TESTCALL("test_engine_freeroms_c64", test_engine_freeroms_c64, &app);
    TESTCALL("test_engine_freeroms_dreamcast", test_engine_freeroms_dreamcast, &app);
    TESTCALL("test_engine_freeroms_fba", test_engine_freeroms_fba, &app);
    TESTCALL("test_engine_freeroms_fds", test_engine_freeroms_fds, &app);
    TESTCALL("test_engine_freeroms_gamegear", test_engine_freeroms_gamegear, &app);
    TESTCALL("test_engine_freeroms_gb", test_engine_freeroms_gb, &app);
    TESTCALL("test_engine_freeroms_gba", test_engine_freeroms_gba, &app);
    TESTCALL("test_engine_freeroms_gbc", test_engine_freeroms_gbc, &app);
    TESTCALL("test_engine_freeroms_gc", test_engine_freeroms_gc, &app);
    TESTCALL("test_engine_freeroms_genesis", test_engine_freeroms_genesis, &app);
    TESTCALL("test_engine_freeroms_gw", test_engine_freeroms_gw, &app);
    TESTCALL("test_engine_freeroms_love", test_engine_freeroms_love, &app);
    TESTCALL("test_engine_freeroms_mame", test_engine_freeroms_mame, &app);
    TESTCALL("test_engine_freeroms_mastersystem", test_engine_freeroms_mastersystem, &app);
    TESTCALL("test_engine_freeroms_megadrive", test_engine_freeroms_megadrive, &app);
    TESTCALL("test_engine_freeroms_msx", test_engine_freeroms_msx, &app);
    TESTCALL("test_engine_freeroms_n64", test_engine_freeroms_n64, &app);
    TESTCALL("test_engine_freeroms_nds", test_engine_freeroms_nds, &app);
    TESTCALL("test_engine_freeroms_neogeo", test_engine_freeroms_neogeo, &app);
    TESTCALL("test_engine_freeroms_nes", test_engine_freeroms_nes, &app);
    TESTCALL("test_engine_freeroms_ngp", test_engine_freeroms_ngp, &app);
    TESTCALL("test_engine_freeroms_ngpc", test_engine_freeroms_ngpc, &app);
    TESTCALL("test_engine_freeroms_pc", test_engine_freeroms_pc, &app);
    TESTCALL("test_engine_freeroms_pcengine", test_engine_freeroms_pcengine, &app);
    TESTCALL("test_engine_freeroms_pcfx", test_engine_freeroms_pcfx, &app);
    TESTCALL("test_engine_freeroms_ps2", test_engine_freeroms_ps2, &app);
    TESTCALL("test_engine_freeroms_psp", test_engine_freeroms_psp, &app);
    TESTCALL("test_engine_freeroms_psx", test_engine_freeroms_psx, &app);
    TESTCALL("test_engine_freeroms_saturn", test_engine_freeroms_saturn, &app);
    TESTCALL("test_engine_freeroms_scummvm", test_engine_freeroms_scummvm, &app);
    TESTCALL("test_engine_freeroms_sega32x", test_engine_freeroms_sega32x, &app);
    TESTCALL("test_engine_freeroms_segacd", test_engine_freeroms_segacd, &app);
    TESTCALL("test_engine_freeroms_sg1000", test_engine_freeroms_sg1000, &app);
    TESTCALL("test_engine_freeroms_snes", test_engine_freeroms_snes, &app);
    TESTCALL("test_engine_freeroms_vectrex", test_engine_freeroms_vectrex, &app);
    TESTCALL("test_engine_freeroms_videopac", test_engine_freeroms_videopac, &app);
    TESTCALL("test_engine_freeroms_virtualboy", test_engine_freeroms_virtualboy, &app);
    TESTCALL("test_engine_freeroms_wii", test_engine_freeroms_wii, &app);
    TESTCALL("test_engine_freeroms_wiiu", test_engine_freeroms_wiiu, &app);
    TESTCALL("test_engine_freeroms_wonderswan", test_engine_freeroms_wonderswan, &app);
    TESTCALL("test_engine_freeroms_wonderswancolor", test_engine_freeroms_wonderswancolor, &app);
    TESTCALL("test_engine_freeroms_zxspectrum", test_engine_freeroms_zxspectrum, &app);
    TESTCALL("test_engine_freeroms_download", test_engine_freeroms_download, &app);

    app.engine.all = romhustler;
    app.engine.enabled = romhustler;
    TESTCALL("test_engine_romhustler_3do", test_engine_romhustler_3do, &app);
    TESTCALL("test_engine_romhustler_amiga", test_engine_romhustler_amiga, &app);
    TESTCALL("test_engine_romhustler_atari2600", test_engine_romhustler_atari2600, &app);
    TESTCALL("test_engine_romhustler_atari7800", test_engine_romhustler_atari7800, &app);
    TESTCALL("test_engine_romhustler_atarijaguar", test_engine_romhustler_atarijaguar, &app);
    TESTCALL("test_engine_romhustler_atarilynx", test_engine_romhustler_atarilynx, &app);
    TESTCALL("test_engine_romhustler_atarist", test_engine_romhustler_atarist, &app);
    TESTCALL("test_engine_romhustler_c64", test_engine_romhustler_c64, &app);
    TESTCALL("test_engine_romhustler_dreamcast", test_engine_romhustler_dreamcast, &app);
    TESTCALL("test_engine_romhustler_fba", test_engine_romhustler_fba, &app);
    TESTCALL("test_engine_romhustler_fds", test_engine_romhustler_fds, &app);
    TESTCALL("test_engine_romhustler_gamegear", test_engine_romhustler_gamegear, &app);
    TESTCALL("test_engine_romhustler_gb", test_engine_romhustler_gb, &app);
    TESTCALL("test_engine_romhustler_gba", test_engine_romhustler_gba, &app);
    TESTCALL("test_engine_romhustler_gbc", test_engine_romhustler_gbc, &app);
    TESTCALL("test_engine_romhustler_gc", test_engine_romhustler_gc, &app);
    TESTCALL("test_engine_romhustler_genesis", test_engine_romhustler_genesis, &app);
    TESTCALL("test_engine_romhustler_gw", test_engine_romhustler_gw, &app);
    TESTCALL("test_engine_romhustler_love", test_engine_romhustler_love, &app);
    TESTCALL("test_engine_romhustler_mame", test_engine_romhustler_mame, &app);
    TESTCALL("test_engine_romhustler_mastersystem", test_engine_romhustler_mastersystem, &app);
    TESTCALL("test_engine_romhustler_megadrive", test_engine_romhustler_megadrive, &app);
    TESTCALL("test_engine_romhustler_msx", test_engine_romhustler_msx, &app);
    TESTCALL("test_engine_romhustler_n64", test_engine_romhustler_n64, &app);
    TESTCALL("test_engine_romhustler_nds", test_engine_romhustler_nds, &app);
    TESTCALL("test_engine_romhustler_neogeo", test_engine_romhustler_neogeo, &app);
    TESTCALL("test_engine_romhustler_nes", test_engine_romhustler_nes, &app);
    TESTCALL("test_engine_romhustler_ngp", test_engine_romhustler_ngp, &app);
    TESTCALL("test_engine_romhustler_ngpc", test_engine_romhustler_ngpc, &app);
    TESTCALL("test_engine_romhustler_pc", test_engine_romhustler_pc, &app);
    TESTCALL("test_engine_romhustler_pcengine", test_engine_romhustler_pcengine, &app);
    TESTCALL("test_engine_romhustler_pcfx", test_engine_romhustler_pcfx, &app);
    TESTCALL("test_engine_romhustler_ps2", test_engine_romhustler_ps2, &app);
    TESTCALL("test_engine_romhustler_psp", test_engine_romhustler_psp, &app);
    TESTCALL("test_engine_romhustler_psx", test_engine_romhustler_psx, &app);
    TESTCALL("test_engine_romhustler_saturn", test_engine_romhustler_saturn, &app);
    TESTCALL("test_engine_romhustler_scummvm", test_engine_romhustler_scummvm, &app);
    TESTCALL("test_engine_romhustler_sega32x", test_engine_romhustler_sega32x, &app);
    TESTCALL("test_engine_romhustler_segacd", test_engine_romhustler_segacd, &app);
    TESTCALL("test_engine_romhustler_sg1000", test_engine_romhustler_sg1000, &app);
    TESTCALL("test_engine_romhustler_snes", test_engine_romhustler_snes, &app);
    TESTCALL("test_engine_romhustler_vectrex", test_engine_romhustler_vectrex, &app);
    TESTCALL("test_engine_romhustler_videopac", test_engine_romhustler_videopac, &app);
    TESTCALL("test_engine_romhustler_virtualboy", test_engine_romhustler_virtualboy, &app);
    TESTCALL("test_engine_romhustler_wii", test_engine_romhustler_wii, &app);
    TESTCALL("test_engine_romhustler_wiiu", test_engine_romhustler_wiiu, &app);
    TESTCALL("test_engine_romhustler_wonderswan", test_engine_romhustler_wonderswan, &app);
    TESTCALL("test_engine_romhustler_wonderswancolor", test_engine_romhustler_wonderswancolor, &app);
    TESTCALL("test_engine_romhustler_zxspectrum", test_engine_romhustler_zxspectrum, &app);
    TESTCALL("test_engine_romhustler_download", test_engine_romhustler_download, &app);

    database_destroy(&app);
    return -1;
}
