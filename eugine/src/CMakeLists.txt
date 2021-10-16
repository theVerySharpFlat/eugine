
target_sources(eugine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/eugine.cpp)

target_include_directories(eugine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/eugine/vendor/glad/include)

target_precompile_headers(eugine PUBLIC egpch.h)

if(${EG_ENABLE_ASSERTIONS})
    target_compile_definitions(eugine PUBLIC EG_ENABLE_ASSERTIONS)
endif()

add_subdirectory(eugine)