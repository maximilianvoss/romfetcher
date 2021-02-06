include(FindPackageHandleStandardArgs)

find_path(ACLL_INCLUDE_DIR_INTERNAL acll.h
        HINTS
        include
        )
mark_as_advanced(ACLL_INCLUDE_DIR_INTERNAL)

FIND_LIBRARY(ACLL_LIBRARIES_INTERNAL
        NAMES acll
        HINTS
        PATH_SUFFIXES lib64 lib
        )
mark_as_advanced(ACLL_LIBRARIES_INTERNAL)

find_package_handle_standard_args(ACLL
        REQUIRED_VARS ACLL_INCLUDE_DIR_INTERNAL ACLL_LIBRARIES_INTERNAL)

find_package(CASSERTS REQUIRED)

set(ACLL_INCLUDE_DIR
        ${ACLL_INCLUDE_DIR_INTERNAL}
        ${CASSERTS_INCLUDE_DIR}
        )
set(ACLL_LIBRARIES
        ${ACLL_LIBRARIES_INTERNAL}
        ${CASSERTS_LIBRARIES}
        )