//
// Created by paladin on 1/17/20.
//

#ifndef GLMINECRAFT3_ITEM_H
#define GLMINECRAFT3_ITEM_H


#include <nbt/__nbt_compound.h>

namespace item {
    struct item;

    struct item_stack {
        int count;
        nbt::nbt_compound tag;

        bool empty() {
            return !count;
        }
    };

    typedef std::shared_ptr<item_stack> item_stack_ptr;

    struct item {
        virtual void attack(item_stack_ptr stack) = 0;

        virtual void use(item_stack_ptr stack) = 0;
        //TODO: item should implement default attack / use

        //TODO: save/load name, count, enchants
        //TODO: save/load should call save_additional_information / load_additional_information
        void save(item_stack_ptr stack, const nbt::nbt_compound_ptr &tag);

        virtual void save_additional_information(item_stack_ptr stack, const nbt::nbt_compound_ptr &tag) = 0;

        void load(item_stack_ptr stack, const nbt::nbt_compound_ptr &tag);

        virtual void load_additional_information(item_stack_ptr stack, const nbt::nbt_compound_ptr &tag) = 0;

        virtual item_stack make(int count) = 0;
    };
}


#endif //GLMINECRAFT3_ITEM_H
