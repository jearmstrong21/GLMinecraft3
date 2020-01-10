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

        [[nodiscard]] virtual std::map<std::string,std::shared_ptr<nbt> >as_compound()const;
        [[nodiscard]] virtual float as_float()const;
        [[nodiscard]] virtual int as_int()const;
        [[nodiscard]] virtual std::vector<std::shared_ptr<nbt>>as_list()const;
        [[nodiscard]] virtual long as_long()const;
        [[nodiscard]] virtual short as_short()const;
        [[nodiscard]] virtual std::string as_string()const;

        virtual [[nodiscard]] const virtual std::map<std::string,std::shared_ptr<nbt>>&compound_ref()const;
        [[nodiscard]] const virtual float&float_ref()const;
        [[nodiscard]] const virtual int&int_ref()const;
        [[nodiscard]] const virtual std::vector<std::shared_ptr<nbt>>list_ref()const;
        [[nodiscard]] const virtual long&long_ref()const;
        [[nodiscard]] const virtual short&short_ref()const;
        [[nodiscard]] const virtual std::string&string_ref()const;

    };

}

#endif //GLMINECRAFT3___NBT_H
