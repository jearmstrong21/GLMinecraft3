//
// Created by Jack Armstrong on 2/1/20.
//

#ifndef GLMINECRAFT3_ITEM_ENUMS_H
#define GLMINECRAFT3_ITEM_ENUMS_H

#include "client/item_renderer.h"

namespace item::enums {

    struct material {

        client::atlas_texture sword_texture;
        std::string name;

        static material WOOD;
        static material STONE;
        static material IRON;
        static material GOLD;
        static material DIAMOND;

    };

}

#endif //GLMINECRAFT3_ITEM_ENUMS_H
