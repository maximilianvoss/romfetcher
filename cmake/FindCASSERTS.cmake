include(FindPackageHandleStandardArgs)

find_path(CASSERTS_INCLUDE_DIR casserts.h
        HINTS
        include
        )
mark_as_advanced(CASSERTS_INCLUDE_DIR)

find_package_handle_standard_args(CASSERTS
        REQUIRED_VARS CASSERTS_INCLUDE_DIR)