//
// Created by Jack Armstrong on 1/26/20.
//

#include "entity_player.h"

#include <utility>
#include <item/registry.h>

namespace entity {

    entity_player::entity_player() : entity(ENTITY_ID_PLAYER) {
        box.size = {0.6, 1.8, 0.6};
        leftclick=false;
        rightclick=false;
    }

    void entity_player::save_additional_information(const nbt::nbt_compound_ptr &tag) {
        tag->value["inventory"] = nbt::nbt_list::make({});
        for (int i = 0; i < inventory_size; i++) {
            nbt::nbt_compound_ptr ptr = nbt::cast_compound(nbt::nbt_compound::make({}));
            item::item_registry::map[inventory[i].item_type_id]->save(inventory[i], ptr);
            tag->value["inventory"]->list_ref().push_back(ptr);
        }
        tag->value["selected_item"] = nbt::nbt_int::make(selected_item);
        tag->value["leftclick"]=nbt::nbt_short::make(leftclick);
        tag->value["rightclick"]=nbt::nbt_short::make(rightclick);
    }

    void entity_player::load_additional_information(const nbt::nbt_compound_ptr &tag) {
        for (int i = 0; i < inventory_size; i++) {
            item::item_registry::map[inventory[i].item_type_id]->load(&inventory[i], nbt::cast_compound(
                    tag->value["inventory"]->list_ref()[i]));
        }
        selected_item = tag->value["selected_item"]->as_int();
        leftclick=tag->value["leftclick"]->as_short();
        rightclick=tag->value["rightclick"]->as_short();
    }

    void entity_player::handle_ai() {

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
        leftclick=true;
    }

    void entity_player::leftclick_continue(){
        ASSERT_OR_EXIT(leftclick==true,"leftclick_continue() called with leftclick=false")
    }

    void entity_player::leftclick_end() {
        leftclick=false;
    }

    void entity_player::rightclick_start() {
        rightclick=true;
    }

    void entity_player::rightclick_continue(){
        ASSERT_OR_EXIT(rightclick==true,"rightclick_continue() called with rightclick=false")
    }

    void entity_player::rightclick_end() {
        rightclick=false;
    }

}