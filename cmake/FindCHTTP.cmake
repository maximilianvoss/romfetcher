include(FindPackageHandleStandardArgs)

find_path(CHTTP_INCLUDE_DIR_INTERNAL chttp.h
        HINTS
        include
        )
mark_as_advanced(CHTTP_INCLUDE_DIR_INTERNAL)

FIND_LIBRARY(CHTTP_LIBRARIES_INTERNAL
        NAMES chttp
        HINTS
        PATH_SUFFIXES lib64 lib
        )
mark_as_advanced(CHTTP_LIBRARIES_INTERNAL)

find_package_handle_standard_args(CHTTP
        REQUIRED_VARS CHTTP_INCLUDE_DIR_INTERNAL CHTTP_LIBRARIES_INTERNAL)

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(CLOGGER REQUIRED)

set(CHTTP_INCLUDE_DIR
        ${CHTTP_INCLUDE_DIR_INTERNAL}
        ${CURL_INCLUDE_DIRS}
        ${OPENSSL_INCLUDE_DIR}
        ${CLOGGER_INCLUDE_DIR}
        )

set(CHTTP_LIBRARIES
        ${CHTTP_LIBRARIES_INTERNAL}
        ${OPENSSL_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        ${CURL_LIBRARIES}
        )
