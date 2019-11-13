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

        block::client::LayerTextureDescr ltd0;
        ltd0.texture = block::client::AtlasTexture::GRASS_SIDE;
        ltd0.color = glm::vec3(1, 1, 1);

        block::client::LayerTextureDescr ltd1;
        ltd1.texture = block::client::AtlasTexture::GRASS_SIDE_OVERLAY;
        ltd1.color=glm::vec3(0.47,0.82,0.37);//#79c05a, https://minecraft.gamepedia.com/Grass_Block

        block::client::QuadTextureDescr qdt;
        qdt.first = ltd0;
        qdt.second = ltd1;

        block::client::QuadDescr qd;
        qd.start = glm::vec3(1, 0, 0);
        qd.d0 = glm::vec3(0, 0, 1);
        qd.d1 = glm::vec3(0, 1, 0);
        qd.texture = qdt;

        block::client::meshQuad(&data, qd, 0, 0, 0);
        qd.start = glm::vec3(0, 0, 0);
        block::client::meshQuad(&data, qd, 0, 0, 0);

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
        glm::mat4 v = glm::lookAt(glm::vec3(2, 1.5, 2), glm::vec3(0, 0, 0), glm::vec3(0, -1, 0));

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