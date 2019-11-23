//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_H
#define GLMINECRAFT3___NBT_H

#include "__nbt_utils.h"
#include <istream>
#include <ostream>

namespace nbt {

    struct nbt {
        nbt() = default;

        virtual ~nbt() = default;

        virtual void write(std::ostream &out) const = 0;

        virtual void read(std::istream &in, bool read_type) = 0;

        virtual nbt_type type() const = 0;

        virtual std::string to_str(std::string indent) const = 0;
    };

}

#endif //GLMINECRAFT3___NBT_H
