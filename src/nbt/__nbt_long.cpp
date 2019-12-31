//
// Created by Jack Armstrong on 11/23/19.
//

#include "__nbt_long.h"

namespace nbt {

    std::string nbt_long::to_str(std::string indent) const {
        return std::to_string(value) + "L";
    }

    nbt_long::nbt_long() : value(0) {

    }

    nbt_long::nbt_long(std::istream &in, bool read_type) {
        read(in, read_type);
    }

    nbt_long::nbt_long(long value) : value(value) {

    }

    nbt_long::~nbt_long() = default;

    void nbt_long::write(std::ostream &out) const {
        bin_write<int>(out, nbt_type_long);
        bin_write<long>(out, value);
    }

    void nbt_long::read(std::istream &in, bool read_type) {
        if (read_type)assert_next_int(in, nbt_type_long);
        bin_read<long>(in, &value);
    }

    nbt_type nbt_long::type() const {
        return nbt_type_long;
    }

}