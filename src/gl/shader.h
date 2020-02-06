//
// Created by Jack Armstrong on 11/5/19.
//

#ifndef GLMINECRAFT3_SHADER_H
#define GLMINECRAFT3_SHADER_H

#include <memory>
#include "gl/gl.h"
#include "utils/utils.h"
#include "texture.h"

namespace gl {

    class shader {
    private:
        int id;
    public:
        shader(const unsigned char *vertData, const int vertLen, const unsigned char *fragData, const int fragLen);

        ~shader();

        void bind();

        [[nodiscard]] int program_id();

        void uniform4x4(const std::string &name, glm::mat4 value);

        void uniform3(const std::string &name, glm::vec3 value);

        void uniform2(const std::string &name, glm::vec2 value);

        void uniform1(const std::string&name,float value);

        void texture(const std::string &name, gl::texture *texture, int unit);
    };

}

#endif //GLMINECRAFT3_SHADER_H
