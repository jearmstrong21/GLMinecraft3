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

    void item::save(const item_stack &stack, const nbt::nbt_compound_ptr &tag) {
        tag->value["count"] = nbt::nbt_int::make(stack.count);
        tag->value["item_type_id"] = nbt::nbt_int::make(stack.item_type_id);
    }

    void item::load(item_stack *stack, const nbt::nbt_compound_ptr &tag) {
        stack->count = tag->value["count"]->as_int();
        stack->item_type_id = tag->value["item_type_id"]->as_int();
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

}