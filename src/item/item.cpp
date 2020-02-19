//
// Created by Jack Armstrong on 1/27/20.
//

#include "item.h"

#include <utility>
#include "registry.h"
#include "entity/entity_player.h"
#include "server/game_room.h"

namespace item {

    item::item(item_properties properties) : properties(std::move(properties)) {

    }

    void item::attack(const item_use_context &ctx) {
        if (ctx.source->type_id == ENTITY_ID_PLAYER) {
            auto player = (entity::entity_player *) ctx.source;
            server::game_room::instance->world_ops.push({true, player->intersection.hit, 0});
        }
    }

    void item::use(const item_use_context &ctx) {

    }

    item *item_stack::item() {
        return item_registry::map[item_type_id];
    }

    bool item_stack::is_empty() {
        return item_type_id == 0 || count <= 0;
    }

    item_stack item::make(int count) {
        item_stack stack = make();
        stack.count = count;
        return stack;
    }

    std::map<std::string, float>item_attribute_modifier::BASE;
    item_attribute_modifier item_attribute_modifier::NONE;

    item_attribute_modifier::item_attribute_modifier() = default;

    item_attribute_modifier::item_attribute_modifier(const nbt::nbt_compound_ptr &tag) {
        load(tag);
    }

    item_stack::~item_stack() = default;

    void item_attribute_modifier::save(const nbt::nbt_compound_ptr &tag) {
        tag->value["attribute_name"] = nbt::nbt_string::make(attribute_name);
        tag->value["modifier_name"] = nbt::nbt_string::make(modifier_name);
        tag->value["op"] = nbt::nbt_short::make((short) op);
        tag->value["value"] = nbt::nbt_float::make(value);
    }

    void item_attribute_modifier::load(const nbt::nbt_compound_ptr &tag) {
        attribute_name = tag->value["attribute_name"]->as_string();
        modifier_name = tag->value["modifier_name"]->as_string();
        op = (item_attribute_modifier_operation) tag->value["op"]->as_short();
        value = tag->value["value"]->as_float();
    }

    float item_stack::get_value(const std::string &attribute_name) {
        float value = item_attribute_modifier::BASE[attribute_name];
        for (auto &p:modifiers) {
            if (p.attribute_name == attribute_name) {
                switch (p.op) {
                    case item_attribute_modifier_operation::add:
                        value += p.value;
                        break;
                    case item_attribute_modifier_operation::multiply:
                        value *= p.value;
                        break;
                }
            }
        }
        return value;
    }

    item_attribute_modifier &item_stack::get_modifier(const std::string &modifier_name) {
        for (auto &p:modifiers) {
            if (p.modifier_name == modifier_name)return p;
        }
        return item_attribute_modifier::NONE;
    }

    bool item_stack::has_modifier(const std::string &modifier_name) {
        for (auto &p:modifiers) {
            if (p.modifier_name == modifier_name)return true;
        }
        return false;
    }

    void item_stack::remove_modifier(const std::string &modifier_name) {
        for (int i = 0; i < modifiers.size(); i++) {
            if (modifiers[i].modifier_name == modifier_name) {
                modifiers.erase(modifiers.begin() + i);
            }
        }
    }

    item_attribute_modifier &item_stack::add_modifier(const item_attribute_modifier &modifier) {
        modifiers.push_back(modifier);
        return modifiers[modifiers.size() - 1];
    }

    item_stack::item_stack(const nbt::nbt_compound_ptr &load_from) {
        load(load_from);
    }

    void item_stack::save(const nbt::nbt_compound_ptr &save_to) {
        save_to->value["item_type_id"] = nbt::nbt_int::make(item_type_id);
        save_to->value["count"] = nbt::nbt_int::make(count);
        save_to->value["modifiers"] = nbt::nbt_list::make({});
        for (auto &p:modifiers) {
            nbt::nbt_compound_ptr cmpnd = std::make_shared<nbt::nbt_compound>();
            p.save(cmpnd);
            save_to->value["modifiers"]->list_ref().push_back(cmpnd);
        }
        save_to->value["tag"] = std::make_shared<nbt::nbt_compound>(tag.value);
    }

    void item_stack::load(const nbt::nbt_compound_ptr &load_from) {
        item_type_id = load_from->value["item_type_id"]->as_int();
        count = load_from->value["count"]->as_int();
        modifiers.clear();
        for (auto &p:load_from->value["modifiers"]->list_ref()) {
            modifiers.emplace_back(nbt::cast_compound(p));
        }
        tag = *nbt::cast_compound(load_from->value["tag"]);
        //TODO delete item::load/save and use item_stack::load/save
    }

    item_stack::item_stack() = default;

    item_stack::item_stack(const item_stack &other) {
        item_type_id = other.item_type_id;
        count = other.count;
        modifiers = other.modifiers;
        tag = other.tag;
    }

}