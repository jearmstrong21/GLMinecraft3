//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_SHADER_H
#define GLMINECRAFT3_SHADER_H

#include "gl/gl.h"
#include "utils.h"

namespace gl {

    class Shader {
    private:
        int id;
    public:
        Shader(const std::string& vert, const std::string& frag);

        ~Shader();

        void bind();

    };

}

#endif //GLMINECRAFT3_SHADER_H
