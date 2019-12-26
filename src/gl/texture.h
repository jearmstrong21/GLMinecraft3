//
// Created by Jack Armstrong on 11/4/19.
//

#ifndef GLMINECRAFT3_TEXTURE_H
#define GLMINECRAFT3_TEXTURE_H

#include <string>
#include "gl/gl.h"

namespace gl {

    class texture {
    private:
        GLuint id;

    public:
        texture(const unsigned char*_data,int _len);

        ~texture();

        void bind(int unit);


    };

}

#endif //GLMINECRAFT3_TEXTURE_H
