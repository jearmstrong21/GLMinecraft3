//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"

namespace client {

    Game::Game(GLFWwindow* window) {
        this->window = window;
    }

    void Game::initialize() {
        shader=std::shared_ptr<gl::Shader>(new gl::Shader("test","test"));

        gl::MeshData data;
        data.tri = {
                0,1,2,
                1,2,3,

                4,5,6,
                5,6,7,

                8,9,10,
                9,10,11,

                12,13,14,
                13,14,15,

                16,17,18,
                17,18,19,

                20,21,22,
                21,22,23
        };
        data.buffers.push_back({3, {
            -1,-1,-1, -1,1,-1, 1,-1,-1, 1,1,-1,
            -1,-1, 1, -1,1, 1, 1,-1, 1, 1,1, 1,

            -1,-1,-1, -1,-1,1, -1,1,-1, -1,1,1,
             1,-1,-1,  1,-1,1,  1,1,-1,  1,1,1,
//
//             -1,-1,-1, -1,-1,1, 1,-1,-1, 1,-1,1,
//             -1,1,-1, -1,1,1, 1,1,-1, 1,1,1
        }});
        data.buffers.push_back({2, {
                1,0, 1,1, 0,0, 0,1,
                0,0, 0,1, 1,0, 1,1,

//                0,0, 0,1, 1,0, 1,1,
                0,0, 1,0, 0,1, 1,1,
                1,0, 0,0, 1,1, 0,1,

                0,0, 0,1, 1,0, 1,1,
                0,0, 0,1, 1,0, 1,1
        }});

        mesh =std::shared_ptr<gl::Mesh>(new gl::Mesh(data));

        texture=std::shared_ptr<gl::Texture>(new gl::Texture("New Piskel.png"));
    }

    void Game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p=glm::perspective(80.0F,1.0F,0.01F,100.0F);
        glm::mat4 v=glm::lookAt(glm::vec3(2,3,2),glm::vec3(0,0,0),glm::vec3(0,-1,0));

        shader->bind();
        shader->uniform4x4("perspective",p);
        shader->uniform4x4("view",v);
        shader->texture("tex",texture,0);
        mesh->renderTriangles();
    }

    void Game::end() {

    }

}