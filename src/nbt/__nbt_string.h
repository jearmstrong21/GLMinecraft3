//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_STRING_H
#define GLMINECRAFT3___NBT_STRING_H


#include "__nbt_utils.h"
#include "__nbt.h"
#include <istream>
#include <ostream>

namespace nbt {

    struct nbt_string:nbt{
        std::string value;
        nbt_string();
        explicit nbt_string(std::istream&in,bool read_type);
        explicit nbt_string(std::string value);
        ~nbt_string()override;
        void write(std::ostream&out)const override;
        void read(std::istream&in,bool read_type)override;
        nbt_type type()const override;
        std::string to_str(std::string indent)const override;
    };

}

#endif //GLMINECRAFT3___NBT_STRING_H
