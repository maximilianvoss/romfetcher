/*
 * Copyright 2020 - 2021 Maximilian Voss (maximilian@voss.rocks)
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
#include <romfetcher.h>
#include "test.h"
#include "systems.h"
#include "hoster.h"
#include "download.h"

int test_engine_romhustler_3do() {
    result_t *list = romhustler->search(threeDo, "Scramble Cobra");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Scramble Cobra (1994)(Panasonic)(Eu)[CDD8990]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_amiga() {
    result_t *list = romhustler->search(amiga, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_atari2600() {
    result_t *list = romhustler->search(atari2600, "Sabotage");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sabotage (Ultravison)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atari7800() {
    result_t *list = romhustler->search(atari7800, "NTSC");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("NTSC Encryption Backdoor Test (2000) (PD)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atarijaguar() {
    result_t *list = romhustler->search(atarijaguar, "Ladybug");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Ladybug Demo (PD)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atarilynx() {
    result_t *list = romhustler->search(atarilynx, "Fighter");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Fighter V.2 Demo (2000) (PD)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_atarist() {
    result_t *list = romhustler->search(atarist, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_c64() {
    result_t *list = romhustler->search(c64, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_dreamcast() {
    result_t *list = romhustler->search(dreamcast, "Zombie");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Zombie Revenge USA DC-KALISTO", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_fba() {
    result_t *list = romhustler->search(fba, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_fds() {
    result_t *list = romhustler->search(fds, "Contra");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Contra", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gamegear() {
    result_t *list = romhustler->search(gamegear, "Panzer");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Panzer Dragoon Mini (J) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gb() {
    result_t *list = romhustler->search(gb, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario (V1.0) (JU) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gba() {
    result_t *list = romhustler->search(gba, "Street");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Midnight Club - Street Racing (Europe) (En,Fr,De,Es,It)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gbc() {
    result_t *list = romhustler->search(gbc, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario (V1.0) (JU) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gc() {
    result_t *list = romhustler->search(gc, "Street");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("FIFA Street (U)(OneUp)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_genesis() {
    result_t *list = romhustler->search(genesis, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic 3D Blast", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_gw() {
    result_t *list = romhustler->search(gw, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_love() {
    result_t *list = romhustler->search(love, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_mame() {
    result_t *list = romhustler->search(mame, "road");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Battle K-Road", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_mastersystem() {
    result_t *list = romhustler->search(mastersystem, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic Blast (UE) [!]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_megadrive() {
    result_t *list = romhustler->search(megadrive, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic 3D Blast", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_msx() {
    result_t *list = romhustler->search(msx, "Gradius");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Gradius (1986) (Konami) (J)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_n64() {
    result_t *list = romhustler->search(n64, "Mortal Kombat");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mortal Kombat 4", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_nds() {
    result_t *list = romhustler->search(nds, "Pokemon");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Pokemon - HeartGold (JP)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_neogeo() {
    result_t *list = romhustler->search(neogeo, "Metal Slug");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Metal Slug", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_nes() {
    result_t *list = romhustler->search(nes, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_ngp() {
    result_t *list = romhustler->search(ngp, "Metal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Metal Slug - 1st Mission (JUE)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_ngpc() {
    result_t *list = romhustler->search(ngpc, "Metal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Metal Slug - 1st Mission (JUE)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_pc() {
    result_t *list = romhustler->search(pc, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_pcengine() {
    result_t *list = romhustler->search(pcengine, "image");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Image Fight 2 - Operation Deepstriker (NTSC-J) [ICCD2002]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_pcfx() {
    result_t *list = romhustler->search(pcfx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_ps2() {
    result_t *list = romhustler->search(ps2, "Grandia");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Grandia II (USA)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_psp() {
    result_t *list = romhustler->search(psp, "mortal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mortal Kombat - Unchained (E)(M5)(pSyPSP)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_psx() {
    result_t *list = romhustler->search(psx, "mortal");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Mortal Kombat - Special Forces [U] [SLUS-00824]", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_saturn() {
    result_t *list = romhustler->search(saturn, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic 3D Blast (U)(Saturn)", list->title);
    result_freeList(list);
    return 0;;
}

int test_engine_romhustler_scummvm() {
    result_t *list = romhustler->search(scummvm, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_sega32x() {
    result_t *list = romhustler->search(sega32x, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_segacd() {
    result_t *list = romhustler->search(segacd, "Sonic");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Sonic CD (U)", list->title);
    result_freeList(list);
    return 0;;
}

int test_engine_romhustler_sg1000() {
    result_t *list = romhustler->search(sg1000, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_snes() {
    result_t *list = romhustler->search(snes, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_vectrex() {
    result_t *list = romhustler->search(vectrex, "Mine");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("3-D Mine Storm (1983)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_videopac() {
    result_t *list = romhustler->search(videopac, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_virtualboy() {
    result_t *list = romhustler->search(virtualboy, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_wii() {
    result_t *list = romhustler->search(wii, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_wiiu() {
    result_t *list = romhustler->search(wiiu, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_wonderswan() {
    result_t *list = romhustler->search(wonderswan, "hunter");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Hunter X Hunter - Greed Island (J)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_wonderswancolor() {
    result_t *list = romhustler->search(wonderswancolor, "hunter");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Hunter X Hunter - Greed Island (J)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_romhustler_zxspectrum() {
    result_t *list = romhustler->search(zxspectrum, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_romhustler_download() {
    result_t *list = romhustler->search(nes, "Mario");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Dr. Mario", list->title);


    test_downloaddata_t *testdata = calloc(1, sizeof(test_downloaddata_t));
    testdata->method = GET;
    testdata->filename = "Dr. Mario.zip";
    testdata->title = "Dr. Mario";
    testdata->system = nes;
    testdata->data = NULL;
    testdata->url = "https://romhustler.org/rom/nes/dr-mario-1";

    freeroms->download(list, testDownloadCallback, testdata);

    free(testdata);

    result_freeList(list);
    return 0;
}