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

namespace client {

    class game {
    public:

        std::shared_ptr<gl::shader>shader;
        std::shared_ptr<gl::texture>texture;
        std::shared_ptr<block::world>world;
        std::shared_ptr<rendered_chunk>rendered_world[WORLD_SIZE][WORLD_SIZE];

        GLFWwindow* window;

        explicit game(GLFWwindow* window,const std::string&host,const std::string&port);

//        void download_world(const std::string& host,std::string port);
        void read_packet();
        void send_packet(std::shared_ptr<nbt::nbt>data);
        std::vector<std::shared_ptr<nbt::nbt>>write_msgs;

        boost::asio::io_context io_context;
        boost::asio::ip::tcp::socket socket;

//        boost::asio::io_context

        void loop();

        void end();

    private:
        void do_write();

    };

}

#endif //GLMINECRAFT3_GAME_H
