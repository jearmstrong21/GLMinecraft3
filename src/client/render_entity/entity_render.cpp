//
// Created by Jack Armstrong on 12/31/19.
//

#include <item/registry.h>
#include "entity_render.h"

extern "C" const unsigned char TEXTURE_entity_steve[];
extern "C" const size_t TEXTURE_entity_steve_len;

extern "C" const unsigned char TEXTURE_entity_zombie[];
extern "C" const size_t TEXTURE_entity_zombie_len;

extern "C" const unsigned char SHADER_item3d_frag[];
extern "C" const size_t SHADER_item3d_frag_len;

extern "C" const unsigned char SHADER_item3d_vert[];
extern "C" const size_t SHADER_item3d_vert_len;

extern "C" const unsigned char TEXTURE_pig_png[];
extern "C" const size_t TEXTURE_pig_png_len;

namespace client {

    entity_render::entity_render() {
        tcr = new textured_cube_renderer();
        steve_texture = new gl::texture(TEXTURE_entity_steve, TEXTURE_entity_steve_len);
        zombie_texture = new gl::texture(TEXTURE_entity_zombie, TEXTURE_entity_zombie_len);
        pig_texture=new gl::texture(TEXTURE_pig_png,TEXTURE_pig_png_len);
        item_shader = new gl::shader(SHADER_item3d_vert, SHADER_item3d_vert_len, SHADER_item3d_frag,
                                     SHADER_item3d_frag_len);
        {
            gl::mesh_data data{
                    {
                            {3, {0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0}}
                    },
                    {       0, 1, 2, 1, 2, 3}
            };
            item_mesh = new gl::mesh(&data);
        }
    }

    entity_render::~entity_render() {
        delete tcr;
        delete steve_texture;
        delete zombie_texture;
        delete pig_texture;
        delete item_shader;
        delete item_mesh;
    }

    void
    entity_render::render_player(glm::mat4 p, glm::mat4 v, const entity::entity_ptr &data, gl::texture *item_texture,
                                 item_renderer *item_rend) {
        std::shared_ptr<entity::entity_player> player = std::dynamic_pointer_cast<entity::entity_player>(data);
        glm::mat4 m0(1);
        m0 *= glm::translate(glm::mat4(1), data->box.pos);

        glm::mat4 m = m0 *
                      glm::lookAt(glm::vec3{0, 0, 0}, glm::normalize(glm::vec3{data->facedir.x, 0, -data->facedir.z}),
                                  glm::vec3{0, 1, 0});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 12, 0} / 16.0F), steve_texture,//arm
                         glm::vec3{8, 12, 4},
                         glm::vec2{16, 32}, glm::vec2{64, 64});

        item_shader->bind();
        item_shader->uniform4x4("perspective", p);
        item_shader->uniform4x4("view", v);
        item_shader->uniform4x4("model", m * glm::rotate(glm::mat4(1), glm::radians(-90.0F), glm::vec3{0, 1, 0}) *
                                         glm::translate(glm::mat4(1),
                                                        glm::vec3{-2 / 16.0F, 10.0F / 16.0F, -6 / 16.0F}));
        item_shader->texture("tex", item_texture, 0);
        item_shader->uniform1("scale", 0.9);
        if (!player->inventory[player->selected_item].is_empty()) {
            item_rend->render_item(
                    player->inventory[player->selected_item].item()->render(player->inventory[player->selected_item]),
                    item_shader, [&]() {
                        item_mesh->render_triangles();
                    });
        }

        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{6, 12, 0} / 16.0F), steve_texture,//body
                         glm::vec3{4, 12, 4},
                         glm::vec2{40, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-6, 12, 0} / 16.0F), steve_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{32, 0}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{2, 0, 0} / 16.0F), steve_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{0, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-2, 0, 0} / 16.0F), steve_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{16, 0}, glm::vec2{64, 64});

        if (!player->firstperson) {
            m = m0 * glm::lookAt(glm::vec3{0, 0, 0}, glm::normalize(glm::vec3{data->lookdir.x, 0, -data->lookdir.z}),
                                 glm::vec3{0, 1, 0});
            tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 24, 0} / 16.0F), steve_texture,
                             glm::vec3{8, 8, 8},
                             glm::vec2{0, 48}, glm::vec2{64, 64});
        }
    }

    void entity_render::render_zombie(glm::mat4 p, glm::mat4 v, const entity::entity_ptr &data) {
        glm::mat4 m0(1);
        m0 *= glm::translate(glm::mat4(1), data->box.pos);

        glm::mat4 m = m0 *
                      glm::lookAt(glm::vec3{0, 0, 0}, glm::normalize(glm::vec3{data->facedir.x, 0, -data->facedir.z}),
                                  glm::vec3{0, 1, 0});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 12, 0} / 16.0F), zombie_texture,
                         glm::vec3{8, 12, 4},
                         glm::vec2{16, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{6, 12, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{40, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-6, 12, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{40, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{2, 0, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{0, 32}, glm::vec2{64, 64});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{-2, 0, 0} / 16.0F), zombie_texture,
                         glm::vec3{4, 12, 4},
                         glm::vec2{0, 32}, glm::vec2{64, 64});

        m = m0 * glm::lookAt(glm::vec3{0, 0, 0}, glm::normalize(glm::vec3{data->lookdir.x, 0, -data->lookdir.z}),
                             glm::vec3{0, 1, 0});
        tcr->render_cube(p, v, m * glm::translate(glm::mat4(1), glm::vec3{0, 24, 0} / 16.0F), zombie_texture,
                         glm::vec3{8, 8, 8},
                         glm::vec2{0, 48}, glm::vec2{64, 64});
    }

    void entity_render::render_pig(glm::mat4 p, glm::mat4 v, const entity::entity_ptr &data) {
        glm::mat4 m0(1);
        m0 *= glm::translate(glm::mat4(1), data->box.pos);

        glm::mat4 m = m0 *
                      glm::lookAt(glm::vec3{0, 0, 0}, glm::normalize(glm::vec3{data->facedir.x, 0, -data->facedir.z}),
                                  glm::vec3{0, 1, 0});
        tcr->render_cube(p,v,m*glm::translate(glm::mat4(1),glm::vec3{0,10,-8}/16.0F)*glm::rotate(glm::mat4(1),glm::radians(90.0F),glm::vec3{1,0,0}),pig_texture,glm::vec3{10,16,8},glm::vec2{28,0},glm::vec2{64,32});//body



        m-m0 *
          glm::lookAt(glm::vec3{0, 0, 0}, glm::normalize(glm::vec3{data->lookdir.x, 0, -data->lookdir.z}),
                      glm::vec3{0, 1, 0});
        tcr->render_cube(p,v,m*glm::translate(glm::mat4(1),glm::vec3{0,10,8}/16.0F),pig_texture,glm::vec3{8,8,8},glm::vec2{0,16 },glm::vec2{64,32});//head
    }

}