project(eugine-sandbox)

add_executable(sandbox src/sandbox.cpp)

target_link_libraries(sandbox eugine)

target_include_directories(sandbox PUBLIC ${CMAKE_SOURCE_DIR}/eugine/src)