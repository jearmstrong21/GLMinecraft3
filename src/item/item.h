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

    enum class item_attribute_modifier_operation {
        add,
        multiply
    };

    struct item_attribute_modifier {

        static std::map<std::string, float> BASE;
        static item_attribute_modifier NONE;

        std::string attribute_name;
        std::string modifier_name;
        //TODO item_slot_type slot;
        item_attribute_modifier_operation op;
        float value;

        item_attribute_modifier();

        explicit item_attribute_modifier(const nbt::nbt_compound_ptr &tag);

        void save(const nbt::nbt_compound_ptr &tag);

        void load(const nbt::nbt_compound_ptr &tag);
    };

    struct item_stack {
        int item_type_id = 0;
        int count = 0;
        std::vector<item_attribute_modifier> modifiers;
        nbt::nbt_compound tag;

        float get_value(const std::string &attribute_name);

        bool has_modifier(const std::string &modifier_name);

        item_attribute_modifier &get_modifier(const std::string &modifier_name);

        void remove_modifier(const std::string &modifier_name);

        item_attribute_modifier &add_modifier(const item_attribute_modifier &modifier);

        item_stack();

        item_stack(const item_stack &other);

        ~item_stack();

        explicit item_stack(const nbt::nbt_compound_ptr &load_from);

        void save(const nbt::nbt_compound_ptr &tag);

        void load(const nbt::nbt_compound_ptr &tag);

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

        item_stack make(int count);

        virtual item_stack make() = 0;

    };

}


#endif //GLMINECRAFT3_ITEM_H
