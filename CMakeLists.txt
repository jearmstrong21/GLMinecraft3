cmake_minimum_required(VERSION 3.15)
project(GLMinecraft3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

add_link_options(/usr/lib/libdl.so.2)

include_directories(/usr/local/include glad)
include_directories(${GLFW_INCLUDE_DIRS})

#find_library(COCOA_LIBRARY Cocoa REQUIRED)
#find_library(IOKIT_LIBRARY IOKit REQUIRED)
#find_library(COREVID_LIBRARY CoreVideo REQUIRED)

include_directories(src)
set(SOURCE_FILES
        src/main.cpp
        src/utils.cpp
        src/gl/gl.cpp
        src/gl/glad.cpp
        src/gl/texture.cpp
        src/gl/shader.cpp
        src/gl/mesh.cpp
        src/client/game.cpp
        )

add_executable(GLMinecraft3 ${SOURCE_FILES})
target_link_libraries (GLMinecraft3
        ${OPENGL_LIBRARIES}
#        ${COCOA_LIBRARY}
#        ${COREVID_LIBRARY}
#        ${IOKIT_LIBRARY}
        ${GLFW3_LIBRARY} glfw)