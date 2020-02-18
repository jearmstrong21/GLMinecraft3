//
// Created by paladin on 1/17/20.
//

#ifndef GLMINECRAFT3_ITEM_H
#define GLMINECRAFT3_ITEM_H


#include <nbt/nbt.h>
#include "client/item_renderer.h"
#include "item_enums.h"

namespace server { struct game_room; }
namespace entity { struct entity; }

namespace item {

    struct item;

    struct item_stack {
        int item_type_id = 0;
        int count = 0;

        item *item();

        bool is_empty();
    };

    struct item_use_context {

        item_stack *stack;
        entity::entity *source;

    };

    struct item_properties {

        int item_type_id;
        std::string item_type_name;

    };

    struct item {

        item_properties properties;

        explicit item(item_properties properties);//TODO: int uuid, item_props props, etc

        virtual void attack(const item_use_context &ctx);

        virtual void use(const item_use_context &ctx);

        virtual client::item_texture_descr render(item_stack stack) = 0;

        virtual void save_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) = 0;

        virtual void load_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) = 0;

        void save(const item_stack &stack, const nbt::nbt_compound_ptr &tag);

        void load(item_stack *stack, const nbt::nbt_compound_ptr &tag);

        item_stack make(int count);

        virtual item_stack make() = 0;

    };

}


#endif //GLMINECRAFT3_ITEM_H
