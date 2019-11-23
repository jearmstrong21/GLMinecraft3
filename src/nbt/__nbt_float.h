//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_FLOAT_H
#define GLMINECRAFT3___NBT_FLOAT_H

#include "__nbt.h"
#include "__nbt_utils.h"
#include <istream>
#include <ostream>

namespace nbt {

    struct nbt_float:nbt{
        float value;
        nbt_float();
        explicit nbt_float(std::istream&in,bool read_type);
        explicit nbt_float(float value);
        ~nbt_float()override;
        void write(std::ostream&out)const override;
        void read(std::istream&in,bool read_type)override;
        nbt_type type()const override;
        std::string to_str(std::string indent)const override;
    };

}

#endif //GLMINECRAFT3___NBT_FLOAT_H
