# Cmake script for project AddInNative
# autor: 1C

CMAKE_MINIMUM_REQUIRED(VERSION 2.6 FATAL_ERROR)

PROJECT(innNative)

SET(innNative_SRC 
    innNative.cpp
    innNative.h
    dllmain.cpp
    stdafx.cpp
    stdafx.h
    )

include_directories(${CMAKE_SOURCE_DIR}/../include)

SET (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} ${CMAKE_SOURCE_DIR})
if (UNIX)
    set_source_files_properties(${innNative_SRC}
                              PROPERTIES COMPILE_FLAGS
                              "-fPIC")
else()
    SET(innDef_SRC innNative.def)
    SET(CMAKE_LINK_DEF_FILE_FLAG innNative.def)
    add_definitions(-DUNICODE -DWIN32 )
endif()

add_library(${PROJECT_NAME} SHARED ${innNative_SRC} ${innDef_SRC})

set_target_properties( ${PROJECT_NAME} PROPERTIES
   CLEAN_DIRECT_OUTPUT 1
   #COMPILE_FLAGS "-m32" LINK_FLAGS "-m32"
   # avoid conflicts between library and binary target names
   OUTPUT_NAME ${PROJECT_NAME}${MySuffix1}${MySuffix2} 
   #WINDOWS_EXPORT_ALL_SYMBOLS TRUE
   )

