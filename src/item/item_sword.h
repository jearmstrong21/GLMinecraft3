//
// Created by Jack Armstrong on 1/31/20.
//

#ifndef GLMINECRAFT3_ITEM_SWORD_H
#define GLMINECRAFT3_ITEM_SWORD_H

#include "item.h"

namespace item {

    struct item_sword : item {

        enums::material material;

        item_sword(item_properties properties, enums::material material);

        client::item_texture_descr render(item_stack stack) override;

        item_stack make() override;

    };

}

#endif //GLMINECRAFT3_ITEM_SWORD_H
