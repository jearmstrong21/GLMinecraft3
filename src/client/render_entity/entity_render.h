//
// Created by Jack Armstrong on 12/31/19.
//

#ifndef GLMINECRAFT3_ENTITY_RENDER_H
#define GLMINECRAFT3_ENTITY_RENDER_H

#include "entity_render.h"
#include "entity/entity.h"
#include "textured_cube_render.h"

namespace client {

    void render_player(glm::mat4 p, glm::mat4 v, const std::shared_ptr<nbt::nbt>& data, textured_cube_renderer *tcr,
                       gl::texture *tex);

}

#endif //GLMINECRAFT3_ENTITY_RENDER_H
