//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_LIST_H
#define GLMINECRAFT3___NBT_LIST_H

#include "__nbt_utils.h"
#include "__nbt.h"
#include <istream>
#include <ostream>
#include <vector>
#include <memory>

namespace nbt {

    std::shared_ptr<nbt> read_nbt(std::istream&in)noexcept;

    struct nbt_list:nbt{
        std::vector<std::shared_ptr<nbt>>value;
        nbt_list();
        nbt_list(std::istream&in,bool read_type);
        nbt_list(std::vector<std::shared_ptr<nbt>>value);
        ~nbt_list()override;
        void write(std::ostream&out)const override;
        void read(std::istream&in,bool read_type)override;
        nbt_type type()const override;
        std::string to_str(std::string indent)const override;
    };

}

#endif //GLMINECRAFT3___NBT_LIST_H
