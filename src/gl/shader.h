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
        shader(const std::string& vert, const std::string& frag);

        ~shader();

        void bind();

        [[nodiscard]] int program_id();

        void uniform4x4(const std::string& name, glm::mat4 value);

        void texture(const std::string& name, std::shared_ptr<gl::texture> texture, int unit);
    };

}

#endif //GLMINECRAFT3_SHADER_H
