//
// Created by paladin on 1/17/20.
//

#ifndef GLMINECRAFT3_ITEM_H
#define GLMINECRAFT3_ITEM_H


#include <nbt/__nbt_compound.h>

namespace item {
    class item;

    class item_stack {
        item* base;
        int count;
        nbt::nbt_compound tag;

        bool empty() {
            return !count;
        }
    };

    class item {
        void attack(item_stack stack);

        void use(item_stack stack);

        void save(item_stack stack, nbt::nbt_compound tag);

        item_stack load(nbt::nbt_compound tag);

        item_stack make(int count);
    };
}


#endif //GLMINECRAFT3_ITEM_H
