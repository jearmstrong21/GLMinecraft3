//
// Created by Jack Armstrong on 11/7/19.
//

#ifndef GLMINECRAFT3_GAME_H
#define GLMINECRAFT3_GAME_H

#include "gl/gl.h"
#include "gl/shader.h"
#include "gl/mesh.h"
#include "gl/texture.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <csignal>
#include "block/blocks.h"
#include "world/world.h"
#include "rendered_chunk.h"
#include <ctime>
#include <nbt/nbt.h>
#include <boost/asio.hpp>
#include <mutex>
#include "utils/utils.h"
#include "text_rendering.h"
#include "item_renderer.h"
#include "render_entity/textured_cube_render.h"
#include "render_entity/entity_render.h"
#include "block_rendering.h"
#include "gui_ctx.h"
#include "entity/entities.h"
#include "box_renderer.h"

namespace client {

    class game {
    private:
        int width, height, fboWidth, fboHeight;
        /********** OPENGL **********/
        gl::shader *shader;
        gl::texture *texture;
        gl::texture *gui_widgets_texture;
        text_renderer *text_rend;
        box_renderer *box_rend;
        entity_render *ent_rend;
        item_renderer *item_rend;
        float rotX = 0, rotY = 0;
        bool attemptQuit = false;
        std::shared_ptr<entity::entity_player> player;
        bool firstperson;

        void render_world();

        void initialize_gl();

        void glfw_key_press_callback(int key, int scancode, int actions, int mods);

        void render_chat();

        /********** GAME STATE **********/
        block::world world;
        std::map<std::string, entity::entity_ptr> entities;
        std::string player_id;
        std::mutex protect_game_state;

        bool freecam = false;
        glm::vec3 freecamPos;
        glm::vec3 lookdir;
        std::string curchatbuffer;
        bool is_chat_open = false;
        bool ignore_character = true;
        std::string chattosend;
        std::vector<std::string> chathistory;
        std::stack<glm::ivec3> dirty_chunk_sections;

        void load_game_update(const std::shared_ptr<nbt::nbt> &obj);

        void handle_char(char c);

        /********** NETWORKING **********/
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::socket socket;

        void read_packet();

        void send_packet(const std::shared_ptr<nbt::nbt> &data);

        void download_world();

        void read_welcome_packet();

        void connect_to_server(const std::string &host, const std::string &port);

        void open_chat();

        void close_chat();

        void render_hud();

    public:
        std::shared_ptr<rendered_chunk> rendered_world[WORLD_SIZE][WORLD_SIZE];
        GLFWwindow *window;

        explicit game(GLFWwindow *window, const std::string &host, const std::string &port);

        void loop();

        void end();

    };

}

#endif //GLMINECRAFT3_GAME_H
