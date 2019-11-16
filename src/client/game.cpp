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

        block::BlockContext ctx{0,0,0,0,0,0,block::GRASS.getDefaultState(),glm::ivec3(0,0,0)};
        block::GRASS.render(&data,&ctx);

//        block::client::LayerTextureDescr ltdGrassSide;
//        ltdGrassSide.texture = block::client::AtlasTexture::GRASS_SIDE;
//        ltdGrassSide.color = glm::vec3(1, 1, 1);
//
//        block::client::LayerTextureDescr ltdGrassSideOverlay;
//        ltdGrassSideOverlay.texture = block::client::AtlasTexture::GRASS_SIDE_OVERLAY;
//        ltdGrassSideOverlay.color=glm::vec3(0.47,0.82,0.37);//#79c05a, https://minecraft.gamepedia.com/Grass_Block
//
//        block::client::LayerTextureDescr ltdGrassTop;
//        ltdGrassTop.texture=block::client::AtlasTexture::GRASS_TOP;
//        ltdGrassTop.color=glm::vec3(0.47,0.82,0.37);
//
//        block::client::LayerTextureDescr ltdDirt;
//        ltdDirt.texture=block::client::AtlasTexture::DIRT;
//        ltdDirt.color=glm::vec3(1,1,1);
//
//        block::client::LayerTextureDescr ltdNone;
//        ltdNone.texture=block::client::AtlasTexture::NONE;
//        ltdNone.color=glm::vec3(1,1,1);
//
//        block::client::QuadTextureDescr qtdGrassSide;
//        qtdGrassSide.first = ltdGrassSide;
//        qtdGrassSide.second = ltdGrassSideOverlay;
//
//        block::client::QuadTextureDescr qtdGrassTop;
//        qtdGrassTop.first=ltdGrassTop;
//        qtdGrassTop.second=ltdNone;
//
//        block::client::QuadTextureDescr qtdDirt;
//        qtdDirt.first=ltdDirt;
//        qtdDirt.second=ltdNone;
//
//
//        block::client::QuadDescr qd;
//
//        qd.texture=qtdGrassSide;qd.xmi();block::client::meshQuad(&data, qd, 0, 0, 0);
//        qd.texture=qtdGrassSide;qd.xpl();block::client::meshQuad(&data, qd, 0, 0, 0);
//        qd.texture=qtdDirt;qd.ymi();block::client::meshQuad(&data, qd, 0, 0, 0);
//        qd.texture=qtdGrassTop;qd.ypl();block::client::meshQuad(&data, qd, 0, 0, 0);
//        qd.texture=qtdGrassSide;qd.zmi();block::client::meshQuad(&data, qd, 0, 0, 0);
//        qd.texture=qtdGrassSide;qd.zpl();block::client::meshQuad(&data, qd, 0, 0, 0);

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