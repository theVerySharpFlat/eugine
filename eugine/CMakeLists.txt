project(eugine-engine)

set(SHARED_OR_STATIC "SHARED")


add_library(eugine ${SHARED_OR_STATIC})

add_subdirectory(src)


target_compile_definitions(eugine PRIVATE EG_BUILD_SHARED_LIB)

add_subdirectory(vendor)

target_link_libraries(eugine PUBLIC glfw spdlog)