cmake_minimum_required(VERSION 3.0)
project(romfetcher C)

# Versioning
set(CPACK_PACKAGE_VERSION_MAJOR "3")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
set(CMAKE_C_STANDARD 99)
set(CMAKE_INSTALL_PREFIX /usr)
set(THREADS_PREFER_PTHREAD_FLAG on)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)

# required packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

# Header includes
set(INCLUDE_DIRECTORIES
        ${INCLUDE_DIRECTORIES}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2TTF_INCLUDE_DIRS}
        ${SQLite3_INCLUDE_DIRS}
        libs/romlibrary
        libs/acll
        libs/csafestring
        libs/json2map
        libs/chttp
        libs/clogger
        )
include_directories(${INCLUDE_DIRECTORIES})

# debug handling for build
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_VERBOSE_MAKEFILE on)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -DCLOGGER_LEVEL=3")
    set(CMAKE_INSTALL_PREFIX /usr/local)
else ()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -O3 -DCLOGGER_LEVEL=1")
endif ()

# Build submodules
add_subdirectory(libs/romlibrary)
add_subdirectory(libs/acll)
add_subdirectory(libs/csafestring)
add_subdirectory(libs/json2map)
add_subdirectory(libs/chttp)

#include other cmake files for building
include(src/application.cmake)
include(resources/resources.cmake)
include(resources/themes/themes.cmake)

set(LINK_LIBRARIES
        csafestring
        json2map
        m
        romlibrary
        chttp
        ${SDL2_LIBRARIES}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2TTF_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
        ${SQLite3_LIBRARIES}
        )

# Linking
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