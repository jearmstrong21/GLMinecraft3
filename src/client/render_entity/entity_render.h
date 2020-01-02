//
// Created by Jack Armstrong on 12/31/19.
//

#ifndef GLMINECRAFT3_ENTITY_RENDER_H
#define GLMINECRAFT3_ENTITY_RENDER_H

#include "entity_render.h"
#include "entity/entity.h"
#include "textured_cube_render.h"

namespace client {

    struct entity_render {

        gl::texture*steve_texture;
        gl::texture*zombie_texture;
        textured_cube_renderer*tcr;

        entity_render();
        ~entity_render();

        void render_player(glm::mat4 p, glm::mat4 v, const std::shared_ptr<nbt::nbt> &data);
        void render_zombie(glm::mat4 p,glm::mat4 v, const std::shared_ptr<nbt::nbt>&data);

    };

}

#endif //GLMINECRAFT3_ENTITY_RENDER_H
