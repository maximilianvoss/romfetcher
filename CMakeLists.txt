cmake_minimum_required(VERSION 3.0)
project(romfetcher C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_C_STANDARD 99)
set(CMAKE_INSTALL_PREFIX /usr)
set(THREADS_PREFER_PTHREAD_FLAG on)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

add_subdirectory(libs/csafestring)
include_directories(libs/csafestring)
add_subdirectory(libs/json2map)
include_directories(libs/json2map)
add_subdirectory(libs/lexbor)
include_directories(libs/lexbor)

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE on)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
    set(CMAKE_INSTALL_PREFIX /usr/local)
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")
endif ()

set(SOURCES_COMMON
        src/common/entities.c
        src/common/entities.h
        src/common/utils.c
        src/common/utils.h
        )

set(SOURCES_LIBRARY
        src/library/helper/curlling.c
        src/library/helper/curlling.h
        src/library/helper/domparsing.c
        src/library/helper/domparsing.h
        src/library/helper/linkedlist.c
        src/library/helper/linkedlist.h
        src/library/helper/md5.c
        src/library/helper/md5.h
        src/library/helper/regex.c
        src/library/helper/regex.h
        src/library/hoster/freeroms/freeroms.c
        src/library/hoster/freeroms/freeroms.h
        src/library/hoster/freeroms/mapping.c
        src/library/hoster/freeroms/mapping.h
        src/library/hoster/hosterhandler.c
        src/library/hoster/hosterhandler.h
        src/library/hoster/results.c
        src/library/hoster/results.h
        src/library/hoster/romhustler/mapping.c
        src/library/hoster/romhustler/mapping.h
        src/library/hoster/romhustler/romhustler.c
        src/library/hoster/romhustler/romhustler.h
        src/library/hoster/romsdownload/icon.h
        src/library/hoster/romsdownload/mapping.c
        src/library/hoster/romsdownload/mapping.h
        src/library/hoster/romsdownload/romsdownload.c
        src/library/hoster/romsdownload/romsdownload.h
        src/library/hoster/romsemulator/icon.h
        src/library/hoster/romsemulator/mapping.c
        src/library/hoster/romsemulator/mapping.h
        src/library/hoster/romsemulator/romsemulator.c
        src/library/hoster/romsemulator/romsemulator.h
        src/library/hoster/romsmania/mapping.c
        src/library/hoster/romsmania/mapping.h
        src/library/hoster/romsmania/romsmania.c
        src/library/hoster/romsmania/romsmania.h
        src/library/hoster/urlhandling.c
        src/library/hoster/urlhandling.h
        src/library/hoster/wowroms/mapping.c
        src/library/hoster/wowroms/mapping.h
        src/library/hoster/wowroms/wowroms.c
        src/library/hoster/wowroms/wowroms.h
        src/library/romfetcher.h
        src/library/systems/systems.c
        src/library/systems/systems.h
        )

set(SOURCES
        src/application/application.h
        src/application/constants.h
        src/application/config/configmenu.c
        src/application/database/download.c
        src/application/database/download.h
        src/application/enginecache/enginecache.c
        src/application/enginecache/enginecache.h
        src/application/database/database.c
        src/application/database/database.h
        src/application/database/linkedlist.c
        src/application/database/linkedlist.h
        src/application/database/postprocess.c
        src/application/database/postprocess.h
        src/application/download/downloader.c
        src/application/download/downloader.h
        src/application/download/downloadercurl.c
        src/application/download/downloadercurl.h
        src/application/download/downloaderinternal.c
        src/application/download/downloaderinternal.h
        src/application/download/downloadpipeline.c
        src/application/download/downloadpipeline.h
        src/application/enums.h
        src/application/helper/map.c
        src/application/helper/map.h
        src/application/helper/path.c
        src/application/helper/path.h
        src/application/helper/uihelper.c
        src/application/helper/uihelper.h
        src/application/download/inputdownload.c
        src/application/download/inputdownload.h
        src/application/download/inputdownloadmanager.c
        src/application/download/inputdownloadmanager.h
        src/application/input/inputhandler.c
        src/application/input/inputhandler.h
        src/application/input/inputkeyboard.c
        src/application/input/inputkeyboard.h
        src/application/input/inputlist.c
        src/application/input/inputlist.h
        src/application/modal/inputmodal.c
        src/application/modal/inputmodal.h
        src/application/search/inputsearch.c
        src/application/search/inputsearch.h
        src/application/download/statedownload.c
        src/application/download/statedownload.h
        src/application/download/statedownloadmanager.c
        src/application/download/statedownloadmanager.h
        src/application/state/statehandler.c
        src/application/state/statehandler.h
        src/application/state/statekeyboard.c
        src/application/state/statekeyboard.h
        src/application/search/statesearch.c
        src/application/search/statesearch.h
        src/application/state/statesystem.c
        src/application/state/statesystem.h
        src/application/themes/loading.c
        src/application/themes/loading.h
        src/application/themes/rendering.c
        src/application/themes/rendering.h
        src/application/ui/display.c
        src/application/ui/display.h
        src/application/ui/rendering.c
        src/application/ui/rendering.h
        src/application/download/uidownload.c
        src/application/download/uidownload.h
        src/application/download/uidownloadmanager.c
        src/application/download/uidownloadmanager.h
        src/application/ui/uihandler.c
        src/application/ui/uihandler.h
        src/application/ui/uikeyboard.c
        src/application/ui/uikeyboard.h
        src/application/ui/uilist.c
        src/application/ui/uilist.h
        src/application/modal/uimodal.c
        src/application/modal/uimodal.h
        src/application/search/uisearch.c
        src/application/search/uisearch.h
        src/application/themes/themes.h
        src/application/download/download.h src/application/input/input.h src/application/ui/texture.h src/application/config/configmenu.h
        src/application/config/config.c src/application/config/config.h src/application/config/configstates.c src/application/config/configstates.h)

set(TEST_SOURCES
        test/download.c
        test/download.h
        test/hoster.c
        test/hoster.h
        test/hoster_freeroms.c
        test/hoster_freeroms.h
        test/hoster_romhustler.c
        test/hoster_romhustler.h
        test/linkedlist.c
        test/linkedlist.h
        test/systems.c
        test/systems.h
        test/test.c
        test/test.h
        )

set(THEME_SUPERMARIO
        resources/themes/supermario/FreeSans.ttf
        resources/themes/supermario/checkbox_checked.png
        resources/themes/supermario/checkbox_unchecked.png
        resources/themes/supermario/selector.png
        resources/themes/supermario/settings.png
        resources/themes/supermario/wallpaper.jpg
        resources/themes/supermario/download.png
        resources/themes/supermario/theme.json
        )

set(THEME_PEACH
        resources/themes/peach/FreeSans.ttf
        resources/themes/peach/checkbox_checked.png
        resources/themes/peach/checkbox_unchecked.png
        resources/themes/peach/selector.png
        resources/themes/peach/settings.png
        resources/themes/peach/wallpaper.png
        resources/themes/peach/download.png
        resources/themes/peach/theme.json
        )

set(THEME_BLACK
        resources/themes/black/FreeSans.ttf
        resources/themes/black/checkbox_checked.png
        resources/themes/black/checkbox_unchecked.png
        resources/themes/black/selector.png
        resources/themes/black/settings.png
        resources/themes/black/download.png
        resources/themes/black/theme.json
        )


set(SCRIPT_FILES
        "resources/ROM Fetcher.sh"
        )


set(LINK_LIBRARIES
        library
        common
        csafestring
        json2map
        lexbor_static
        m
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${CURL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${SQLite3_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        )

include_directories(src/library libs/lexbor/source ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
add_library(common ${SOURCES_COMMON})
add_library(library ${SOURCES_LIBRARY})
add_executable(romfetcher src/application/application.c ${SOURCES})
target_link_libraries(romfetcher ${LINK_LIBRARIES})
install(TARGETS romfetcher DESTINATION bin)
install(FILES ${THEME_SUPERMARIO} DESTINATION share/romfetcher/themes/supermario)
install(FILES ${THEME_PEACH} DESTINATION share/romfetcher/themes/peach)
install(FILES ${THEME_BLACK} DESTINATION share/romfetcher/themes/black)
install(FILES ${SCRIPT_FILES} DESTINATION /home/pi/RetroPie/retropiemenu)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ROM Fetcher for Emulationstation")
set(CPACK_PACKAGE_VENDOR "Maximilian Voss (maximlian@voss.rocks)")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "6")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maximilian Voss")
set(CPACK_DEBIAN_PACKAGE_DEPENDS, "libsdl2-2.0-0 (>=2.0.8), libsdl2-ttf-2.0-0 (>=2.0.14), libsdl2-image-2.0-0 (>=2.0.3), libsqlite3-0 (>=3.22.0), openssl (>=1.1.1)")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/maximilianvoss/romfetcher")
include(CPack)

enable_testing()
add_executable(romfetcher_testcases ${SOURCES} ${TEST_SOURCES})
target_link_libraries(romfetcher_testcases ${LINK_LIBRARIES})

add_test(NAME test_linkedList_deleteElement0 COMMAND romfetcher_testcases test_linkedList_deleteElement0)
add_test(NAME test_linkedList_deleteElement1 COMMAND romfetcher_testcases test_linkedList_deleteElement1)
add_test(NAME test_linkedList_deleteElement2 COMMAND romfetcher_testcases test_linkedList_deleteElement2)
add_test(NAME test_linkedList_deleteElement3 COMMAND romfetcher_testcases test_linkedList_deleteElement3)
add_test(NAME test_linkedList_deleteElement4 COMMAND romfetcher_testcases test_linkedList_deleteElement4)
add_test(NAME test_linkedList_sortElements COMMAND romfetcher_testcases test_linkedList_sortElements)
add_test(NAME test_linkedList_findElementFound COMMAND romfetcher_testcases test_linkedList_findElementFound)
add_test(NAME test_linkedList_findElementNotFound COMMAND romfetcher_testcases test_linkedList_findElementNotFound)
add_test(NAME test_linkedList_getElementCountNull COMMAND romfetcher_testcases test_linkedList_getElementCountNull)
add_test(NAME test_linkedList_getElementCountNotNull COMMAND romfetcher_testcases test_linkedList_getElementCountNotNull)
add_test(NAME test_linkedList_cloneListNull COMMAND romfetcher_testcases test_linkedList_cloneListNull)
add_test(NAME test_linkedList_cloneListNotNull COMMAND romfetcher_testcases test_linkedList_cloneListNotNull)
add_test(NAME test_linkedList_pop COMMAND romfetcher_testcases test_linkedList_pop)
add_test(NAME test_linkedList_removeElement0 COMMAND romfetcher_testcases test_linkedList_removeElement0)
add_test(NAME test_linkedList_removeElement1 COMMAND romfetcher_testcases test_linkedList_removeElement1)
add_test(NAME test_linkedList_removeElement2 COMMAND romfetcher_testcases test_linkedList_removeElement2)
add_test(NAME test_linkedList_removeElement3 COMMAND romfetcher_testcases test_linkedList_removeElement3)
add_test(NAME test_linkedList_removeElement4 COMMAND romfetcher_testcases test_linkedList_removeElement4)

add_test(NAME test_engine_freeroms_3do COMMAND romfetcher_testcases test_engine_freeroms_3do)
add_test(NAME test_engine_freeroms_amiga COMMAND romfetcher_testcases test_engine_freeroms_amiga)
add_test(NAME test_engine_freeroms_atari2600 COMMAND romfetcher_testcases test_engine_freeroms_atari2600)
add_test(NAME test_engine_freeroms_atari7800 COMMAND romfetcher_testcases test_engine_freeroms_atari7800)
add_test(NAME test_engine_freeroms_atarijaguar COMMAND romfetcher_testcases test_engine_freeroms_atarijaguar)
add_test(NAME test_engine_freeroms_atarilynx COMMAND romfetcher_testcases test_engine_freeroms_atarilynx)
add_test(NAME test_engine_freeroms_atarist COMMAND romfetcher_testcases test_engine_freeroms_atarist)
add_test(NAME test_engine_freeroms_c64 COMMAND romfetcher_testcases test_engine_freeroms_c64)
add_test(NAME test_engine_freeroms_dreamcast COMMAND romfetcher_testcases test_engine_freeroms_dreamcast)
add_test(NAME test_engine_freeroms_fba COMMAND romfetcher_testcases test_engine_freeroms_fba)
add_test(NAME test_engine_freeroms_fds COMMAND romfetcher_testcases test_engine_freeroms_fds)
add_test(NAME test_engine_freeroms_gamegear COMMAND romfetcher_testcases test_engine_freeroms_gamegear)
add_test(NAME test_engine_freeroms_gb COMMAND romfetcher_testcases test_engine_freeroms_gb)
add_test(NAME test_engine_freeroms_gba COMMAND romfetcher_testcases test_engine_freeroms_gba)
add_test(NAME test_engine_freeroms_gbc COMMAND romfetcher_testcases test_engine_freeroms_gbc)
add_test(NAME test_engine_freeroms_gc COMMAND romfetcher_testcases test_engine_freeroms_gc)
add_test(NAME test_engine_freeroms_genesis COMMAND romfetcher_testcases test_engine_freeroms_genesis)
add_test(NAME test_engine_freeroms_gw COMMAND romfetcher_testcases test_engine_freeroms_gw)
add_test(NAME test_engine_freeroms_love COMMAND romfetcher_testcases test_engine_freeroms_love)
add_test(NAME test_engine_freeroms_mame COMMAND romfetcher_testcases test_engine_freeroms_mame)
add_test(NAME test_engine_freeroms_mastersystem COMMAND romfetcher_testcases test_engine_freeroms_mastersystem)
add_test(NAME test_engine_freeroms_megadrive COMMAND romfetcher_testcases test_engine_freeroms_megadrive)
add_test(NAME test_engine_freeroms_msx COMMAND romfetcher_testcases test_engine_freeroms_msx)
add_test(NAME test_engine_freeroms_n64 COMMAND romfetcher_testcases test_engine_freeroms_n64)
add_test(NAME test_engine_freeroms_nds COMMAND romfetcher_testcases test_engine_freeroms_nds)
add_test(NAME test_engine_freeroms_neogeo COMMAND romfetcher_testcases test_engine_freeroms_neogeo)
add_test(NAME test_engine_freeroms_nes COMMAND romfetcher_testcases test_engine_freeroms_nes)
add_test(NAME test_engine_freeroms_ngp COMMAND romfetcher_testcases test_engine_freeroms_ngp)
add_test(NAME test_engine_freeroms_ngpc COMMAND romfetcher_testcases test_engine_freeroms_ngpc)
add_test(NAME test_engine_freeroms_pc COMMAND romfetcher_testcases test_engine_freeroms_pc)
add_test(NAME test_engine_freeroms_pcengine COMMAND romfetcher_testcases test_engine_freeroms_pcengine)
add_test(NAME test_engine_freeroms_pcfx COMMAND romfetcher_testcases test_engine_freeroms_pcfx)
add_test(NAME test_engine_freeroms_ps2 COMMAND romfetcher_testcases test_engine_freeroms_ps2)
add_test(NAME test_engine_freeroms_psp COMMAND romfetcher_testcases test_engine_freeroms_psp)
add_test(NAME test_engine_freeroms_psx COMMAND romfetcher_testcases test_engine_freeroms_psx)
add_test(NAME test_engine_freeroms_saturn COMMAND romfetcher_testcases test_engine_freeroms_saturn)
add_test(NAME test_engine_freeroms_scummvm COMMAND romfetcher_testcases test_engine_freeroms_scummvm)
add_test(NAME test_engine_freeroms_sega32x COMMAND romfetcher_testcases test_engine_freeroms_sega32x)
add_test(NAME test_engine_freeroms_segacd COMMAND romfetcher_testcases test_engine_freeroms_segacd)
add_test(NAME test_engine_freeroms_sg1000 COMMAND romfetcher_testcases test_engine_freeroms_sg1000)
add_test(NAME test_engine_freeroms_snes COMMAND romfetcher_testcases test_engine_freeroms_snes)
add_test(NAME test_engine_freeroms_vectrex COMMAND romfetcher_testcases test_engine_freeroms_vectrex)
add_test(NAME test_engine_freeroms_videopac COMMAND romfetcher_testcases test_engine_freeroms_videopac)
add_test(NAME test_engine_freeroms_virtualboy COMMAND romfetcher_testcases test_engine_freeroms_virtualboy)
add_test(NAME test_engine_freeroms_wii COMMAND romfetcher_testcases test_engine_freeroms_wii)
add_test(NAME test_engine_freeroms_wiiu COMMAND romfetcher_testcases test_engine_freeroms_wiiu)
add_test(NAME test_engine_freeroms_wonderswan COMMAND romfetcher_testcases test_engine_freeroms_wonderswan)
add_test(NAME test_engine_freeroms_wonderswancolor COMMAND romfetcher_testcases test_engine_freeroms_wonderswancolor)
add_test(NAME test_engine_freeroms_zxspectrum COMMAND romfetcher_testcases test_engine_freeroms_zxspectrum)
add_test(NAME test_engine_freeroms_download COMMAND romfetcher_testcases test_engine_freeroms_download)

add_test(NAME test_engine_romhustler_3do COMMAND romfetcher_testcases test_engine_romhustler_3do)
add_test(NAME test_engine_romhustler_amiga COMMAND romfetcher_testcases test_engine_romhustler_amiga)
add_test(NAME test_engine_romhustler_atari2600 COMMAND romfetcher_testcases test_engine_romhustler_atari2600)
add_test(NAME test_engine_romhustler_atari7800 COMMAND romfetcher_testcases test_engine_romhustler_atari7800)
add_test(NAME test_engine_romhustler_atarijaguar COMMAND romfetcher_testcases test_engine_romhustler_atarijaguar)
add_test(NAME test_engine_romhustler_atarilynx COMMAND romfetcher_testcases test_engine_romhustler_atarilynx)
add_test(NAME test_engine_romhustler_atarist COMMAND romfetcher_testcases test_engine_romhustler_atarist)
add_test(NAME test_engine_romhustler_c64 COMMAND romfetcher_testcases test_engine_romhustler_c64)
add_test(NAME test_engine_romhustler_dreamcast COMMAND romfetcher_testcases test_engine_romhustler_dreamcast)
add_test(NAME test_engine_romhustler_fba COMMAND romfetcher_testcases test_engine_romhustler_fba)
add_test(NAME test_engine_romhustler_fds COMMAND romfetcher_testcases test_engine_romhustler_fds)
add_test(NAME test_engine_romhustler_gamegear COMMAND romfetcher_testcases test_engine_romhustler_gamegear)
add_test(NAME test_engine_romhustler_gb COMMAND romfetcher_testcases test_engine_romhustler_gb)
add_test(NAME test_engine_romhustler_gba COMMAND romfetcher_testcases test_engine_romhustler_gba)
add_test(NAME test_engine_romhustler_gbc COMMAND romfetcher_testcases test_engine_romhustler_gbc)
add_test(NAME test_engine_romhustler_gc COMMAND romfetcher_testcases test_engine_romhustler_gc)
add_test(NAME test_engine_romhustler_genesis COMMAND romfetcher_testcases test_engine_romhustler_genesis)
add_test(NAME test_engine_romhustler_gw COMMAND romfetcher_testcases test_engine_romhustler_gw)
add_test(NAME test_engine_romhustler_love COMMAND romfetcher_testcases test_engine_romhustler_love)
add_test(NAME test_engine_romhustler_mame COMMAND romfetcher_testcases test_engine_romhustler_mame)
add_test(NAME test_engine_romhustler_mastersystem COMMAND romfetcher_testcases test_engine_romhustler_mastersystem)
add_test(NAME test_engine_romhustler_megadrive COMMAND romfetcher_testcases test_engine_romhustler_megadrive)
add_test(NAME test_engine_romhustler_msx COMMAND romfetcher_testcases test_engine_romhustler_msx)
add_test(NAME test_engine_romhustler_n64 COMMAND romfetcher_testcases test_engine_romhustler_n64)
add_test(NAME test_engine_romhustler_nds COMMAND romfetcher_testcases test_engine_romhustler_nds)
add_test(NAME test_engine_romhustler_neogeo COMMAND romfetcher_testcases test_engine_romhustler_neogeo)
add_test(NAME test_engine_romhustler_nes COMMAND romfetcher_testcases test_engine_romhustler_nes)
add_test(NAME test_engine_romhustler_ngp COMMAND romfetcher_testcases test_engine_romhustler_ngp)
add_test(NAME test_engine_romhustler_ngpc COMMAND romfetcher_testcases test_engine_romhustler_ngpc)
add_test(NAME test_engine_romhustler_pc COMMAND romfetcher_testcases test_engine_romhustler_pc)
add_test(NAME test_engine_romhustler_pcengine COMMAND romfetcher_testcases test_engine_romhustler_pcengine)
add_test(NAME test_engine_romhustler_pcfx COMMAND romfetcher_testcases test_engine_romhustler_pcfx)
add_test(NAME test_engine_romhustler_ps2 COMMAND romfetcher_testcases test_engine_romhustler_ps2)
add_test(NAME test_engine_romhustler_psp COMMAND romfetcher_testcases test_engine_romhustler_psp)
add_test(NAME test_engine_romhustler_psx COMMAND romfetcher_testcases test_engine_romhustler_psx)
add_test(NAME test_engine_romhustler_saturn COMMAND romfetcher_testcases test_engine_romhustler_saturn)
add_test(NAME test_engine_romhustler_scummvm COMMAND romfetcher_testcases test_engine_romhustler_scummvm)
add_test(NAME test_engine_romhustler_sega32x COMMAND romfetcher_testcases test_engine_romhustler_sega32x)
add_test(NAME test_engine_romhustler_segacd COMMAND romfetcher_testcases test_engine_romhustler_segacd)
add_test(NAME test_engine_romhustler_sg1000 COMMAND romfetcher_testcases test_engine_romhustler_sg1000)
add_test(NAME test_engine_romhustler_snes COMMAND romfetcher_testcases test_engine_romhustler_snes)
add_test(NAME test_engine_romhustler_vectrex COMMAND romfetcher_testcases test_engine_romhustler_vectrex)
add_test(NAME test_engine_romhustler_videopac COMMAND romfetcher_testcases test_engine_romhustler_videopac)
add_test(NAME test_engine_romhustler_virtualboy COMMAND romfetcher_testcases test_engine_romhustler_virtualboy)
add_test(NAME test_engine_romhustler_wii COMMAND romfetcher_testcases test_engine_romhustler_wii)
add_test(NAME test_engine_romhustler_wiiu COMMAND romfetcher_testcases test_engine_romhustler_wiiu)
add_test(NAME test_engine_romhustler_wonderswan COMMAND romfetcher_testcases test_engine_romhustler_wonderswan)
add_test(NAME test_engine_romhustler_wonderswancolor COMMAND romfetcher_testcases test_engine_romhustler_wonderswancolor)
add_test(NAME test_engine_romhustler_zxspectrum COMMAND romfetcher_testcases test_engine_romhustler_zxspectrum)
add_test(NAME test_engine_romhustler_download COMMAND romfetcher_testcases test_engine_romhustler_download)