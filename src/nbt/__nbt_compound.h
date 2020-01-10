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

    std::shared_ptr<nbt> read_nbt(std::istream &in) noexcept;

    struct nbt_compound : nbt {
        nbt_compound(const nbt_compound &) = delete;

        std::map<std::string, std::shared_ptr<nbt>> value;

        nbt_compound();

        explicit nbt_compound(std::istream &in, bool read_type);

        explicit nbt_compound(std::map<std::string, std::shared_ptr<nbt>> value);

        ~nbt_compound() override;

        void write(std::ostream &out) const override;

        void read(std::istream &in, bool read_type) override;

        [[nodiscard]] nbt_type type() const override;

        [[nodiscard]] std::string to_str(std::string indent) const override;

        [[nodiscard]] std::map<std::string,std::shared_ptr<nbt> >as_compound()const override;
        [[nodiscard]] float as_float()const override ;
        [[nodiscard]] int as_int()const override;
        [[nodiscard]] std::vector<std::shared_ptr<nbt>>as_list()const override;
        [[nodiscard]] long as_long()const override;
        [[nodiscard]] short as_short()const override;
        [[nodiscard]] std::string as_string()const override;

        [[nodiscard]] const std::map<std::string,std::shared_ptr<nbt>>&compound_ref()const override;
        [[nodiscard]] const float&float_ref()const override;
        [[nodiscard]] const int&int_ref()const override;
        [[nodiscard]] const std::vector<std::shared_ptr<nbt>>list_ref()const override;
        [[nodiscard]] const long&long_ref()const override;
        [[nodiscard]] const short&short_ref()const override;
        [[nodiscard]] const std::string&string_ref()const override;

    };

}
#endif //GLMINECRAFT3___NBT_COMPOUND_H
