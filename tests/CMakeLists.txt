file(GLOB_RECURSE prophecy_TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

add_executable(prophecy_tests ${prophecy_TEST_SOURCES})

target_link_libraries(prophecy_tests PRIVATE
    kronkflow-static
    kronklab-static
)

target_compile_options(prophecy_tests PRIVATE
    -Wall
    -Wextra
)

target_compile_features(prophecy_tests PRIVATE c_std_11)

add_test(NAME prophecy COMMAND prophecy_tests)
