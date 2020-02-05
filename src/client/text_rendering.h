//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_TEXT_RENDERING_H
#define GLMINECRAFT3_TEXT_RENDERING_H

#include "gl/gl.h"
#include "gl/shader.h"
#include "gl/mesh.h"
#include "gl/texture.h"

extern "C" const unsigned char TEXTURE_ascii_png[];
extern "C" const size_t TEXTURE_ascii_png_len;

extern "C" const unsigned char SHADER_text_frag[];
extern "C" const size_t SHADER_text_frag_len;

extern "C" const unsigned char SHADER_text_vert[];
extern "C" const size_t SHADER_text_vert_len;

namespace client {

    struct text_renderer {

        gl::shader *shader;
        gl::mesh *mesh;
        gl::texture *texture;
        GLFWwindow *window;

        explicit text_renderer(GLFWwindow *window) : window(window) {
            shader = new gl::shader(SHADER_text_vert, SHADER_text_vert_len, SHADER_text_frag, SHADER_text_frag_len);
            gl::mesh_data data{
                    {{2, {0, 0, 1, 0, 0, 1, 1, 1}}, {2, {0, 0, 1, 0, 0, 1, 1, 1}}},
                    {0,                             1, 2, 1, 2, 3}
            };
            mesh = new gl::mesh(&data);
            texture = new gl::texture(TEXTURE_ascii_png, TEXTURE_ascii_png_len);
        }

        glm::mat4 get_ortho() {
            int w, h;
//            glfwGetWindowSize(window,&w,&h);
            w = h = 1000;//TODOD what the fric k
//            return glm::ortho(0,w,0,h);
//return glm::mat4(1);
            return glm::translate(glm::mat4(1), glm::vec3{-1, -1, 0}) *
                   glm::scale(glm::mat4(1), 2.0F * glm::vec3{1.0 / w, 1.0 / h, 0});
        }

        int charsize = 16;

        void render_string(std::string s, int x, int y) {
            int cx = x, cy = y;
            for (int i = 0; i < s.size(); i++) {
                if (s[i] == '\n') {
                    cy -= charsize;
                    cx = x;
                    continue;
                }
                render_character(s[i], cx, cy);
                cx += charsize;
            }
        }

        void render_character(char c, int x, int y) {
            shader->bind();
            glm::mat4 p = get_ortho();
            p *= glm::translate(glm::mat4(1), glm::vec3{x, y, 0});
            p *= glm::scale(glm::mat4(1), glm::vec3{(float) charsize});
            shader->uniform4x4("projection", p);
            shader->texture("tex", texture, 0);
            int i = (int) c;
            x = 15 - c / 16;
            y = c % 16;
            shader->uniform2("inUV", glm::vec2{y / 16.0F, x / 16.0F});
            mesh->render_triangles();
        }

        ~text_renderer() {
            delete shader;
            delete mesh;
            delete texture;
        }

    };

}

#endif //GLMINECRAFT3_TEXT_RENDERING_H
