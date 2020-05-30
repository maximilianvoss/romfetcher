cmake_minimum_required(VERSION 2.8)

project(json2map)

set(SOURCE_FILES
        jsmn.c
        jsmn.h
        json2map.c
        json2map.h
        map2json.c
        map2json.h
        stringlib.c
        stringlib.h
        logging.h
        config.h)

if (NOT TARGET json2map)
    add_library(json2map ${SOURCE_FILES})
    target_link_libraries(json2map csafestring)
endif ()