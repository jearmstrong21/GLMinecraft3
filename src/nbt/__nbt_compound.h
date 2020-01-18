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
#include <utility>
#include "__nbt_string.h"
#include "__nbt_int.h"
#include "__nbt_long.h"
#include "__nbt_float.h"
#include "__nbt_string.h"
#include "__nbt_list.h"

namespace nbt {

    nbt_ptr read_nbt(std::istream &in) noexcept;

    struct nbt_compound : nbt {
        nbt_compound(const nbt_compound &) = delete;

        std::map<std::string, nbt_ptr> value;

        nbt_compound();

        explicit nbt_compound(std::istream &in, bool read_type);

        explicit nbt_compound(std::map<std::string, nbt_ptr> value);

        ~nbt_compound() override;

        void write(std::ostream &out) const override;

        void read(std::istream &in, bool read_type) override;

        [[nodiscard]] nbt_type type() const override;

        [[nodiscard]] std::string to_str(std::string indent) const override;

        [[nodiscard]] std::map<std::string, nbt_ptr> as_compound() const override;

        [[nodiscard]] float as_float() const override;

        [[nodiscard]] int as_int() const override;

        [[nodiscard]] std::vector<nbt_ptr> as_list() const override;

        [[nodiscard]] long as_long() const override;

        [[nodiscard]] short as_short() const override;

        [[nodiscard]] std::string as_string() const override;

        [[nodiscard]] std::map<std::string, nbt_ptr> &compound_ref() override;

        [[nodiscard]] float &float_ref() override;

        [[nodiscard]] int &int_ref() override;

        [[nodiscard]] std::vector<nbt_ptr> &list_ref() override;

        [[nodiscard]] long &long_ref() override;

        [[nodiscard]] short &short_ref() override;

        [[nodiscard]] std::string &string_ref() override;

        static nbt_ptr make(const std::map<std::string, nbt_ptr> &value) {
            return std::shared_ptr<nbt>(new nbt_compound(value));
        }

    };

    typedef std::shared_ptr<nbt_compound> nbt_compound_ptr;

}
#endif //GLMINECRAFT3___NBT_COMPOUND_H
