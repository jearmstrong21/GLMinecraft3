//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_LONG_H
#define GLMINECRAFT3___NBT_LONG_H


#include "__nbt.h"
#include "__nbt_utils.h"
#include <istream>
#include <ostream>

namespace nbt {

    struct nbt_long:nbt{
        long value;
        nbt_long();
        explicit nbt_long(std::istream&in,bool read_type);
        explicit nbt_long(long value);
        ~nbt_long()override;
        void write(std::ostream&out)const override;
        void read(std::istream&in,bool read_type)override;
        [[nodiscard]] nbt_type type()const override;
        [[nodiscard]] std::string to_str(std::string indent)const override;
    };

}
#endif //GLMINECRAFT3___NBT_LONG_H
