//
// Created by Jack Armstrong on 2/1/20.
//

#ifndef GLMINECRAFT3_ITEM_NONE_H
#define GLMINECRAFT3_ITEM_NONE_H

#include "item.h"

namespace item {

    struct item_none : item {

        explicit item_none(item_properties properties);

        client::item_texture_descr render(item_stack stack) override;

        item_stack make() override;

    };

}

#endif //GLMINECRAFT3_ITEM_NONE_H
