//
// Created by Jack Armstrong on 11/7/19.
//

#include "game.h"

namespace client {

    Game::Game(GLFWwindow *window) {
        this->window=window;
    }

    void Game::initialize(){
        shader=new gl::Shader("../shaders/test.vert","../shaders/test.frag");

        gl::MeshData data;
        data.tri={0,1,3, 1,2,3};
        data.buffers.push_back({2, {.5,.5,.5,-.5,-.5,-.5,-.5,.5}});
        data.vertCount=6;

        mesh=new gl::Mesh(data);
    }

    void Game::loop() {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClearColor(0.2,0.3,0.3,1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->bind();
        mesh->renderTriangles();
    }

    void Game::end() {
        delete shader;
        delete mesh;
    }

}