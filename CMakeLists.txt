cmake_minimum_required(VERSION 3.11)
project(ScnnDirectivities)

set(MATIO_MAT73 OFF CACHE BOOL OFF FORCE)
add_subdirectory(deps/matio)
include_directories(deps/matio/src build/deps/matio/src)
include_directories(deps/matplotlib-cpp)
add_subdirectory(deps/fftw-3.3.10)
include_directories(deps/fftw-3.3.10/api)

find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
find_package(Python3 COMPONENTS NumPy)
add_executable(${PROJECT_NAME} src/main.cpp src/directivity.cpp)
target_link_libraries(${PROJECT_NAME} matio fftw3)
target_link_libraries(${PROJECT_NAME} Python3::Python Python3::Module)
if(Python3_NumPy_FOUND)
target_link_libraries(${PROJECT_NAME} Python3::NumPy)
else()
target_compile_definitions(${PROJECT_NAME} WITHOUT_NUMPY)
endif()