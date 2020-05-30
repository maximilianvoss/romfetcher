/*
 * Copyright 2020 Maximilian Voss (maximilian@voss.rocks)
 *
 * Licensed under the Apache License, Version 2.0 (the "License"){ return 0; }
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
#include "engine_romhustler.h"
#include "engines.h"
#include "test.h"
#include "systems.h"
#include "../src/engine/results.h"
#include "../src/helper/path.h"

int test_engine_romhustler_3do(app_t *app) {
    searchresult_t *list = romhustler->search(app, threeDo, "Scramble Cobra");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Scramble Cobra (1994)(Panasonic)(Eu)[CDD8990]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_amiga(app_t *app) {
    searchresult_t *list = romhustler->search(app, amiga, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_atari2600(app_t *app) {
    searchresult_t *list = romhustler->search(app, atari2600, "Sabotage");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sabotage (Ultravison)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atari7800(app_t *app) {
    searchresult_t *list = romhustler->search(app, atari7800, "NTSC");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NTSC Encryption Backdoor Test (2000) (PD)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atarijaguar(app_t *app) {
    searchresult_t *list = romhustler->search(app, atarijaguar, "Ladybug");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Ladybug Demo (PD)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atarilynx(app_t *app) {
    searchresult_t *list = romhustler->search(app, atarilynx, "Fighter");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Fighter V.2 Demo (2000) (PD)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atarist(app_t *app) {
    searchresult_t *list = romhustler->search(app, atarist, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_c64(app_t *app) {
    searchresult_t *list = romhustler->search(app, c64, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_dreamcast(app_t *app) {
    searchresult_t *list = romhustler->search(app, dreamcast, "Zombie");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Zombie Revenge USA DC-KALISTO", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_fba(app_t *app) {
    searchresult_t *list = romhustler->search(app, fba, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_fds(app_t *app) {
    searchresult_t *list = romhustler->search(app, fds, "Contra");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Contra", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gamegear(app_t *app) {
    searchresult_t *list = romhustler->search(app, gamegear, "Panzer");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Panzer Dragoon Mini (J) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gb(app_t *app) {
    searchresult_t *list = romhustler->search(app, gb, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario (V1.0) (JU) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gba(app_t *app) {
    searchresult_t *list = romhustler->search(app, gba, "Street");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Midnight Club - Street Racing (Europe) (En,Fr,De,Es,It)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gbc(app_t *app) {
    searchresult_t *list = romhustler->search(app, gbc, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario (V1.0) (JU) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gc(app_t *app) {
    searchresult_t *list = romhustler->search(app, gc, "Street");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("FIFA Street (U)(OneUp)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_genesis(app_t *app) {
    searchresult_t *list = romhustler->search(app, genesis, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic 3D Blast", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gw(app_t *app) {
    searchresult_t *list = romhustler->search(app, gw, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_love(app_t *app) {
    searchresult_t *list = romhustler->search(app, love, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_mame(app_t *app) {
    searchresult_t *list = romhustler->search(app, mame, "road");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Battle K-Road", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_mastersystem(app_t *app) {
    searchresult_t *list = romhustler->search(app, mastersystem, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic Blast (UE) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_megadrive(app_t *app) {
    searchresult_t *list = romhustler->search(app, megadrive, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic 3D Blast", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_msx(app_t *app) {
    searchresult_t *list = romhustler->search(app, msx, "Gradius");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Gradius (1986) (Konami) (J)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_n64(app_t *app) {
    searchresult_t *list = romhustler->search(app, n64, "Mortal Kombat");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mortal Kombat 4", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_nds(app_t *app) {
    searchresult_t *list = romhustler->search(app, nds, "Pokemon");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Pokemon - HeartGold (JP)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_neogeo(app_t *app) {
    searchresult_t *list = romhustler->search(app, neogeo, "Metal Slug");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Metal Slug", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_nes(app_t *app) {
    searchresult_t *list = romhustler->search(app, nes, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_ngp(app_t *app) {
    searchresult_t *list = romhustler->search(app, ngp, "Metal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Metal Slug - 1st Mission (JUE)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_ngpc(app_t *app) {
    searchresult_t *list = romhustler->search(app, ngpc, "Metal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Metal Slug - 1st Mission (JUE)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_pc(app_t *app) {
    searchresult_t *list = romhustler->search(app, pc, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_pcengine(app_t *app) {
    searchresult_t *list = romhustler->search(app, pcengine, "image");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Image Fight 2 - Operation Deepstriker (NTSC-J) [ICCD2002]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_pcfx(app_t *app) {
    searchresult_t *list = romhustler->search(app, pcfx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_ps2(app_t *app) {
    searchresult_t *list = romhustler->search(app, ps2, "Grandia");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Grandia II (USA)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_psp(app_t *app) {
    searchresult_t *list = romhustler->search(app, psp, "mortal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mortal Kombat - Unchained (E)(M5)(pSyPSP)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_psx(app_t *app) {
    searchresult_t *list = romhustler->search(app, psx, "mortal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mortal Kombat - Special Forces [U] [SLUS-00824]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_saturn(app_t *app) {
    searchresult_t *list = romhustler->search(app, saturn, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic 3D Blast (U)(Saturn)", list->title);
    result_freeList(list);
    return 0;;
}

int test_engine_romhustler_scummvm(app_t *app) {
    searchresult_t *list = romhustler->search(app, scummvm, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_sega32x(app_t *app) {
    searchresult_t *list = romhustler->search(app, sega32x, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_segacd(app_t *app) {
    searchresult_t *list = romhustler->search(app, segacd, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic CD (U)", list->title);
    result_freeList(list);
    return 0;;
}

int test_engine_romhustler_sg1000(app_t *app) {
    searchresult_t *list = romhustler->search(app, sg1000, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_snes(app_t *app) {
    searchresult_t *list = romhustler->search(app, snes, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_vectrex(app_t *app) {
    searchresult_t *list = romhustler->search(app, vectrex, "Mine");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("3-D Mine Storm (1983)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_videopac(app_t *app) {
    searchresult_t *list = romhustler->search(app, videopac, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_virtualboy(app_t *app) {
    searchresult_t *list = romhustler->search(app, virtualboy, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_wii(app_t *app) {
    searchresult_t *list = romhustler->search(app, wii, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_wiiu(app_t *app) {
    searchresult_t *list = romhustler->search(app, wiiu, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_wonderswan(app_t *app) {
    searchresult_t *list = romhustler->search(app, wonderswan, "hunter");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Hunter X Hunter - Greed Island (J)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_wonderswancolor(app_t *app) {
    searchresult_t *list = romhustler->search(app, wonderswancolor, "hunter");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Hunter X Hunter - Greed Island (J)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_zxspectrum(app_t *app) {
    searchresult_t *list = romhustler->search(app, zxspectrum, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_download(app_t *app) {
    searchresult_t *list = romhustler->search(app, nes, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario", list->title);

    csafestring_t *downloadPath = path_downloadTarget(nes, "Dr. Mario.7z");
    if (file_exists(downloadPath->data)) {
        remove(downloadPath->data);
    }
    romhustler->download(app, list, &test_downloadCallback);
    ASSERTFILEEXISTS(downloadPath->data);
    safe_destroy(downloadPath);

    result_freeList(list);
    return 0;
}