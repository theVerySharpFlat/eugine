project(stb_image)

add_library(stbi STATIC stb_image.cpp)
target_include_directories(stbi PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_property(TARGET stbi PROPERTY POSITION_INDEPENDENT_CODE ON)