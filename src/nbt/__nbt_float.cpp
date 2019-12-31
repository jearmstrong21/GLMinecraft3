//
// Created by Jack Armstrong on 11/23/19.
//

#include "__nbt_float.h"

namespace nbt {

    nbt_float::nbt_float(std::istream &in, bool read_type) : value(0) {
        read(in, read_type);
    }

    nbt_float::nbt_float(float value) {
        this->value = value;
    }

    void nbt_float::write(std::ostream &out) const {
        bin_write<int>(out, nbt_type_float);
        bin_write<float>(out, value);
    }

    void nbt_float::read(std::istream &in, bool read_type) {
        if (read_type)assert_next_int(in, nbt_type_float);
        bin_read<float>(in, &value);
    }

    nbt_type nbt_float::type() const {
        return nbt_type_float;
    }

    std::string nbt_float::to_str(std::string) const {
        return std::to_string(value) + "F";
    }

    nbt_float::nbt_float() : value(0) {

    }

    nbt_float::~nbt_float() {

    }
}