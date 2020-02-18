//
// Created by Jack Armstrong on 2/16/20.
//

#ifndef GLMINECRAFT3_ITEM_BLOCK_H
#define GLMINECRAFT3_ITEM_BLOCK_H

#include "item.h"
#include "block/blockstate.h"

namespace item {

    struct item_block : item {

        block::block_state state;

        explicit item_block(item_properties properties, block::block_state state);

        void use(const item_use_context &ctx) override;

        client::item_texture_descr render(item_stack stack) override;

        virtual client::item_texture_descr render_no_count(item_stack stack) = 0;

        void save_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) override;

        void load_additional_information(item_stack stack, const nbt::nbt_compound_ptr &tag) override;

        item_stack make() override;

    };

    struct item_block_default : item_block {
        client::item_texture_descr tex;

        explicit item_block_default(item_properties properties, block::block_state state,
                                    client::item_texture_descr tex);

        client::item_texture_descr render_no_count(item_stack stack) override;

    };

}

#endif //GLMINECRAFT3_ITEM_BLOCK_H
