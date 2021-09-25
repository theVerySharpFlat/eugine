project(eugine-engine)

if(NOT EG_BUILD_TYPE)
    set(EG_BUILD_TYPE SHARED)
endif()

add_library(eugine ${EG_BUILD_TYPE})

if(EG_BUILD_TYPE STREQUAL SHARED)
    target_compile_definitions(eugine PRIVATE EG_BUILD_TYPE_SHARED)

else()
    target_compile_definitions(eugine PRIVATE EG_BUILD_TYPE_STATIC)
endif()
target_compile_definitions(eugine PRIVATE EG_BUILDING_SHARED_LIB)

add_subdirectory(vendor)

find_package(OpenGL REQUIRED)

target_link_libraries(eugine PUBLIC spdlog glad)


target_link_libraries(eugine PRIVATE OpenGL::GL glfw imgui)

add_subdirectory(src)