//
// Created by Jack Armstrong on 1/26/20.
//

#include "entity_player.h"

#include <utility>
#include <item/registry.h>

namespace entity {

    entity_player::entity_player(): entity(ENTITY_ID_PLAYER){
        box.size={0.6,1.8,0.6};
        inventory[0]=item::item_registry::DIAMOND_SWORD->make();
    }

    void entity_player::save_additional_information(const nbt::nbt_compound_ptr &tag) {
//        for(auto & i : inventory){
//            nbt::nbt_compound_ptr ptr=nbt::cast_compound(nbt::nbt_compound::make({}));
//            i.item->save(i,ptr);
//            tag->value["inventory"]->list_ref().push_back(ptr);
//        }
        tag->value["inventory"]=nbt::nbt_list::make({});
        for(int i=0;i<inventory_size;i++){
            nbt::nbt_compound_ptr ptr=nbt::cast_compound(nbt::nbt_compound::make({}));
            item::item_registry::map[inventory[i].item_type_id]->save(inventory[i],ptr);
            tag->value["inventory"]->list_ref().push_back(ptr);
        }
        std::cout<<"TAG: "<<tag->to_str("")<<"\n";
    }

    void entity_player::load_additional_information(const nbt::nbt_compound_ptr &tag) {
        for(int i=0;i<inventory_size;i++){
            item::item_registry::map[inventory[i].item_type_id]->load(&inventory[i],nbt::cast_compound(tag->value["inventory"]->list_ref()[i]));
        }
    }

    void entity_player::handle_ai() {

    }

    entity_ptr entity_player::spawn(std::string id, glm::vec3 pos, server::game_room*server) {
        std::shared_ptr<entity_player>p=std::make_shared<entity_player>();
        p->uuid=std::move(id);
        p->box.pos=pos;
        p->server=server;
        return std::dynamic_pointer_cast<entity>(p);
    }

}