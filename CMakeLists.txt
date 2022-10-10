cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Release")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options("-O3")
endif()

project(Raytracing)

set(SOURCE_FILE
    main.cpp
    Matrix3.cpp
    Transform.cpp
    Sphere.cpp
    Triangle.cpp
    Renderer.cpp
    HittableList.cpp
    TransformedObject.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR})
