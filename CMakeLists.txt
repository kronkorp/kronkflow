cmake_minimum_required(VERSION 3.15)
project(kronkflow LANGUAGES C)

include(cmake/ProjectOptions.cmake)
include(cmake/CompilerOptions.cmake)
include(cmake/Utils.cmake)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

file(GLOB_RECURSE kronkflow_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(kronkflow-obj OBJECT ${kronkflow_SOURCES})

target_include_directories(kronkflow-obj
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_options(kronkflow-obj PRIVATE
    -Wall
    -Wextra
    -O3
)

set_target_properties(kronkflow-obj PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

add_library(kronkflow-static STATIC $<TARGET_OBJECTS:kronkflow-obj>)
target_link_libraries(kronkflow-static PUBLIC kronkflow-obj) 

add_library(kronkflow-shared SHARED $<TARGET_OBJECTS:kronkflow-obj>)
target_link_libraries(kronkflow-shared PUBLIC kronkflow-obj)

target_compile_definitions(kronkflow-shared PRIVATE kronkflow_BUILD_DLL)

add_library(kronkflow ALIAS kronkflow-static)

set_target_properties(kronkflow-static PROPERTIES
    OUTPUT_NAME kronkflow
)

set_target_properties(kronkflow-shared PROPERTIES
    OUTPUT_NAME kronkflow
)

# NOTE: Installation

include(GNUInstallDirs)

install(TARGETS kronkflow-static kronkflow-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronkflow
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
