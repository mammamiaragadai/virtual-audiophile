cmake_minimum_required(VERSION 3.11)
project(ScnnDirectivities)

add_definitions(-DMATIO_MAT73=OFF)
set(MATIO_MAT73 OFF CACHE BOOL OFF FORCE)
add_subdirectory(deps/matio)
include_directories(deps/matio/src build/deps/matio/src)

add_executable(${PROJECT_NAME} src/main.c src/directivity.c)
target_link_libraries(${PROJECT_NAME} matio)