//
// Created by Jack Armstrong on 11/23/19.
//

#include "__nbt_int.h"

namespace nbt {

    nbt_int::nbt_int() : value(0) {

    }

    nbt_int::nbt_int(std::istream &in, bool read_type) : value(0) {
        read(in, read_type);
    }

    nbt_int::nbt_int(int value) {
        this->value = value;
    }

    nbt_int::~nbt_int() = default;

    void nbt_int::write(std::ostream &out) const {
        bin_write<int>(out, nbt_type_int);
        bin_write<int>(out, value);
    }

    void nbt_int::read(std::istream &in, bool read_type) {
        if (read_type)assert_next_int(in, nbt_type_int);
        bin_read<int>(in, &value);
    }

    nbt_type nbt_int::type() const {
        return nbt_type_int;
    }

    std::string nbt_int::to_str(std::string) const {
        return std::to_string(value) + "I";
    }

}