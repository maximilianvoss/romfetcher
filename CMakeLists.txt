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

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE on)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
    set(CMAKE_INSTALL_PREFIX /usr/local)
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")
endif ()

set(SOURCES
        src/config.h
        src/config/advanced.c
        src/config/advanced.h
        src/config/init.c
        src/config/init.h
        src/database/config.c
        src/database/config.h
        src/database/enginecache.c
        src/database/enginecache.h
        src/database/init.c
        src/database/init.h
        src/database/linkedlist.c
        src/database/linkedlist.h
        src/database/postprocess.c
        src/database/postprocess.h
        src/engine/downloader.c
        src/engine/downloader.h
        src/engine/freeroms/freeroms.c
        src/engine/freeroms/freeroms.h
        src/engine/freeroms/mapping.c
        src/engine/freeroms/mapping.h
        src/engine/curlling.c
        src/engine/curlling.h
        src/engine/enginehandler.c
        src/engine/enginehandler.h
        src/engine/results.c
        src/engine/results.h
        src/engine/romhustler/mapping.c
        src/engine/romhustler/mapping.h
        src/engine/romhustler/romhustler.c
        src/engine/romhustler/romhustler.h
        src/engine/romsdownload/mapping.c
        src/engine/romsdownload/mapping.h
        src/engine/romsdownload/romsdownload.c
        src/engine/romsdownload/romsdownload.h
        src/engine/romsemulator/mapping.c
        src/engine/romsemulator/mapping.h
        src/engine/romsemulator/romsemulator.c
        src/engine/romsemulator/romsemulator.h
        src/engine/romsmania/mapping.c
        src/engine/romsmania/mapping.h
        src/engine/romsmania/romsmania.c
        src/engine/romsmania/romsmania.h
        src/engine/romsmode/mapping.c
        src/engine/romsmode/mapping.h
        src/engine/romsmode/romsmode.c
        src/engine/romsmode/romsmode.h
        src/engine/urlhandling.c
        src/engine/urlhandling.h
        src/engine/wowroms/mapping.c
        src/engine/wowroms/mapping.h
        src/engine/wowroms/wowroms.c
        src/engine/wowroms/wowroms.h
        src/enums.h
        src/helper/entities.c
        src/helper/entities.h
        src/helper/linkedlist.c
        src/helper/linkedlist.h
        src/helper/map.c
        src/helper/map.h
        src/helper/md5.c
        src/helper/md5.h
        src/helper/path.c
        src/helper/path.h
        src/helper/regex.c
        src/helper/regex.h
        src/helper/utils.c
        src/helper/utils.h
        src/input/inputdownload.c
        src/input/inputdownload.h
        src/input/inputhandler.c
        src/input/inputhandler.h
        src/input/inputkeyboard.c
        src/input/inputkeyboard.h
        src/input/inputlist.c
        src/input/inputlist.h
        src/input/inputmodal.c
        src/input/inputmodal.h
        src/input/inputsearch.c
        src/input/inputsearch.h
        src/state/stateconfig.c
        src/state/stateconfig.h
        src/state/stateconfigadvanced.c
        src/state/stateconfigadvanced.h
        src/state/stateconfigengine.c
        src/state/stateconfigengine.h
        src/state/stateconfigresolution.c
        src/state/stateconfigresolution.h
        src/state/stateconfigsystems.c
        src/state/stateconfigsystems.h
        src/state/stateconfigthemes.c
        src/state/stateconfigthemes.h
        src/state/statedownload.c
        src/state/statedownload.h
        src/state/statehandler.c
        src/state/statehandler.h
        src/state/statekeyboard.c
        src/state/statekeyboard.h
        src/state/statesearch.c
        src/state/statesearch.h
        src/state/statesystem.c
        src/state/statesystem.h
        src/structs.h
        src/systems/systemhandler.c
        src/systems/systemhandler.h
        src/themes/loading.c
        src/themes/loading.h
        src/themes/rendering.c
        src/themes/rendering.h
        src/ui/core.c
        src/ui/core.h
        src/ui/fonts.c
        src/ui/fonts.h
        src/ui/rendering.c
        src/ui/rendering.h
        src/ui/textures.c
        src/ui/textures.h
        src/ui/uidownload.c
        src/ui/uidownload.h
        src/ui/uihandler.c
        src/ui/uihandler.h
        src/ui/uimodal.c
        src/ui/uimodal.h
        src/ui/uikeyboard.c
        src/ui/uikeyboard.h
        src/ui/uilist.c
        src/ui/uilist.h
        src/ui/uisearch.c
        src/ui/uisearch.h
        )

set(TEST_SOURCES
        test/engine_freeroms.c
        test/engine_freeroms.h
        test/engine_romhustler.c
        test/engine_romhustler.h
        test/engines.c
        test/engines.h
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
        resources/themes/supermario/theme.json
        )

set(THEME_PEACH
        resources/themes/peach/FreeSans.ttf
        resources/themes/peach/checkbox_checked.png
        resources/themes/peach/checkbox_unchecked.png
        resources/themes/peach/selector.png
        resources/themes/peach/settings.png
        resources/themes/peach/wallpaper.png
        resources/themes/peach/theme.json
        )

set(THEME_BLACK
        resources/themes/black/FreeSans.ttf
        resources/themes/black/checkbox_checked.png
        resources/themes/black/checkbox_unchecked.png
        resources/themes/black/selector.png
        resources/themes/black/settings.png
        resources/themes/black/theme.json
        )


set(SCRIPT_FILES
        "resources/ROM Fetcher.sh"
        )

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
add_executable(romfetcher src/main.c ${SOURCES})
target_link_libraries(romfetcher csafestring json2map ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${CURL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SQLite3_LIBRARIES} ${OPENSSL_LIBRARIES})
install(TARGETS romfetcher DESTINATION bin)
install(FILES ${THEME_SUPERMARIO} DESTINATION share/romfetcher/themes/supermario)
install(FILES ${THEME_PEACH} DESTINATION share/romfetcher/themes/peach)
install(FILES ${THEME_BLACK} DESTINATION share/romfetcher/themes/black)
install(FILES ${SCRIPT_FILES} DESTINATION /home/pi/RetroPie/retropiemenu)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ROM Fetcher for Emulationstation")
set(CPACK_PACKAGE_VENDOR "Maximilian Voss (maximlian@voss.rocks)")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_VERSION_MAJOR "2")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maximilian Voss")
set(CPACK_DEBIAN_PACKAGE_DEPENDS, "libsdl2-2.0-0 (>=2.0.8), libsdl2-ttf-2.0-0 (>=2.0.14), libsdl2-image-2.0-0 (>=2.0.3), libsqlite3-0 (>=3.22.0), openssl (>=1.1.1)")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/maximilianvoss/romfetcher")
include(CPack)

enable_testing()
add_executable(romfetcher_testcases ${SOURCES} ${TEST_SOURCES})
target_link_libraries(romfetcher_testcases csafestring json2map ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${CURL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SQLite3_LIBRARIES} ${OPENSSL_LIBRARIES})

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
#add_test(NAME test_engine_freeroms_download COMMAND romfetcher_testcases test_engine_freeroms_download)

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
#add_test(NAME test_engine_romhustler_download COMMAND romfetcher_testcases test_engine_romhustler_download)