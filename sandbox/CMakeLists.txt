project(eugine-sandbox)

add_executable(sandbox src/sandbox.cpp)

target_link_libraries(sandbox eugine)

add_custom_command(TARGET sandbox PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/res/ ${CMAKE_BINARY_DIR}/res)