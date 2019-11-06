//
// Created by Jack Armstrong on 11/4/19.
//

#ifndef GLMINECRAFT3_TEXTURE_H
#define GLMINECRAFT3_TEXTURE_H

#include <string>
#include "gl/gl.h"

namespace gl {

    class Texture {
    private:
        GLuint id;

    public:
        Texture(const std::string &filename);

        ~Texture();

        void bind(int unit);


    };

}

#endif //GLMINECRAFT3_TEXTURE_H
