cmake_minimum_required(VERSION 3.15)
project(GLMinecraft3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

if(NOT APPLE)
    add_link_options(/usr/lib/libdl.so.2)
endif()

include_directories(/usr/local/include glad)
include_directories(${GLFW_INCLUDE_DIRS})

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)
endif()

include_directories(src)
set(SOURCE_FILES
        src/main.cpp
        src/utils/utils.cpp
        src/gl/gl.cpp
        src/gl/glad.cpp
        src/gl/texture.cpp
        src/gl/shader.cpp
        src/gl/mesh.cpp
        src/client/game.cpp
        src/client/block_rendering.cpp
        src/server/registry.cpp
        )

set(MAC_LIBS)

if(APPLE)
    set(MAC_LIBS
            ${COCOA_LIBRARY}
            ${COREVID_LIBRARY}
            ${IOKIT_LIBRARY}
            )
endif()

add_executable(GLMinecraft3 ${SOURCE_FILES})
target_link_libraries (GLMinecraft3
        ${OPENGL_LIBRARIES}
#        ${COCOA_LIBRARY}
#        ${COREVID_LIBRARY}
#        ${IOKIT_LIBRARY}
        ${MAC_LIBS}
        ${GLFW3_LIBRARY} glfw)