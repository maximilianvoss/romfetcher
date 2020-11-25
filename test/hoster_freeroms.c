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
#include <romfetcher.h>
#include "test.h"
#include "systems.h"
#include "hoster.h"
#include "download.h"

int test_engine_freeroms_3do() {
    result_t *list = freeroms->search(threeDo, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_amiga() {
    result_t *list = freeroms->search(amiga, "DAMAGE");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Damage", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atari2600() {
    result_t *list = freeroms->search(atari2600, "PENGO");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Pengo", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atari7800() {
    result_t *list = freeroms->search(atari7800, "GHOST MANOR");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Ghost Manor", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atarijaguar() {
    result_t *list = freeroms->search(atarijaguar, "NBA");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NBA Jam TE (1996)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atarilynx() {
    result_t *list = freeroms->search(atarilynx, "Conners Tennis");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Jimmy Conners Tennis (1991)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_atarist() {
    result_t *list = freeroms->search(atarist, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_c64() {
    result_t *list = freeroms->search(c64, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_dreamcast() {
    result_t *list = freeroms->search(dreamcast, "Dalmations To The Rescue");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("101 Dalmations To The Rescue", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_fba() {
    result_t *list = freeroms->search(fba, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_fds() {
    result_t *list = freeroms->search(fds, "Z-Gunda");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Z-Gundam", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gamegear() {
    result_t *list = freeroms->search(gamegear, "Excellent Dizzy");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Excellent Dizzy", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gb() {
    result_t *list = freeroms->search(gb, "Oide Rascal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Oide Rascal (Japan)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gba() {
    result_t *list = freeroms->search(gba, "Helsing");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Van Helsing", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gbc() {
    result_t *list = freeroms->search(gbc, "Olympic Summer");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Olympic Summer Games", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gc() {
    result_t *list = freeroms->search(gc, "Sorcerers Stone");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Harry Potter and the Sorcerers Stone", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_genesis() {
    result_t *list = freeroms->search(genesis, "NBA Jam");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NBA Jam Tournament Edition (32X)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_gw() {
    result_t *list = freeroms->search(gw, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_love() {
    result_t *list = freeroms->search(love, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_mame() {
    result_t *list = freeroms->search(mame, "Quadro Quiz");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Quadro Quiz II", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_mastersystem() {
    result_t *list = freeroms->search(mastersystem, "Out Run 3D");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Out Run 3D", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_megadrive() {
    result_t *list = freeroms->search(megadrive, "NBA Jam");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NBA Jam Tournament Edition (32X)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_msx() {
    result_t *list = freeroms->search(msx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_n64() {
    result_t *list = freeroms->search(n64, "Mario Kart");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mario Kart 64", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_nds() {
    result_t *list = freeroms->search(nds, "Mario and Luigi Partners in Time Europe");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mario and Luigi Partners in Time Europe", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_neogeo() {
    result_t *list = freeroms->search(neogeo, "Infinity Cure");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Infinity Cure (J) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_nes() {
    result_t *list = freeroms->search(nes, "Z-Gunda");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Z-Gundam", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_ngp() {
    result_t *list = freeroms->search(ngp, "Wrestling Madness");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Wrestling Madness (Beta)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_ngpc() {
    result_t *list = freeroms->search(ngpc, "Magical Drop Pocket Hack");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Magical Drop Pocket Hack", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_pc() {
    result_t *list = freeroms->search(pc, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_pcengine() {
    result_t *list = freeroms->search(pcengine, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_pcfx() {
    result_t *list = freeroms->search(pcfx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_ps2() {
    result_t *list = freeroms->search(ps2, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_psp() {
    result_t *list = freeroms->search(psp, "Qix");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Qix++ Japan", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_psx() {
    result_t *list = freeroms->search(psx, "Panzer Front");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Panzer Front", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_saturn() {
    result_t *list = freeroms->search(saturn, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_scummvm() {
    result_t *list = freeroms->search(scummvm, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_sega32x() {
    result_t *list = freeroms->search(sega32x, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_segacd() {
    result_t *list = freeroms->search(segacd, "NBA Jam");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NBA Jam Tournament Edition (32X)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_sg1000() {
    result_t *list = freeroms->search(sg1000, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_snes() {
    result_t *list = freeroms->search(snes, "Q-bert");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Q-bert 3", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_vectrex() {
    result_t *list = freeroms->search(vectrex, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_videopac() {
    result_t *list = freeroms->search(videopac, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_virtualboy() {
    result_t *list = freeroms->search(virtualboy, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_wii() {
    result_t *list = freeroms->search(wii, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_wiiu() {
    result_t *list = freeroms->search(wiiu, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_wonderswan() {
    result_t *list = freeroms->search(wonderswan, "Tetsuman");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Tetsuman (J) [M][!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_freeroms_wonderswancolor() {
    result_t *list = freeroms->search(wonderswancolor, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_zxspectrum() {
    result_t *list = freeroms->search(zxspectrum, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_freeroms_download() {
    result_t *list = freeroms->search(amiga, "DAMAGE");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Damage", list->title);


    test_downloaddata_t *testdata = calloc(1, sizeof(test_downloaddata_t));
    testdata->method = GET;
    testdata->filename = "Damage.zip";
    testdata->title = "Damage";
    testdata->system = amiga;
    testdata->data = NULL;
    testdata->url = "https://www.freeroms.com/dl_roms/rom_download_tr.php?system=amiga&game_id=10579";

    freeroms->download(list, testDownloadCallback, testdata);

    free(testdata);

    result_freeList(list);
    return 0;
}

