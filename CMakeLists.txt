cmake_minimum_required(VERSION 3.16)

project(eugine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

#platform detection
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    message("building for linux")
    add_compile_definitions(EG_PLATFORM_LINUX)
    set(OpenGL_GL_PREFERENCE "GLVND")
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    message("building for windows")
    add_compile_definitions(EG_PLATFORM_WINDOWS)
elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
    message("building for OSX")
    add_compile_definitions(EG_PLATFORM_DARWIN)
endif()

add_subdirectory(eugine)
add_subdirectory(sandbox)
