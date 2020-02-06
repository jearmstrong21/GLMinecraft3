cmake_minimum_required(VERSION 3.15)
project(GLMinecraft3)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

include_directories(/usr/local/include glad)
include_directories(src)

add_executable(embedfile __embed_file.c)
#execute_process(COMMAND embedfile )
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_chunk_vert.c
        COMMAND embedfile SHADER_chunk_vert ../src/assets/shaders/chunk.vert
        DEPENDS src/assets/shaders/chunk.vert
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_chunk_frag.c
        COMMAND embedfile SHADER_chunk_frag ../src/assets/shaders/chunk.frag
        DEPENDS src/assets/shaders/chunk.frag
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_wireframe_vert.c
        COMMAND embedfile SHADER_wireframe_vert ../src/assets/shaders/wireframe.vert
        DEPENDS src/assets/shaders/wireframe.vert
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_wireframe_frag.c
        COMMAND embedfile SHADER_wireframe_frag ../src/assets/shaders/wireframe.frag
        DEPENDS src/assets/shaders/wireframe.frag
)
add_custom_command(
        OUTPUT ../lib/gen_assets/TEXTURE_1_8_textures_0_png.c
        COMMAND embedfile TEXTURE_1_8_textures_0_png ../src/assets/textures/1.8_textures_0.png
        DEPENDS src/assets/textures/1.8_textures_0.png
)
add_custom_command(
        OUTPUT ../lib/gen_assets/TEXTURE_ascii_png.c
        COMMAND embedfile TEXTURE_ascii_png ../src/assets/textures/ascii.png
        DEPENDS src/assets/textures/ascii.png
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_text_frag.c
        COMMAND embedfile SHADER_text_frag ../src/assets/shaders/text.frag
        DEPENDS src/assets/shaders/text.frag
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_text_vert.c
        COMMAND embedfile SHADER_text_vert ../src/assets/shaders/text.vert
        DEPENDS src/assets/shaders/text.vert
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_entity_frag.c
        COMMAND embedfile SHADER_entity_frag ../src/assets/shaders/entity.frag
        DEPENDS src/assets/shaders/entity.frag
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_entity_vert.c
        COMMAND embedfile SHADER_entity_vert ../src/assets/shaders/entity.vert
        DEPENDS src/assets/shaders/entity.vert
)
add_custom_command(
        OUTPUT ../lib/gen_assets/TEXTURE_entity_steve.c
        COMMAND embedfile TEXTURE_entity_steve ../src/assets/textures/steve.png
        DEPENDS src/assets/textures/steve.png
)
add_custom_command(
        OUTPUT ../lib/gen_assets/TEXTURE_entity_zombie.c
        COMMAND embedfile TEXTURE_entity_zombie ../src/assets/textures/zombie.png
        DEPENDS src/assets/textures/zombie.png
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_item_frag.c
        COMMAND embedfile SHADER_item_frag ../src/assets/shaders/item.frag
        DEPENDS src/assets/shaders/item.frag
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_item_vert.c
        COMMAND embedfile SHADER_item_vert ../src/assets/shaders/item.vert
        DEPENDS src/assets/shaders/item.vert
)
add_custom_command(
        OUTPUT ../lib/gen_assets/TEXTURE_gui_widgets_png.c
        COMMAND embedfile TEXTURE_gui_widgets_png ../src/assets/textures/gui_widgets.png
        DEPENDS src/assets/textures/gui_widgets.png
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_item3d_frag.c
        COMMAND embedfile SHADER_item3d_frag ../src/assets/shaders/item3d.frag
        DEPENDS src/assets/shaders/item3d.frag
)
add_custom_command(
        OUTPUT ../lib/gen_assets/SHADER_item3d_vert.c
        COMMAND embedfile SHADER_item3d_vert ../src/assets/shaders/item3d.vert
        DEPENDS src/assets/shaders/item3d.vert
)
#
#lib/gen_assets/SHADER_chunk_vert.c
#lib/gen_assets/SHADER_chunk_frag.c
#lib/gen_assets/SHADER_wireframe_vert.c
#lib/gen_assets/SHADER_wireframe_frag.c
#lib/gen_assets/TEXTURE_1_8_textures_0_png.c
#lib/gen_assets/TEXTURE_ascii_png.c
#lib/gen_assets/SHADER_text_vert.c
#lib/gen_assets/SHADER_text_frag.c
#lib/gen_assets/SHADER_entity_vert.c
#lib/gen_assets/SHADER_entity_frag.c
#lib/gen_assets/TEXTURE_entity_steve.c
#lib/gen_assets/TEXTURE_entity_zombie.c
#lib/gen_assets/SHADER_item_vert.c
#lib/gen_assets/SHADER_item_frag.c
#lib/gen_assets/TEXTURE_gui_widgets_png.c
#lib/gen_assets/SHADER_item3d_vert.c
#lib/gen_assets/SHADER_item3d_frag.c
#
file(GLOB_RECURSE SOURCE_FILES_SRC "src/**/*.cpp")
file(GLOB_RECURSE SOURCE_FILES_SRC1 "src/*.cpp")
list(APPEND SOURCE_FILES_SRC ${SOURCE_FILES_SRC1})

file(GLOB_RECURSE SOURCE_FILES_LIB "lib/**/*.cpp")
file(GLOB_RECURSE SOURCE_FILES_LIB1 "lib/*.cpp")
list(APPEND SOURCE_FILES_LIB ${SOURCE_FILES_LIB1})

set(SOURCE_FILES_GEN
        lib/gen_assets/SHADER_chunk_vert.c
        lib/gen_assets/SHADER_chunk_frag.c
        lib/gen_assets/SHADER_wireframe_vert.c
        lib/gen_assets/SHADER_wireframe_frag.c
        lib/gen_assets/TEXTURE_1_8_textures_0_png.c
        lib/gen_assets/TEXTURE_ascii_png.c
        lib/gen_assets/SHADER_text_vert.c
        lib/gen_assets/SHADER_text_frag.c
        lib/gen_assets/SHADER_entity_vert.c
        lib/gen_assets/SHADER_entity_frag.c
        lib/gen_assets/TEXTURE_entity_steve.c
        lib/gen_assets/TEXTURE_entity_zombie.c
        lib/gen_assets/SHADER_item_vert.c
        lib/gen_assets/SHADER_item_frag.c
        lib/gen_assets/TEXTURE_gui_widgets_png.c
        lib/gen_assets/SHADER_item3d_vert.c
        lib/gen_assets/SHADER_item3d_frag.c
        )

set(SOURCE_FILES )
list(APPEND SOURCE_FILES ${SOURCE_FILES_SRC})
list(APPEND SOURCE_FILES ${SOURCE_FILES_LIB})
list(APPEND SOURCE_FILES ${SOURCE_FILES_GEN})

message(STATUS "source files are: "${SOURCE_FILES})

if (APPLE)
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
endif ()

if (NOT APPLE)
    find_package(PkgConfig REQUIRED)
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    add_link_options(/usr/lib/libdl.so.2)
    add_link_options(-lpthread)
    add_link_options(-lboost_thread)
    add_link_options(-lboost_program_options)
    include_directories(${GLFW_INCLUDE_DIRS})

    add_link_options(/usr/lib/libglfw.so)
endif ()


add_executable(GLMinecraft3 ${SOURCE_FILES})
target_link_libraries(GLMinecraft3)
