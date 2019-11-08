//
// Created by Jack Armstrong on 11/7/19.
//

#ifndef GLMINECRAFT3_GAME_H
#define GLMINECRAFT3_GAME_H

#include "gl/gl.h"
#include "gl/shader.h"
#include "gl/mesh.h"

namespace client {

    class Game {
    public:

        gl::Shader*shader;
        gl::Mesh*mesh;

        GLFWwindow* window;

        Game(GLFWwindow* window);

        void initialize();

        void loop();

        void end();

    };

}

#endif //GLMINECRAFT3_GAME_H
