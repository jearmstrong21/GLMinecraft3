//
// Created by Jack Armstrong on 11/4/19.
//

#include "gl/texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "gl/stb_image.h"

namespace gl {

    Texture::Texture(const std::string &filename) {
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        int w, h, comps;
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(("assets/" + filename).c_str(), &w, &h, &comps, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            printf("F %s,%i", __FILE__, __LINE__);
        }
        stbi_image_free(data);
        glBindTexture(GL_TEXTURE_2D,0);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &id);
    }

    void Texture::bind(int unit) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }

}