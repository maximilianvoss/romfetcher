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
#include "engine_progameroms.h"
#include "test.h"
#include "engines.h"
#include "systems.h"
#include "../src/engine/results.h"
#include "../src/helper/path.h"

int test_engine_progameroms_3do(app_t *app) {
    searchresult_t *list = progameroms->search(app, threeDo, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_amiga(app_t *app) {
    searchresult_t *list = progameroms->search(app, amiga, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_atari2600(app_t *app) {
    searchresult_t *list = progameroms->search(app, atari2600, "KING");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("King Kong", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_atari7800(app_t *app) {
    searchresult_t *list = progameroms->search(app, atari7800, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_atarijaguar(app_t *app) {
    searchresult_t *list = progameroms->search(app, atarijaguar, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_atarilynx(app_t *app) {
    searchresult_t *list = progameroms->search(app, atarilynx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_atarist(app_t *app) {
    searchresult_t *list = progameroms->search(app, atarist, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_c64(app_t *app) {
    searchresult_t *list = progameroms->search(app, c64, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_dreamcast(app_t *app) {
    searchresult_t *list = progameroms->search(app, dreamcast, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_fba(app_t *app) {
    searchresult_t *list = progameroms->search(app, fba, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_fds(app_t *app) {
    searchresult_t *list = progameroms->search(app, fds, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_gamegear(app_t *app) {
    searchresult_t *list = progameroms->search(app, gamegear, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_gb(app_t *app) {
    searchresult_t *list = progameroms->search(app, gb, "Heiankyo Alien");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Heiankyo Alien (USA)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_gba(app_t *app) {
    searchresult_t *list = progameroms->search(app, gba, "Taxi 3");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Taxi 3 (F)", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_gbc(app_t *app) {
    searchresult_t *list = progameroms->search(app, gbc, "Paperb");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Paperboy", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_gc(app_t *app) {
    searchresult_t *list = progameroms->search(app, gc, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_genesis(app_t *app) {
    searchresult_t *list = progameroms->search(app, genesis, "Ranger-");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Ranger-X", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_gw(app_t *app) {
    searchresult_t *list = progameroms->search(app, gw, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_love(app_t *app) {
    searchresult_t *list = progameroms->search(app, love, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_mame(app_t *app) {
    searchresult_t *list = progameroms->search(app, mame, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_mastersystem(app_t *app) {
    searchresult_t *list = progameroms->search(app, mastersystem, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_megadrive(app_t *app) {
    searchresult_t *list = progameroms->search(app, megadrive, "Taz-Man");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Taz-Mania", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_msx(app_t *app) {
    searchresult_t *list = progameroms->search(app, msx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_n64(app_t *app) {
    searchresult_t *list = progameroms->search(app, n64, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_nds(app_t *app) {
    searchresult_t *list = progameroms->search(app, nds, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_neogeo(app_t *app) {
    searchresult_t *list = progameroms->search(app, neogeo, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_nes(app_t *app) {
    searchresult_t *list = progameroms->search(app, nes, "High Speed");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("High Speed", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_ngp(app_t *app) {
    searchresult_t *list = progameroms->search(app, ngp, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_ngpc(app_t *app) {
    searchresult_t *list = progameroms->search(app, ngpc, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_pc(app_t *app) {
    searchresult_t *list = progameroms->search(app, pc, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_pcengine(app_t *app) {
    searchresult_t *list = progameroms->search(app, pcengine, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_pcfx(app_t *app) {
    searchresult_t *list = progameroms->search(app, pcfx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_ps2(app_t *app) {
    searchresult_t *list = progameroms->search(app, ps2, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_psp(app_t *app) {
    searchresult_t *list = progameroms->search(app, psp, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_psx(app_t *app) {
    searchresult_t *list = progameroms->search(app, psx, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_saturn(app_t *app) {
    searchresult_t *list = progameroms->search(app, saturn, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_scummvm(app_t *app) {
    searchresult_t *list = progameroms->search(app, scummvm, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_sega32x(app_t *app) {
    searchresult_t *list = progameroms->search(app, sega32x, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_segacd(app_t *app) {
    searchresult_t *list = progameroms->search(app, segacd, "Out of this Wo");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Out of this World", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_sg1000(app_t *app) {
    searchresult_t *list = progameroms->search(app, sg1000, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_snes(app_t *app) {
    searchresult_t *list = progameroms->search(app, snes, "Shaq");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Shaq Fu (U) ", list->title);
    result_freeList(list);
    return 0;
}

int test_engine_progameroms_vectrex(app_t *app) {
    searchresult_t *list = progameroms->search(app, vectrex, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_videopac(app_t *app) {
    searchresult_t *list = progameroms->search(app, videopac, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_virtualboy(app_t *app) {
    searchresult_t *list = progameroms->search(app, virtualboy, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_wii(app_t *app) {
    searchresult_t *list = progameroms->search(app, wii, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_wiiu(app_t *app) {
    searchresult_t *list = progameroms->search(app, wiiu, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_wonderswan(app_t *app) {
    searchresult_t *list = progameroms->search(app, wonderswan, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_wonderswancolor(app_t *app) {
    searchresult_t *list = progameroms->search(app, wonderswancolor, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_zxspectrum(app_t *app) {
    searchresult_t *list = progameroms->search(app, zxspectrum, "");
    ASSERTNULL(list);
    return 0;
}

int test_engine_progameroms_download(app_t *app) {
    searchresult_t *list = progameroms->search(app, snes, "Shaq");
    ASSERTNOTNULL(list);
    list = linkedlist_sort(list);
    ASSERTSTR("Shaq Fu (U) ", list->title);

    csafestring_t *downloadPath = path_downloadTarget(snes, "Shaq_Fu_(U)_[!].rar");
    if (file_exists(downloadPath->data)) {
        remove(downloadPath->data);
    }
    progameroms->download(app, list, &test_downloadCallback);
    ASSERTFILEEXISTS(downloadPath->data);
    safe_destroy(downloadPath);

    result_freeList(list);
    return 0;
}