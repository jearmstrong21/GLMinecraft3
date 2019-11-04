cmake_minimum_required(VERSION 3.15)
project(GLMinecraft3)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

include_directories(/usr/local/include glad)
include_directories(${GLFW_INCLUDE_DIRS})

find_library(COCOA_LIBRARY Cocoa REQUIRED)
find_library(IOKIT_LIBRARY IOKit REQUIRED)
find_library(COREVID_LIBRARY CoreVideo REQUIRED)

add_executable(GLMinecraft3
        main.cpp
        glad.h glad.c
        khrplatform.h
        gl.h
        gl.cpp
        )
target_link_libraries (GLMinecraft3
        ${OPENGL_LIBRARIES}
        ${COCOA_LIBRARY}
        ${COREVID_LIBRARY}
        ${IOKIT_LIBRARY}
        ${GLFW3_LIBRARY} glfw)