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

    std::shared_ptr<nbt> read_nbt(std::istream &in) noexcept;

    struct nbt_list : nbt {
        std::vector<std::shared_ptr<nbt>> value;

        nbt_list();

        nbt_list(std::istream &in, bool read_type);

        explicit nbt_list(std::vector<std::shared_ptr<nbt>> value);

        ~nbt_list() override;

        void write(std::ostream &out) const override;

        void read(std::istream &in, bool read_type) override;

        [[nodiscard]] nbt_type type() const override;

        [[nodiscard]] std::string to_str(std::string indent) const override;

        [[nodiscard]] std::map<std::string, std::shared_ptr<nbt> > as_compound() const override;

        [[nodiscard]] float as_float() const override;

        [[nodiscard]] int as_int() const override;

        [[nodiscard]] std::vector<std::shared_ptr<nbt>> as_list() const override;

        [[nodiscard]] long as_long() const override;

        [[nodiscard]] short as_short() const override;

        [[nodiscard]] std::string as_string() const override;

        [[nodiscard]] std::map<std::string, std::shared_ptr<nbt>> &compound_ref() override;

        [[nodiscard]] float &float_ref() override;

        [[nodiscard]] int &int_ref() override;

        [[nodiscard]] std::vector<std::shared_ptr<nbt>> &list_ref() override;

        [[nodiscard]] long &long_ref() override;

        [[nodiscard]] short &short_ref() override;

        [[nodiscard]] std::string &string_ref() override;
    };

}

#endif //GLMINECRAFT3___NBT_LIST_H
