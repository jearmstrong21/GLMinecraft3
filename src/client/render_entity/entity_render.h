//
// Created by Jack Armstrong on 12/31/19.
//

#ifndef GLMINECRAFT3_ENTITY_RENDER_H
#define GLMINECRAFT3_ENTITY_RENDER_H

#include "entity_render.h"
#include "entity/entity.h"
#include "textured_cube_render.h"
#include "entity/entities.h"

namespace client {

    struct entity_render {

        gl::texture *steve_texture;
        gl::texture *zombie_texture;
        gl::texture *pig_texture;
        textured_cube_renderer *tcr;

        gl::mesh *item_mesh;
        gl::shader *item_shader;

        entity_render();

        ~entity_render();

        void render_player(glm::mat4 p, glm::mat4 v, const entity::entity_ptr &data, gl::texture *item_texture,
                           item_renderer *item_rend);

        void render_zombie(glm::mat4 p, glm::mat4 v, const entity::entity_ptr &data);

        void render_pig(glm::mat4 p,glm::mat4 v,const entity::entity_ptr &data);

    };

}

#endif //GLMINECRAFT3_ENTITY_RENDER_H
