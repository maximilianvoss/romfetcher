include(FindPackageHandleStandardArgs)

find_path(CSAFESTRING_INCLUDE_DIR csafestring.h
        HINTS
        include
        )
mark_as_advanced(CSAFESTRING_INCLUDE_DIR)

FIND_LIBRARY(CSAFESTRING_LIBRARIES
        NAMES csafestring
        HINTS
        PATH_SUFFIXES lib64 lib
        )
mark_as_advanced(CSAFESTRING_LIBRARIES)

find_package_handle_standard_args(CSAFESTRING
        REQUIRED_VARS CSAFESTRING_INCLUDE_DIR CSAFESTRING_LIBRARIES)