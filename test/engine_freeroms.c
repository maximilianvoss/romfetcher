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

#include <csafestring.h>
#include "../src/structs.h"
#include "../src/engine/results.h"
#include "engines.h"
#include "systems.h"
#include "test.h"
#include "../src/helper/path.h"

int test_engine_freeroms_3do(app_t *app) {
    searchresult_t *list = freeroms->search(app, threeDo, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_amiga(app_t *app) {
    searchresult_t *list = freeroms->search(app, amiga, "DAMAGE");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Damage", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atari2600(app_t *app) {
    searchresult_t *list = freeroms->search(app, atari2600, "PENGO");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Pengo", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atari7800(app_t *app) {
    searchresult_t *list = freeroms->search(app, atari7800, "GHOST MANOR");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Ghost Manor", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atarijaguar(app_t *app) {
    searchresult_t *list = freeroms->search(app, atarijaguar, "NBA");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NBA Jam TE (1996)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atarilynx(app_t *app) {
    searchresult_t *list = freeroms->search(app, atarilynx, "Conners Tennis");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Jimmy Conners Tennis (1991)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atarist(app_t *app) {
    searchresult_t *list = freeroms->search(app, atarist, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_c64(app_t *app) {
    searchresult_t *list = freeroms->search(app, c64, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_dreamcast(app_t *app) {
    searchresult_t *list = freeroms->search(app, dreamcast, "Dalmations To The Rescue");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("101 Dalmations To The Rescue", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_fba(app_t *app) {
    searchresult_t *list = freeroms->search(app, fba, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_fds(app_t *app) {
    searchresult_t *list = freeroms->search(app, fds, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_gamegear(app_t *app) {
    searchresult_t *list = freeroms->search(app, gamegear, "Excellent Dizzy");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Excellent Dizzy", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gb(app_t *app) {
    searchresult_t *list = freeroms->search(app, gb, "Oide Rascal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Oide Rascal (Japan)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gba(app_t *app) {
    searchresult_t *list = freeroms->search(app, gba, "Helsing");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Van Helsing", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gbc(app_t *app) {
    searchresult_t *list = freeroms->search(app, gbc, "Olympic Summer");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Olympic Summer Games", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gc(app_t *app) {
    searchresult_t *list = freeroms->search(app, gc, "Sorcerers Stone");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Harry Potter and the Sorcerers Stone", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_genesis(app_t *app) {
    searchresult_t *list = freeroms->search(app, genesis, "NBA Jam");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NBA Jam Tournament Edition (32X)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gw(app_t *app) {
    searchresult_t *list = freeroms->search(app, gw, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_love(app_t *app) {
    searchresult_t *list = freeroms->search(app, love, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_mame(app_t *app) {
    searchresult_t *list = freeroms->search(app, mame, "Quadro Quiz");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Quadro Quiz II", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_mastersystem(app_t *app) {
    searchresult_t *list = freeroms->search(app, mastersystem, "Out Run 3D");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Out Run 3D", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_megadrive(app_t *app) {
    searchresult_t *list = freeroms->search(app, megadrive, "NBA Jam");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NBA Jam Tournament Edition (32X)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_msx(app_t *app) {
    searchresult_t *list = freeroms->search(app, msx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_n64(app_t *app) {
    searchresult_t *list = freeroms->search(app, n64, "Mario Kart");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mario Kart 64", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_nds(app_t *app) {
    searchresult_t *list = freeroms->search(app, nds, "Mario and Luigi Partners in Time Europe");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mario and Luigi Partners in Time Europe", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_neogeo(app_t *app) {
    searchresult_t *list = freeroms->search(app, neogeo, "Infinity Cure");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Infinity Cure (J) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_nes(app_t *app) {
    searchresult_t *list = freeroms->search(app, nes, "Z-Gunda");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Z-Gundam", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_ngp(app_t *app) {
    searchresult_t *list = freeroms->search(app, ngp, "Wrestling Madness");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Wrestling Madness (Beta)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_ngpc(app_t *app) {
    searchresult_t *list = freeroms->search(app, ngpc, "Magical Drop Pocket Hack");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Magical Drop Pocket Hack", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_pc(app_t *app) {
    searchresult_t *list = freeroms->search(app, pc, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_pcengine(app_t *app) {
    searchresult_t *list = freeroms->search(app, pcengine, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_pcfx(app_t *app) {
    searchresult_t *list = freeroms->search(app, pcfx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_ps2(app_t *app) {
    searchresult_t *list = freeroms->search(app, ps2, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_psp(app_t *app) {
    searchresult_t *list = freeroms->search(app, psp, "Qix");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Qix++ Japan", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_psx(app_t *app) {
    searchresult_t *list = freeroms->search(app, psx, "Panzer Front");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Panzer Front", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_saturn(app_t *app) {
    searchresult_t *list = freeroms->search(app, saturn, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_scummvm(app_t *app) {
    searchresult_t *list = freeroms->search(app, scummvm, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_sega32x(app_t *app) {
    searchresult_t *list = freeroms->search(app, sega32x, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_segacd(app_t *app) {
    searchresult_t *list = freeroms->search(app, segacd, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_sg1000(app_t *app) {
    searchresult_t *list = freeroms->search(app, sg1000, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_snes(app_t *app) {
    searchresult_t *list = freeroms->search(app, snes, "Q-bert");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Q-bert 3", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_vectrex(app_t *app) {
    searchresult_t *list = freeroms->search(app, vectrex, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_videopac(app_t *app) {
    searchresult_t *list = freeroms->search(app, videopac, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_virtualboy(app_t *app) {
    searchresult_t *list = freeroms->search(app, virtualboy, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_wii(app_t *app) {
    searchresult_t *list = freeroms->search(app, wii, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_wiiu(app_t *app) {
    searchresult_t *list = freeroms->search(app, wiiu, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_wonderswan(app_t *app) {
    searchresult_t *list = freeroms->search(app, wonderswan, "Tetsuman");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Tetsuman (J) [M][!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_wonderswancolor(app_t *app) {
    searchresult_t *list = freeroms->search(app, wonderswancolor, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_zxspectrum(app_t *app) {
    searchresult_t *list = freeroms->search(app, zxspectrum, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_download(app_t *app) {
    searchresult_t *list = freeroms->search(app, amiga, "DAMAGE");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Damage", list->title);

    csafestring_t *downloadPath = path_downloadTarget(amiga, "Damage.zip");
    if (file_exists(downloadPath->data)) {
        remove(downloadPath->data);
    }
    freeroms->download(app, list, &test_downloadCallback);
    ASSERTFILEEXISTS(downloadPath->data);
    safe_destroy(downloadPath);

    result_freeList(list);
    return 0;
}

