cmake_minimum_required(VERSION 3.0)
project(romfetcher C)

# Versioning
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_C_STANDARD 99)
set(THREADS_PREFER_PTHREAD_FLAG on)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

# required packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_TTF REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(JSON2MAP REQUIRED)
find_package(ROMLIBRARY REQUIRED)

# debug handling for build
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE on)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -DCLOGGER_LEVEL=2")
    set(CMAKE_INSTALL_PREFIX /usr/local)
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -DCLOGGER_LEVEL=1")
endif ()

if (RL_DYNAMIC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DRL_DYNAMIC")
    if (APPLE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DMACOS")
    elseif (UNIX)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUNIX")
    endif ()
endif ()

# Header includes
set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIRS}
        ${SQLite3_INCLUDE_DIRS}
        ${JSON2MAP_INCLUDE_DIR}
        ${ROMLIBRARY_INCLUDE_DIR}
        )

include_directories(${INCLUDE_DIRECTORIES})

#include other cmake files for building
include(resources/resources.cmake)
include(resources/themes/themes.cmake)

set(LINK_LIBRARIES
        m
        ${CMAKE_THREAD_LIBS_INIT}
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${SQLite3_LIBRARIES}
        ${JSON2MAP_LIBRARIES}
        ${ACLL_LIBRARIES}
        ${CHTTP_LIBRARIES}
        )

if (NOT RL_DYNAMIC)
    set(LINK_LIBRARIES
            ${LINK_LIBRARIES}
            ${ROMLIBRARY_LIBRARIES}
            )
endif ()

set(SOURCES_APPLICATION
        src/application.c
        src/application.h
        src/config/config.c
        src/config/config.h
        src/config/configmenu.c
        src/config/configmenu.h
        src/config/configstates.c
        src/config/configstates.h
        src/constants.h
        src/database/database.c
        src/database/database.h
        src/definitions/config.h
        src/definitions/controller.h
        src/definitions/download.h
        src/definitions/enums.h
        src/definitions/listitem.h
        src/definitions/texture.h
        src/definitions/themes.h
        src/download/download.c
        src/download/download.h
        src/download/downloader.c
        src/download/downloader.h
        src/download/downloadmanager.c
        src/download/downloadmanager.h
        src/download/postprocess.c
        src/download/postprocess.h
        src/enginecache/enginecache.c
        src/enginecache/enginecache.h
        src/helper/entities.c
        src/helper/entities.h
        src/helper/map.c
        src/helper/map.h
        src/helper/path.c
        src/helper/path.h
        src/helper/uihelper.c
        src/helper/uihelper.h
        src/helper/utils.c
        src/helper/utils.h
        src/input/input.c
        src/input/input.h
        src/input/keyboard.c
        src/input/keyboard.h
        src/list/list.c
        src/list/list.h
        src/loader/dynamic.c
        src/loader/dynamic.h
        src/modal/modal.c
        src/modal/modal.h
        src/search/search.c
        src/search/search.h
        src/state/statehandler.c
        src/state/statehandler.h
        src/state/statesystem.c
        src/state/statesystem.h
        src/themes/loading.c
        src/themes/loading.h
        src/themes/rendering.c
        src/themes/rendering.h
        src/ui/display.c
        src/ui/display.h
        src/ui/rendering.c
        src/ui/rendering.h
        src/ui/uihandler.c
        src/ui/uihandler.h
        )

# Linking
add_executable(romfetcher ${SOURCES_APPLICATION})
target_link_libraries(romfetcher ${LINK_LIBRARIES})

# Install
install(TARGETS romfetcher DESTINATION bin)

# Packaging
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ROM Fetcher for Emulationstation")
set(CPACK_PACKAGE_VENDOR "Maximilian Voss (maximlian@voss.rocks)")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maximilian Voss")
set(CPACK_DEBIAN_PACKAGE_DEPENDS, "libsdl2-2.0-0 (>=2.0.8), libsdl2-ttf-2.0-0 (>=2.0.14), libsdl2-image-2.0-0 (>=2.0.3), libsqlite3-0 (>=3.22.0), openssl (>=1.1.1)")
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://github.com/maximilianvoss/romfetcher")
include(CPack)