//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"

namespace client {

    game::game(GLFWwindow *window) {
        this->window = window;
    }

    void game::initialize() {
        shader = std::shared_ptr<gl::shader>(new gl::shader("test", "test"));

        std::shared_ptr<block::world> world(new block::world());

        for(int x=0;x<WORLD_SIZE;x++){
            for(int z=0;z<WORLD_SIZE;z++){
                rendered_world[x][z]=std::shared_ptr<rendered_chunk>(new rendered_chunk({x, z}));
                rendered_world[x][z]->take_chunk(world, world->map[x][z]);
            }
        }

        texture = std::shared_ptr<gl::texture>(new gl::texture("1.8_textures_0.png"));
    }

    void game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.7, 0.9, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p = glm::perspective(80.0F, 1.0F, 0.01F, 1000.0F);
        glm::mat4 v = glm::lookAt(glm::vec3(cos(glfwGetTime())*16*WORLD_SIZE/2+16*WORLD_SIZE/2,sin(glfwGetTime()*0.25)*32*WORLD_SIZE/2+16*WORLD_SIZE/2,sin(glfwGetTime())*20+8), glm::vec3(WORLD_SIZE*8, 60, WORLD_SIZE*8), glm::vec3(0, -1, 0));

        shader->bind();
        shader->uniform4x4("perspective", p);
        shader->uniform4x4("view", v);
        shader->texture("tex", texture, 0);
        for(int x=0;x<WORLD_SIZE;x++){
            for(int z=0;z<WORLD_SIZE;z++){
                rendered_world[x][z]->render(shader);
            }
        }

        if (glfwGetKey(window, GLFW_KEY_Q)) {
            std::raise(11);
        }
    }

    void game::end() {

    }

}