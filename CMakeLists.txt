cmake_minimum_required(VERSION 3.15)
project(GLMinecraft3)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(/usr/local/include glad)
include_directories(src)

add_executable(embedfile __embed_file.c)
#execute_process(COMMAND embedfile )
add_custom_command(
        OUTPUT ../src/gen_assets/SHADER_chunk_vert.c
        COMMAND embedfile SHADER_chunk_vert ../src/assets/shaders/chunk.vert
        DEPENDS src/assets/shaders/chunk.vert
)
add_custom_command(
        OUTPUT ../src/gen_assets/SHADER_chunk_frag.c
        COMMAND embedfile SHADER_chunk_frag ../src/assets/shaders/chunk.frag
        DEPENDS src/assets/shaders/chunk.frag
)
add_custom_command(
        OUTPUT ../src/gen_assets/SHADER_wireframe_vert.c
        COMMAND embedfile SHADER_wireframe_vert ../src/assets/shaders/wireframe.vert
        DEPENDS src/assets/shaders/wireframe.vert
)
add_custom_command(
        OUTPUT ../src/gen_assets/SHADER_wireframe_frag.c
        COMMAND embedfile SHADER_wireframe_frag ../src/assets/shaders/wireframe.frag
        DEPENDS src/assets/shaders/wireframe.frag
)
add_custom_command(
        OUTPUT ../src/gen_assets/TEXTURE_1_8_textures_0_png.c
        COMMAND embedfile TEXTURE_1_8_textures_0_png ../src/assets/textures/1.8_textures_0.png
        DEPENDS src/assets/textures/1.8_textures_0.png
)
#
#src/gen_assets/SHADER_chunk_vert.c
#src/gen_assets/SHADER_chunk_frag.c
#src/gen_assets/SHADER_wireframe_vert.c
#src/gen_assets/SHADER_wireframe_frag.c
#
set(SOURCE_FILES
        src/gen_assets/SHADER_chunk_vert.c
        src/gen_assets/SHADER_chunk_frag.c
        src/gen_assets/SHADER_wireframe_vert.c
        src/gen_assets/SHADER_wireframe_frag.c
        src/gen_assets/TEXTURE_1_8_textures_0_png.c

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

        src/server.cpp
        src/client.cpp
        src/both.h
        )

if(APPLE)
    include_directories(/usr/local/include/GLFW)
    include_directories(/usr/local/include/GL)

    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    find_library(IOKIT_LIBRARY IOKit REQUIRED)
    find_library(COREVID_LIBRARY CoreVideo REQUIRED)

    link_libraries(${COCOA_LIBRARY})
    link_libraries(${IOKIT_LIBRARY})
    link_libraries(${COREVID_LIBRARY})

    link_libraries(/usr/local/lib/libboost_thread-mt.a)
    link_libraries(/usr/local/lib/libboost_program_options-mt.a)
    link_libraries(/usr/local/lib/libglfw3.a)
endif()

if(NOT APPLE)
    find_package (PkgConfig REQUIRED)
    find_package (OpenGL REQUIRED)
    find_package (glfw3 REQUIRED)
    add_link_options(/usr/lib/libdl.so.2)
    add_link_options(-lpthread)
    add_link_options(-lboost_thread)
    add_link_options(-lboost_program_options)
    include_directories(${GLFW_INCLUDE_DIRS})

    add_link_options(/usr/lib/libglfw.so)
endif()


add_executable(GLMinecraft3 ${SOURCE_FILES})
target_link_libraries (GLMinecraft3)
