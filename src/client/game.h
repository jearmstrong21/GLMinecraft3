//
// Created by Jack Armstrong on 11/7/19.
//

#ifndef GLMINECRAFT3_GAME_H
#define GLMINECRAFT3_GAME_H

#include "gl/gl.h"
#include "gl/shader.h"
#include "gl/mesh.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <csignal>
#include "block/blocks.h"
#include "world/world.h"
#include "rendered_chunk.h"
#include <ctime>
#include <nbt/nbt.h>
#include <boost/asio.hpp>
#include <mutex>

//**********
namespace client {
    void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    class game {
    private:
        /********** OPENGL **********/
        gl::shader* shader;
        gl::texture* texture;
        gl::shader* wireframe_shader;
        gl::mesh* wireframe_mesh;

        void render_world();

        void initialize_gl();

        /********** GAME STATE **********/
        block::world world;
        std::map<std::string, std::shared_ptr<nbt::nbt>> entities;
        std::string player_id;
        std::mutex protect_game_state;

        bool freecam = false;

        void load_game_update(const std::shared_ptr<nbt::nbt>& obj);

        /********** NETWORKING **********/
        boost::asio::io_context io_context;
        boost::asio::ip::tcp::socket socket;

        void read_packet();

        void send_packet(const std::shared_ptr<nbt::nbt>& data);

        void download_world();

        void read_welcome_packet();

        void connect_to_server(const std::string& host, const std::string& port);

    public:
        std::shared_ptr<rendered_chunk> rendered_world[WORLD_SIZE][WORLD_SIZE];

        GLFWwindow* window;

        explicit game(GLFWwindow* window, const std::string& host, const std::string& port);

        void loop();

        void end();

    };

}

#endif //GLMINECRAFT3_GAME_H
