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

namespace client {

    class game {
    public:

        std::shared_ptr<gl::shader>shader;
        std::shared_ptr<gl::texture>texture;
        std::shared_ptr<rendered_chunk>rendered_world[WORLD_SIZE][WORLD_SIZE];

        GLFWwindow* window;

        explicit game(GLFWwindow* window);

        void initialize();

        void loop();

        void end();

    };

}

#endif //GLMINECRAFT3_GAME_H
