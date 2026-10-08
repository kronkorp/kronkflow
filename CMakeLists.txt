cmake_minimum_required(VERSION 3.15)
project(kronkflow LANGUAGES C)

option(BUILD_TESTS "Build the prophecy test suite (requires kronklab)" OFF)

include(cmake/ProjectOptions.cmake)
include(cmake/CompilerOptions.cmake)
include(cmake/Utils.cmake)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
    message(FATAL_ERROR "Les builds dans le dossier source sont interdits. Créez un dossier de build (ex: 'build/').")
endif()

include(FetchContent)

# NOTE: Pinned: kronkflow does not change when their main does. kronkpool
#       guards its "project_options" target itself (ours has the same name), so
#       it needs no patch anymore
FetchContent_Declare(
    kronkutils_c
    GIT_REPOSITORY https://github.com/kronkorp/kronkutils-C
    GIT_TAG        30587669f40347e071fac6dd532e01e47d327a10
)

FetchContent_Declare(
    kronkpool
    GIT_REPOSITORY https://github.com/kronkorp/kronkpool
    GIT_TAG        387735e0fd831224f7730437ca858bb4f1f78220
)

# NOTE: Their own tests are not ours to build (and kronklab, which they use,
#       does not build on Windows)
set(BUILD_TESTING OFF)
set(KRONKPOOL_BUILD_TESTS OFF)
FetchContent_MakeAvailable(kronkutils_c kronkpool)
unset(BUILD_TESTING)
unset(KRONKPOOL_BUILD_TESTS)

file(GLOB_RECURSE kronkflow_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)

add_library(kronkflow-obj OBJECT ${kronkflow_SOURCES})

target_link_libraries(kronkflow-obj PUBLIC kronkutils_static kronkpool)

target_include_directories(kronkflow-obj
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

if(MSVC)
    # NOTE: No /O2 here: CMake gives it in Release, and it cannot go with the /RTC1 of Debug
    target_compile_options(kronkflow-obj PRIVATE /W4)
else()
    target_compile_options(kronkflow-obj PRIVATE
        -Wall
        -Wextra
        -O3
    )
endif()

set_target_properties(kronkflow-obj PROPERTIES
    C_STANDARD 11
    POSITION_INDEPENDENT_CODE ON
)

add_library(kronkflow-static STATIC $<TARGET_OBJECTS:kronkflow-obj>)
target_link_libraries(kronkflow-static PUBLIC kronkflow-obj) 

add_library(kronkflow-shared SHARED $<TARGET_OBJECTS:kronkflow-obj>)
target_link_libraries(kronkflow-shared PUBLIC kronkflow-obj)

target_compile_definitions(kronkflow-shared PRIVATE kronkflow_BUILD_DLL)

# NOTE: On Windows, a DLL only gives what it exports: the API is not marked for
#       it (the objects are the same as the static library's), so it exports all
set_target_properties(kronkflow-shared PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

add_library(kronkflow ALIAS kronkflow-static)

set_target_properties(kronkflow-static PROPERTIES
    OUTPUT_NAME kronkflow
)

set_target_properties(kronkflow-shared PROPERTIES
    OUTPUT_NAME kronkflow
)

if(MSVC)
    # NOTE: The import library of the DLL is already kronkflow.lib
    set_target_properties(kronkflow-static PROPERTIES OUTPUT_NAME kronkflow_static)
endif()

# NOTE: Installation

include(GNUInstallDirs)

install(TARGETS kronkflow-static kronkflow-shared
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/kronkflow
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# NOTE: Tests (opt-in, via -DBUILD_TESTS=ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
