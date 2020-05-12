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
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -DDEVELOP")
    set(CMAKE_INSTALL_PREFIX /usr/local)
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3")
endif ()

set(SOURCES
        src/main.c
        src/config.h
        src/database/config.c
        src/database/config.h
        src/database/init.c
        src/database/init.h
        src/database/sytems.c
        src/database/sytems.h
        src/engine/curlling.c
        src/engine/curlling.h
        src/engine/enginehandler.c
        src/engine/enginehandler.h
        src/engine/results.c
        src/engine/results.h
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
        src/structs.h
        src/themes/loading.c
        src/themes/loading.h
        src/themes/rendering.c
        src/themes/rendering.h
        src/ui/core.c
        src/ui/core.h
        src/ui/fonts.c
        src/ui/fonts.h
        src/ui/inputconfig.c
        src/ui/inputconfig.h
        src/ui/inputlist.c
        src/ui/inputlist.h
        src/ui/inputdownload.c
        src/ui/inputdownload.h
        src/ui/inputhandler.c
        src/ui/inputhandler.h
        src/ui/inputkeyboard.c
        src/ui/inputkeyboard.h
        src/ui/inputsearch.c
        src/ui/inputsearch.h
        src/ui/rendering.c
        src/ui/rendering.h
        src/ui/statehandler.c
        src/ui/statehandler.h
        src/ui/textures.c
        src/ui/textures.h
        src/ui/uiconfig.c
        src/ui/uiconfig.h
        src/ui/uilist.c
        src/ui/uilist.h
        src/ui/uidownload.c
        src/ui/uidownload.h
        src/ui/uihandler.c
        src/ui/uihandler.h
        src/ui/uikeyboard.c
        src/ui/uikeyboard.h
        src/ui/uisearch.c
        src/ui/uisearch.h
        )

set(DEFAULT_THEME
        resources/themes/default/FreeSans.ttf
        resources/themes/default/checkbox_checked.png
        resources/themes/default/checkbox_unchecked.png
        resources/themes/default/selector.png
        resources/themes/default/settings.png
        resources/themes/default/wallpaper.jpg
        resources/themes/default/theme.json
        )

set(SCRIPT_FILES
        "resources/ROM Fetcher.sh"
        )

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
add_executable(romfetcher ${SOURCES})
target_link_libraries(romfetcher csafestring json2map ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${CURL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SQLite3_LIBRARIES} ${OPENSSL_LIBRARIES})
install(TARGETS romfetcher DESTINATION bin)
install(FILES ${DEFAULT_THEME} DESTINATION share/romfetcher/themes/default)
install(FILES ${SCRIPT_FILES} DESTINATION /home/pi/RetroPie/retropiemenu)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ROM Fetcher for Emulationstation")
set(CPACK_PACKAGE_VENDOR "Maximilian Voss (maximlian@voss.rocks)")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "2")
set(CPACK_PACKAGE_VERSION_PATCH "0")


set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maximilian Voss")
set(CPACK_DEBIAN_PACKAGE_DEPENDS, "libsdl2-2.0-0 (>=2.0.8), libsdl2-ttf-2.0-0 (>=2.0.14), libsdl2-image-2.0-0 (>=2.0.3), libsqlite3-0 (>=3.22.0), openssl (>=1.1.1)")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/maximilianvoss/romfetcher")
include(CPack)