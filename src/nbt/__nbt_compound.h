//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_COMPOUND_H
#define GLMINECRAFT3___NBT_COMPOUND_H

#include "__nbt_utils.h"
#include "__nbt.h"
#include <istream>
#include <ostream>
#include <map>
#include "__nbt_string.h"
#include "__nbt_int.h"
#include "__nbt_long.h"
#include "__nbt_float.h"
#include "__nbt_string.h"
#include "__nbt_list.h"

namespace nbt {

    std::shared_ptr<nbt> read_nbt(std::istream&in);

    struct nbt_compound:nbt{
        nbt_compound(const nbt_compound&)=delete;
        std::map<std::string,std::shared_ptr<nbt>>value;
        nbt_compound();
        explicit nbt_compound(std::istream&in,bool read_type);
        explicit nbt_compound(std::map<std::string,std::shared_ptr<nbt>> value);
        ~nbt_compound()override;
        void write(std::ostream&out)const override;
        void read(std::istream&in,bool read_type)override;
        nbt_type type()const override;
        std::string to_str(std::string indent)const override;
    };

}
#endif //GLMINECRAFT3___NBT_COMPOUND_H
