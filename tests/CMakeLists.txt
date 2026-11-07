# NOTE: Included from the root CMakeLists.txt with -DBUILD_TESTS=ON

# NOTE: The smoke test runs on every platform
add_executable(kronkflow_smoke smoke/smoke.c)
target_link_libraries(kronkflow_smoke PRIVATE kronkflow-static)
target_compile_features(kronkflow_smoke PRIVATE c_std_11)
add_test(NAME kronkflow_smoke COMMAND kronkflow_smoke)

# NOTE: kronklab runs each test in a fork(), which Windows does not have
if(WIN32)
    return()
endif()

include(FetchContent)

FetchContent_Declare(
    kronklab
    GIT_REPOSITORY https://github.com/kronkorp/kronklab.git
    GIT_TAG        5584149a3c9ccbb12baaf71cfd591db3b37541ad
)
FetchContent_MakeAvailable(kronklab)

file(GLOB kronkflow_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_executable(kronkflow_tests ${kronkflow_TEST_SOURCES})

target_link_libraries(kronkflow_tests PRIVATE
    kronkflow-static
    kronklab-static
)

target_compile_options(kronkflow_tests PRIVATE
    -Wall
    -Wextra
)

target_compile_features(kronkflow_tests PRIVATE c_std_11)

add_test(NAME kronkflow COMMAND kronkflow_tests)
