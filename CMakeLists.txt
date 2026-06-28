cmake_minimum_required(VERSION 3.15)
project(prophecy LANGUAGES C)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

file(GLOB_RECURSE prophecy_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(prophecy-obj OBJECT ${prophecy_SOURCES})

target_include_directories(prophecy-obj
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(prophecy-obj PRIVATE
    -Wall
    -Wextra
    -O3
)

set_target_properties(prophecy-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

add_library(prophecy-static STATIC $<TARGET_OBJECTS:prophecy-obj>)
target_link_libraries(prophecy-static PUBLIC prophecy-obj) 

add_library(prophecy-shared SHARED $<TARGET_OBJECTS:prophecy-obj>)
target_link_libraries(prophecy-shared PUBLIC prophecy-obj)

target_compile_definitions(prophecy-shared PRIVATE prophecy_BUILD_DLL)

add_library(prophecy ALIAS prophecy-static)

set_target_properties(prophecy-static PROPERTIES
    OUTPUT_NAME prophecy
)

set_target_properties(prophecy-shared PROPERTIES
    OUTPUT_NAME prophecy
)

# NOTE: Installation

include(GNUInstallDirs)

install(TARGETS prophecy-static prophecy-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/prophecy
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
