//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_SHADER_H
#define GLMINECRAFT3_SHADER_H

#include "gl/gl.h"
#include "utils/utils.h"
#include "texture.h"

namespace gl {

    class Shader {
    private:
        int id;
    public:
        Shader(const std::string& vert, const std::string& frag);

        ~Shader();

        void bind();

        int programID();

        void uniform4x4(const std::string& name,glm::mat4 value);
        void texture(const std::string &name,std::shared_ptr<gl::Texture>texture, int unit);
    };

}

#endif //GLMINECRAFT3_SHADER_H
