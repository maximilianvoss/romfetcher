include(FindPackageHandleStandardArgs)

find_path(LEXBOR_INCLUDE_DIR html.h
        HINTS
        include
        PATH_SUFFIXES
        lexbor/html
        )
mark_as_advanced(LEXBOR_INCLUDE_DIR)

FIND_LIBRARY(LEXBOR_LIBRARIES
        NAMES lexbor_static
        HINTS
        lib64 lib
        )
mark_as_advanced(LEXBOR_LIBRARIES)

find_package_handle_standard_args(LEXBOR
        REQUIRED_VARS LEXBOR_INCLUDE_DIR LEXBOR_LIBRARIES)
