project(Whereami)

add_library(wai STATIC whereami/src/whereami.c)
target_include_directories(wai PUBLIC whereami/src)