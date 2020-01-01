//
// Created by Jack Armstrong on 12/31/19.
//

#include "entity_render.h"

namespace client {

    void render_player(glm::mat4 p, glm::mat4 v, const std::shared_ptr<nbt::nbt>& data, textured_cube_renderer *tcr,
                       gl::texture *tex) {
        std::shared_ptr<nbt::nbt_compound> ent = nbt::cast_compound(data);
        std::shared_ptr<nbt::nbt_list> pos = nbt::cast_list(ent->value["position"]);
        std::shared_ptr<nbt::nbt_list> look = nbt::cast_list(ent->value["lookdir"]);
        std::shared_ptr<nbt::nbt_list> size = nbt::cast_list(ent->value["bbsize"]);

        glm::mat4 m(1);
        m *= glm::translate(glm::mat4(1),
                            glm::vec3{nbt::cast_float(pos->value[0])->value, nbt::cast_float(pos->value[1])->value,
                                      nbt::cast_float(pos->value[2])->value});
        tcr->render_cube(p, v, m*glm::translate(glm::mat4(1),glm::vec3{0,6,0}/16.0F), tex, glm::vec3{8, 12, 4}, glm::vec2{16, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m*glm::translate(glm::mat4(1),glm::vec3{6,6,0}/16.0F), tex, glm::vec3{4, 12, 4}, glm::vec2{40, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m*glm::translate(glm::mat4(1),glm::vec3{-6,6,0}/16.0F), tex, glm::vec3{4, 12, 4}, glm::vec2{32, 0}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m*glm::translate(glm::mat4(1),glm::vec3{0,16,0}/16.0F),tex,glm::vec3{8,8,8},glm::vec2{0,48},glm::vec2{64,64});
        tcr->render_cube(p,v,m*glm::translate(glm::mat4(1),glm::vec3{2,-6,0}/16.0F),tex,glm::vec3{4,12,4},glm::vec2{0,32},glm::vec2{64,64});
        tcr->render_cube(p,v,m*glm::translate(glm::mat4(1),glm::vec3{-2,-6,0}/16.0F),tex,glm::vec3{4,12,4},glm::vec2{16,0},glm::vec2{64,64});
    }

}