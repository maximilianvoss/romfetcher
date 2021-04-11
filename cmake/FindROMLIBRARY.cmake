include(FindPackageHandleStandardArgs)

find_path(ROMLIBRARY_INCLUDE_DIR_INTERNAL romlibrary.h
        HINTS
        include
        )
mark_as_advanced(ROMLIBRARY_INCLUDE_DIR_INTERNAL)

FIND_LIBRARY(ROMLIBRARY_LIBRARIES_INTERNAL
        NAMES romlibrary
        HINTS
        PATH_SUFFIXES lib64 lib
        )
mark_as_advanced(ROMLIBRARY_INCLUDE_DIR_INTERNAL)

if (ROMLIBRARY_INCLUDE_DIR_INTERNAL)
    file(STRINGS ${ROMLIBRARY_INCLUDE_DIR_INTERNAL}/romlibrary.h _ver_line
            REGEX "^#define RL_VERSION *[0-9]+"
            LIMIT_COUNT 1)
    string(REGEX MATCH "[0-9]+"
            ROMLIBRARY_VERSION "${_ver_line}")
    unset(_ver_line)

    if (${ROMLIBRARY_VERSION} GREATER_EQUAL 110 AND ${ROMLIBRARY_VERSION} LESS_EQUAL 200)
        message("Romlibrary Version found: " ${ROMLIBRARY_VERSION})
    else ()
        message("Not supported Romlibrary version found: " ${ROMLIBRARY_VERSION})
        set(ROMLIBRARY_INCLUDE_DIR_INTERNAL "")
        set(ROMLIBRARY_INCLUDE_DIR_INTERNAL "")
    endif ()
endif ()

find_package_handle_standard_args(ROMLIBRARY
        REQUIRED_VARS ROMLIBRARY_INCLUDE_DIR_INTERNAL ROMLIBRARY_LIBRARIES_INTERNAL)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ACLL REQUIRED)
find_package(CHTTP REQUIRED)
find_package(CLOGGER REQUIRED)
find_package(CSAFESTRING REQUIRED)
find_package(LEXBOR REQUIRED)

set(ROMLIBRARY_INCLUDE_DIR
        ${ROMLIBRARY_INCLUDE_DIR_INTERNAL}
        ${CURL_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
        ${ACLL_INCLUDE_DIR}
        ${CHTTP_INCLUDE_DIR}
        ${CLOGGER_INCLUDE_DIR}
        ${CSAFESTRING_INCLUDE_DIR}
        ${LEXBOR_INCLUDE_DIR}
        )
set(ROMLIBRARY_LIBRARIES
        ${ROMLIBRARY_LIBRARIES_INTERNAL}
        ${OPENSSL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${ACLL_LIBRARIES}
        ${CHTTP_LIBRARIES}
        ${CSAFESTRING_LIBRARIES}
        ${LEXBOR_LIBRARIES}
        ${CURL_LIBRARIES}
        )