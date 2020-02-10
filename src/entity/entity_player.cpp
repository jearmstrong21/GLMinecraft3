//
// Created by Jack Armstrong on 1/26/20.
//

#include "entity_player.h"

#include <utility>
#include <item/registry.h>
#include <server/game_room.h>

namespace entity {

    entity_player::entity_player() : entity(ENTITY_ID_PLAYER) {
        box.size = {0.6, 1.8, 0.6};
        leftclick = false;
        rightclick = false;
        intersection.res = false;
    }

    void entity_player::save_additional_information(const nbt::nbt_compound_ptr &tag) {
        tag->value["inventory"] = nbt::nbt_list::make({});
        for (int i = 0; i < inventory_size; i++) {
            nbt::nbt_compound_ptr ptr = nbt::cast_compound(nbt::nbt_compound::make({}));
            item::item_registry::map[inventory[i].item_type_id]->save(inventory[i], ptr);
            tag->value["inventory"]->list_ref().push_back(ptr);
        }
        tag->value["selected_item"] = nbt::nbt_int::make(selected_item);
        tag->value["leftclick"] = nbt::nbt_short::make(leftclick);
        tag->value["rightclick"] = nbt::nbt_short::make(rightclick);
    }

    void entity_player::load_additional_information(const nbt::nbt_compound_ptr &tag) {
        for (int i = 0; i < inventory_size; i++) {
            item::item_registry::map[inventory[i].item_type_id]->load(&inventory[i], nbt::cast_compound(
                    tag->value["inventory"]->list_ref()[i]));
        }
        selected_item = tag->value["selected_item"]->as_int();
        leftclick = tag->value["leftclick"]->as_short();
        rightclick = tag->value["rightclick"]->as_short();
    }

    void entity_player::handle_ai() {
        int range = 20;
        glm::vec3 start = box.pos + glm::vec3{0, eye_height, 0};
        glm::vec3 end = start + lookdir * (float) range;
        const float x1 = start.x;
        const float y1 = start.y;
        const float z1 = start.z;
        const float x2 = end.x;
        const float y2 = end.y;
        const float z2 = end.z;
        int i = floor(x1);
        int j = floor(y1);
        int k = floor(z1);
        const int di = x2 < x1 ? -1 : (x2 == x1 ? 0 : 1);
        const int dj = y2 < y1 ? -1 : (y2 == y1 ? 0 : 1);
        const int dk = z2 < z1 ? -1 : (z2 == z1 ? 0 : 1);
        const float dx = 1 / abs(x2 - x1);
        const float dy = 1 / abs(y2 - y1);
        const float dz = 1 / abs(z2 - z1);
        const float minx = floor(x1), maxx = minx + 1;
        const float miny = floor(y1), maxy = miny + 1;
        const float minz = floor(z1), maxz = minz + 1;
        float tx = ((x1 > x2) ? (x1 - minx) : (maxx - x1)) * dx;
        float ty = ((y1 > y2) ? (y1 - miny) : (maxy - y1)) * dy;
        float tz = ((z1 > z2) ? (z1 - minz) : (maxz - z1)) * dz;
        std::vector<glm::ivec3> list;
        for (int ___ = 0; ___ < range; ___++) {
            list.emplace_back(i, j, k);
//            std::cout<<___<<" "<<i<<" "<<j<<" "<<k<<" "<<server::game_room::instance->world.get(i,j,k)<<"\n";
            if (tx <= ty && tx <= tz) {
                tx += dx;
                i += di;
            } else if (ty <= tz) {
                ty += dy;
                j += dj;
            } else {
                tz += dz;
                k += dk;
            }
        }
        intersection.res = false;
        for (int ind = 1; ind < range; ind++) {
            block::block_state bs = server::game_room::instance->world.get(list[ind]);
            if (bs != 0) {
                intersection.res = true;
                intersection.hit = list[ind];
                intersection.prev = list[ind - 1];
                intersection.normal = list[ind - 1] - list[ind];
                break;
            }
        }
    }

    entity_ptr entity_player::spawn(std::string id, glm::vec3 pos, server::game_room *server) {
        std::shared_ptr<entity_player> p = std::make_shared<entity_player>();
        p->uuid = std::move(id);
        p->box.pos = pos;
        p->server = server;
        p->inventory[0] = item::item_registry::DIAMOND_SWORD->make();
        p->inventory[1] = item::item_registry::GOLD_SWORD->make();
        p->inventory[8] = item::item_registry::IRON_SWORD->make();
        p->inventory[4] = item::item_registry::STONE_SWORD->make();
        p->inventory[7] = item::item_registry::WOOD_SWORD->make();
        p->selected_item = 8;
        return std::dynamic_pointer_cast<entity>(p);
    }

    void entity_player::leftclick_start() {
        leftclick = true;
        if (intersection.res) {
            server::game_room::instance->world_ops.push({true, intersection.hit, 0});
        }
    }

    void entity_player::leftclick_continue() {
        ASSERT_OR_EXIT(leftclick == true, "leftclick_continue() called with leftclick=false")
    }

    void entity_player::leftclick_end() {
        leftclick = false;
    }

    void entity_player::rightclick_start() {
        rightclick = true;
        if (intersection.res) {
            server::game_room::instance->world_ops.push({true, intersection.prev, block::BRICKS.defaultState});
        }
    }

    void entity_player::rightclick_continue() {
        ASSERT_OR_EXIT(rightclick == true, "rightclick_continue() called with rightclick=false")
    }

    void entity_player::rightclick_end() {
        rightclick = false;
    }

}