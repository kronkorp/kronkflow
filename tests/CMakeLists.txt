file(GLOB_RECURSE kronkflow_TEST_SOURCES
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
