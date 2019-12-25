cmake_minimum_required(VERSION 3.15)
project(GLMinecraft3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_MODULE_PATH /usr/local/lib/cmake)
set(CMAKE_PREFIX_PATH /usr/local/lib/cmake/glfw3)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

find_package (PkgConfig REQUIRED)
find_package (OpenGL REQUIRED)
find_package (glfw3 REQUIRED)

if(NOT APPLE)
    add_link_options(/usr/lib/libdl.so.2)
    add_link_options(-lpthread)
    add_link_options(-lboost_thread)
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
        src/utils/noise/noise.cpp
        src/gl/gl.cpp
        src/gl/glad.cpp
        src/gl/texture.cpp
        src/gl/shader.cpp
        src/gl/mesh.cpp
        src/client/game.cpp
        src/client/block_rendering.cpp
        src/client/rendered_chunk.cpp
        src/server/registry.cpp
        src/block/blockstate.cpp
        src/block/block.cpp
        src/world/world.cpp
        src/block/blocks.cpp

        src/block/types/block_grass.cpp
        src/block/types/block_dirt.cpp
        src/block/types/block_stone.cpp
        src/block/types/block_none.cpp

        src/world/gen/carver/base_carver.cpp
        src/world/gen/features/base_feature.cpp
        src/world/gen/surface/base_surface.cpp
        src/world/gen/structures/base_structure.cpp
        src/world/gen/surface/simple_grass_surface.cpp
        src/world/gen/carver/simple_carver.cpp

        src/nbt/__nbt_compound.cpp
        src/nbt/__nbt_float.cpp
        src/nbt/__nbt_int.cpp
        src/nbt/__nbt_list.cpp
        src/nbt/__nbt_long.cpp
        src/nbt/__nbt_short.cpp
        src/nbt/__nbt_string.cpp
        src/nbt/__nbt_utils.cpp
        src/nbt/nbt.cpp

        src/server/entity.cpp
        src/server/server.cpp
        src/client.cpp src/both.h src/server.cpp src/networking/message.h src/utils/eventable.cpp src/utils/eventable.h)

set(MAC_LIBS)

if(APPLE)
    set(MAC_LIBS
            ${COCOA_LIBRARY}
            ${COREVID_LIBRARY}
            ${IOKIT_LIBRARY}
            )
endif()

if(APPLE)
    link_libraries(/usr/local/lib/libboost_thread-mt.dylib)
endif()
add_executable(GLMinecraft3 ${SOURCE_FILES})
target_link_libraries (GLMinecraft3
        ${OPENGL_LIBRARIES}
#        ${COCOA_LIBRARY}
#        ${COREVID_LIBRARY}
#        ${IOKIT_LIBRARY}
        ${MAC_LIBS}
        ${GLFW3_LIBRARY} glfw)
