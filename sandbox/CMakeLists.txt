project(eugine-sandbox)

add_executable(sandbox src/sandbox.cpp)

target_link_libraries(sandbox eugine)

add_custom_target(sandbox_res
    COMMAND ${CMAKE_COMMAND} -E create_symlink
    ${CMAKE_CURRENT_SOURCE_DIR}/res/ ${CMAKE_BINARY_DIR}/res
)