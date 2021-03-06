//
// Created by Jack Armstrong on 11/4/19.
//

#include "gl/texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include "../../lib/stb_image.h"

namespace gl {

    texture::texture(const unsigned char *_data, int _len) : id(-1) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        int w, h, chn;
        stbi_set_flip_vertically_on_load(true);
//        unsigned char*data=stbi_load(("../src/assets/textures/"+filename).c_str(),&w,&h,&chn,0);
        unsigned char *data = stbi_load_from_memory(_data, _len, &w, &h, &chn, 0);

        GLenum frmt = GL_RGBA;
//        printf("Loaded file with dimensions %ix%i and channels %i\n",w,h,chn);
        if (chn == 3)frmt = GL_RGB;

        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, frmt, w, h, 0, frmt, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            printf("Failed to load texture\n");
            stbi_image_free(data);
            exit(1);
        }
    }

    texture::~texture() {
        glDeleteTextures(1, &id);
    }

    void texture::bind(int unit) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

}