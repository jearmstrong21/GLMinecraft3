//
// Created by Jack Armstrong on 1/31/20.
//

#ifndef GLMINECRAFT3_ITEM_SWORD_H
#define GLMINECRAFT3_ITEM_SWORD_H

#include "item.h"

namespace item {

    struct item_sword: item {

        explicit item_sword(item_properties properties);

        void attack(const item_use_context&ctx)override;
        void use(const item_use_context&ctx)override;
        client::item_texture_descr render(item_stack stack)override;

        void save_additional_information(item_stack stack,const nbt::nbt_compound_ptr &tag) override;

        void load_additional_information(item_stack stack,const nbt::nbt_compound_ptr &tag) override;

        item_stack make()override;

    };

}

#endif //GLMINECRAFT3_ITEM_SWORD_H
