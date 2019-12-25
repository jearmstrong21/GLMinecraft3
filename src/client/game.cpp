//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"
#include <thread>
#include <boost/asio.hpp>

namespace client {

    game::game(GLFWwindow* window) {
        this->window = window;
    }

    void game::initialize() {
        world=std::make_shared<block::world>();
        shader = std::make_shared<gl::shader>("test", "test");
        texture = std::make_shared<gl::texture>("1.8_textures_0.png");
    }

    void game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 1000.0F);
        glm::mat4 v = glm::lookAt(glm::vec3(cos(glfwGetTime() * 0.25) * 16 * WORLD_SIZE / 2 + 16 * WORLD_SIZE / 2,
                                            sin(glfwGetTime() * 0.25) * 32 + 64,
                                            sin(glfwGetTime() * 0.25) * 16 * WORLD_SIZE / 2 + 16 * WORLD_SIZE / 2),
                                  glm::vec3(WORLD_SIZE * 8, 60, WORLD_SIZE * 8), glm::vec3(0, -1, 0));

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);

        for (int x = 0; x < WORLD_SIZE; x++) {
            for (int z = 0; z < WORLD_SIZE; z++) {
//                if (rendered_world[x][z] != nullptr)
                rendered_world[x][z]->render(shader);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            std::raise(11);
        }
    }

    void game::end() {

    }

    void game::download_world() {
        try {
//            for(int x=0;x<WORLD_SIZE*16;x++){
//                for(int y=0;y<256;y++){
//                    for(int z=0;z<WORLD_SIZE*16;z++){
//                        if(rand()%10==5){
//                            world->set(x,y,z,block::STONE.defaultState);
//                        }
//                    }
//                }
//            }
            boost::asio::io_context io_context;

            boost::asio::ip::tcp::resolver resolver(io_context);
            boost::asio::ip::tcp::resolver::results_type endpoints=resolver.resolve("localhost","daytime");

            boost::asio::ip::tcp::socket socket(io_context);
            boost::asio::connect(socket,endpoints);

            boost::system::error_code err;
            boost::array<long, 4096>arr{};
            for(int x=0;x<WORLD_SIZE;x++) {
                for (int y = 0; y < 16; y++) {
                    for (int z = 0; z < WORLD_SIZE; z++) {
                        size_t len=socket.read_some(boost::asio::buffer(arr),err);
                        if(err==boost::asio::error::eof){
                            printf("UNEXPECTED EOF %i %i %i\n",x,y,z);
                            std::raise(11);
                        }else if(err){
                            throw boost::system::system_error(err);
                        }
                        world->map[x][z]->read(y,arr);
                    }
                }
            }



            for (int x = 0; x < WORLD_SIZE; x++) {
                for (int z = 0; z < WORLD_SIZE; z++) {
                    rendered_world[x][z]=std::make_shared<rendered_chunk>(glm::ivec2{x,z});
                    rendered_world[x][z]->take_chunk(world,world->map[x][z]);
                    rendered_world[x][z]->render_chunk();
                }
            }
        }catch(std::exception&e){
            std::cerr<<"client ex caught: "<<e.what()<<"\n";
            std::raise(11);
        }
    }

}