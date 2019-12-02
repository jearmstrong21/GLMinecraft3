//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_INT_H
#define GLMINECRAFT3___NBT_INT_H

#include "__nbt_utils.h"
#include "__nbt.h"
#include <istream>
#include <ostream>

namespace nbt{

    struct nbt_int:nbt{
        int value;
        nbt_int();
        explicit nbt_int(std::istream&in,bool read_type);
        explicit nbt_int(int value);
        ~nbt_int()override;
        void write(std::ostream&out)const override;
        void read(std::istream&in,bool read_type)override;
        nbt_type type()const override;
        std::string to_str(std::string indent)const override;
    };

//    std::shared_ptr<nbt::nbt>(new nbt::nbt_int(321));
//    nbt::make_int(321)

}

#endif //GLMINECRAFT3___NBT_INT_H
