
target_sources(eugine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/eugine.cpp)

target_include_directories(eugine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_precompile_headers(eugine PUBLIC egpch.h)

add_subdirectory(eugine)