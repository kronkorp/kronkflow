add_library(project_options INTERFACE)

target_compile_options(project_options INTERFACE
    ${PROJECT_WARNINGS}
    $<$<CONFIG:Debug>:${PROJECT_DEBUG_FLAGS}>
    $<$<CONFIG:Release>:${PROJECT_RELEASE_FLAGS}>
)

target_link_libraries(project_options INTERFACE
    stdc++exp
)

target_link_options(project_options INTERFACE
    -rdynamic
)

target_compile_definitions(project_options INTERFACE
    $<$<CONFIG:Debug>:_DEBUG>
)
