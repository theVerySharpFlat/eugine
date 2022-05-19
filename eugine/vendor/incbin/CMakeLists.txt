project(incbin)

add_library(incbin STATIC incbin/incbin.c)
target_include_directories(incbin PUBLIC incbin)
