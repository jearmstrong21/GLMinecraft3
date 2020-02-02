//
// Created by Jack Armstrong on 1/31/20.
//

#ifndef GLMINECRAFT3_REGISTRY_H
#define GLMINECRAFT3_REGISTRY_H

#include "item.h"
#include "item_sword.h"

namespace item {

    struct item_registry {

        static std::shared_ptr<item> DIAMOND_SWORD;

        static std::map<int, std::shared_ptr<item>> map;

        static void initialize();

    };

}

#endif //GLMINECRAFT3_REGISTRY_H
