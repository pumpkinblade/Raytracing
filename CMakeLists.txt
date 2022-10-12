cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        add_compile_options("/O3")
    else()
        add_compile_options("-O3")
    endif()
endif()

project(Raytracing)

file(GLOB SOURCE_FILE ${PROJECT_SOURCE_DIR}/*.h ${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
