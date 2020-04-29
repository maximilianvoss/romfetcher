cmake_minimum_required(VERSION 3.0)
project(romfetcher C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_C_STANDARD 99)
set(CMAKE_INSTALL_PREFIX /usr)
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS})
set(CMAKE_VERBOSE_MAKEFILE on)
set(THREADS_PREFER_PTHREAD_FLAG on)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(CURL REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

set(SOURCES
        main.c
        config.h
        database/config.c
        database/config.h
        database/init.c
        database/init.h
        database/sytems.c
        database/sytems.h
        download/utils.c
        download/utils.h
        engine/curlling.c
        engine/curlling.h
        engine/enginehandler.c
        engine/enginehandler.h
        engine/results.c
        engine/results.h
        engine/romsmania/mapping.c
        engine/romsmania/mapping.h
        engine/romsmania/romsmania.c
        engine/romsmania/romsmania.h
        engine/romsmode/mapping.c
        engine/romsmode/mapping.h
        engine/romsmode/romsmode.c
        engine/romsmode/romsmode.h
        engine/romsdownload/mapping.h
        engine/romsdownload/romsdownload.c
        engine/romsdownload/romsdownload.h
        engine/romsdownload/mapping.c
        engine/romsemulator/mapping.c
        engine/romsemulator/mapping.h
        engine/romsemulator/romsemulator.c
        engine/romsemulator/romsemulator.h
        engine/urlhandling.c
        engine/urlhandling.h
        engine/wowroms/mapping.c
        engine/wowroms/mapping.h
        engine/wowroms/wowroms.c
        engine/wowroms/wowroms.h
        helper/entities.c
        helper/entities.h
        helper/md5.c
        helper/md5.h
        helper/regex.c
        helper/regex.h
        helper/utils.c
        helper/utils.h
        structs.h
        ui/core.c
        ui/core.h
        ui/fonts.c
        ui/fonts.h
        ui/inputdownload.c
        ui/inputdownload.h
        ui/inputhandler.c
        ui/inputhandler.h
        ui/inputkeyboard.c
        ui/inputkeyboard.h
        ui/inputsearch.c
        ui/inputsearch.h
        ui/inputsystemselect.c
        ui/inputsystemselect.h
        ui/rendering.c
        ui/rendering.h
        ui/textures.c
        ui/textures.h
        ui/uidownload.c
        ui/uidownload.h
        ui/uihandler.c
        ui/uihandler.h
        ui/uikeyboard.c
        ui/uikeyboard.h
        ui/uisearch.c
        ui/uisearch.h
        ui/uisystem.c
        ui/uisystem.h
        )

set(RESOURCE_FILES
        resources/gfx/FreeSans.ttf
        resources/gfx/selector.png
        resources/gfx/wallpaper.jpg
        )

set(SCRIPT_FILES
        "resources/ROM Fetcher.sh"
        )

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})
add_executable(romfetcher ${SOURCES})
target_link_libraries(romfetcher ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2TTF_LIBRARY} ${CURL_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${SQLite3_LIBRARIES} ${OPENSSL_LIBRARIES})
install(TARGETS romfetcher DESTINATION bin)
install(FILES ${RESOURCE_FILES} DESTINATION share/romfetcher)
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