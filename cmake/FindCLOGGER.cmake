include(FindPackageHandleStandardArgs)

find_path(CLOGGER_INCLUDE_DIR clogger.h
        HINTS
        include
        )
mark_as_advanced(CLOGGER_INCLUDE_DIR)

find_package_handle_standard_args(CLOGGER
        REQUIRED_VARS CLOGGER_INCLUDE_DIR)