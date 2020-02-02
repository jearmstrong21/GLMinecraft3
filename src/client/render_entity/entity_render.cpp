//
// Created by Jack Armstrong on 12/31/19.
//

#include "entity_render.h"

extern "C" const unsigned char TEXTURE_entity_steve[];
extern "C" const size_t TEXTURE_entity_steve_len;

extern "C" const unsigned char TEXTURE_entity_zombie[];
extern "C" const size_t TEXTURE_entity_zombie_len;

namespace client {

    entity_render::entity_render() {
        tcr = new textured_cube_renderer();
        steve_texture = new gl::texture(TEXTURE_entity_steve, TEXTURE_entity_steve_len);
        zombie_texture = new gl::texture(TEXTURE_entity_zombie, TEXTURE_entity_zombie_len);
    }

    entity_render::~entity_render() {
        delete tcr;
        delete steve_texture;
        delete zombie_texture;
    }

    void entity_render::render_player(glm::mat4 p, glm::mat4 v, const entity::entity_ptr &data) {
        glm::mat4 m(1);
        m *= glm::translate(glm::mat4(1), data->box.pos);
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 12, 0} / 16.0F), steve_texture,//arm
                         glm::vec3{8, 12, 4},
                         glm::vec2{16, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{6, 12, 0} / 16.0F), steve_texture,//body
                         glm::vec3{4, 12, 4},
                         glm::vec2{40, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-6, 12, 0} / 16.0F), steve_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{32, 0}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 24, 0} / 16.0F), steve_texture,
                         glm::vec3{8, 8, 8},
                         glm::vec2{0, 48}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{2, 0, 0} / 16.0F), steve_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{0, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-2, 0, 0} / 16.0F), steve_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{16, 0}, glm::vec2{64, 64});
    }

    void entity_render::render_zombie(glm::mat4 p, glm::mat4 v, const entity::entity_ptr &data) {
        glm::mat4 m(1);
        m *= glm::translate(glm::mat4(1), data->box.pos);
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 12, 0} / 16.0F), zombie_texture,
                         glm::vec3{8, 12, 4},
                         glm::vec2{16, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{6, 12, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{40, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-6, 12, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{40, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 24, 0} / 16.0F), zombie_texture,
                         glm::vec3{8, 8, 8},
                         glm::vec2{0, 48}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{2, 0, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{0, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-2, 0, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{0, 32}, glm::vec2{64, 64});
    }

}