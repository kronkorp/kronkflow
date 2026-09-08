function(apply_project_settings target)
    target_link_libraries(${target} PRIVATE project_options)
endfunction()
