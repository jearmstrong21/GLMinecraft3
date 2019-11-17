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

        std::shared_ptr<block::World> world(new block::World());

        for(int x=0;x<WORLD_SIZE;x++){
            for(int z=0;z<WORLD_SIZE;z++){
                renderedWorld[x][z]=std::shared_ptr<RenderedChunk>(new RenderedChunk({x,z}));
                renderedWorld[x][z]->takeChunk(world,world->map[x][z]);
            }
        }

        texture = std::shared_ptr<gl::Texture>(new gl::Texture("1.8_textures_0.png"));
    }

    void Game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 100.0F);
        glm::mat4 v = glm::lookAt(glm::vec3(cos(glfwGetTime())*16*WORLD_SIZE/2+16*WORLD_SIZE/2,sin(glfwGetTime()*0.25)*16*WORLD_SIZE/2+16*WORLD_SIZE/2,sin(glfwGetTime())*20+8), glm::vec3(WORLD_SIZE*8, 10, WORLD_SIZE*8), glm::vec3(0, -1, 0));

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);
        for(int x=0;x<WORLD_SIZE;x++){
            for(int z=0;z<WORLD_SIZE;z++){
                renderedWorld[x][z]->render(shader);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            std::raise(11);
        }
    }

    void Game::end() {

    }

}