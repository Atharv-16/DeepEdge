cmake_minimum_required(VERSION 3.10)
project(ImageResizeSIMD)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(ImageResizeSIMD main.cpp)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2 -fopenmp")
    target_link_libraries(ImageResizeSIMD PRIVATE gomp)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2 /openmp")
endif()

target_link_libraries(ImageResizeSIMD PRIVATE ${OpenCV_LIBS})
