//
// Created by Jack Armstrong on 11/23/19.
//

#ifndef GLMINECRAFT3___NBT_H
#define GLMINECRAFT3___NBT_H

#include "__nbt_utils.h"
#include <istream>
#include <ostream>
#include <map>

namespace nbt {

    struct nbt {
        nbt() = default;

        virtual ~nbt() = default;

        virtual void write(std::ostream &out) const = 0;

        virtual void read(std::istream &in, bool read_type) = 0;

        [[nodiscard]] virtual nbt_type type() const = 0;

        [[nodiscard]] virtual std::string to_str(std::string indent) const = 0;

        [[nodiscard]] virtual std::map<std::string, std::shared_ptr<nbt> > as_compound() const = 0;

        [[nodiscard]] virtual float as_float() const = 0;

        [[nodiscard]] virtual int as_int() const = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<nbt>> as_list() const = 0;

        [[nodiscard]] virtual long as_long() const = 0;

        [[nodiscard]] virtual short as_short() const = 0;

        [[nodiscard]] virtual std::string as_string() const = 0;

        [[nodiscard]] virtual std::map<std::string, std::shared_ptr<nbt>> &compound_ref() = 0;

        [[nodiscard]] virtual float &float_ref() = 0;

        [[nodiscard]] virtual int &int_ref() = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<nbt>> &list_ref() = 0;

        [[nodiscard]] virtual long &long_ref() = 0;

        [[nodiscard]] virtual short &short_ref() = 0;

        [[nodiscard]] virtual std::string &string_ref() = 0;

    };

}

#endif //GLMINECRAFT3___NBT_H
