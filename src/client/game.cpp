//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"

namespace client {

    Game::Game(GLFWwindow *window) {
        this->window = window;
    }

    void Game::initialize() {
        shader = std::shared_ptr<gl::Shader>(new gl::Shader("test", "test"));

        gl::MeshData data;
        block::client::initChunkBuffers(&data);

        block::World world;
//        block::BlockContext ctx=world.getCTX({0,0,0});
//        block::BlockContext ctx{0,0,0,0,0,0,block::GRASS.getDefaultState(),{0,0,0}};
        for(int x=0;x<16;x++){
            for(int y=0;y<256;y++){
                for(int z=0;z<16;z++){
                    block::fromID(block::id(world.get(x,y,z)))->render(&data,world.getCTX({x,y,z}));
                }
            }
        }
//        block::STONE.render(&data,&ctx);

        mesh = std::shared_ptr<gl::Mesh>(new gl::Mesh(&data));

        texture = std::shared_ptr<gl::Texture>(new gl::Texture("1.8_textures_0.png"));
//        texture=std::shared_ptr<gl::Texture>(new gl::Texture("New Piskel.png"));
    }

    void Game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 100.0F);
        glm::mat4 v = glm::lookAt(glm::vec3(cos(glfwGetTime())*3+0.5,sin(glfwGetTime()*0.5)*3,sin(glfwGetTime())*3+0.5), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);
        mesh->renderTriangles();

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            std::raise(11);
        }
    }

    void Game::end() {

    }

}