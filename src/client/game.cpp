//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"

namespace client {

    Game::Game(GLFWwindow* window) {
        this->window = window;
    }

    void Game::initialize() {
        shader=std::shared_ptr<gl::Shader>(new gl::Shader("../shaders/test.vert", "../shaders/test.frag"));

        gl::MeshData data;
        data.tri = {0,1,2, 1,2,3};
        float f=0.5;
        data.buffers.push_back({3, {-f,-f,0, -f,f,0, f,-f,0, f,f,0}});
        data.buffers.push_back({3, {0,0,0, 1,0,0, 0,1,0, 1,1,0}});
        data.vertCount = 6;

        mesh =std::shared_ptr<gl::Mesh>(new gl::Mesh(data));
    }

    void Game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.2, 0.3, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 p=glm::perspective(80.0F,1.0F,0.01F,100.0F);
        glm::mat4 v=glm::lookAt(glm::vec3(1,2,2),glm::vec3(0,0,0),glm::vec3(0,1,0));

        shader->bind();
        shader->uniform4x4("perspective",p);
        shader->uniform4x4("view",v);
        mesh->renderTriangles();
    }

    void Game::end() {

    }

}