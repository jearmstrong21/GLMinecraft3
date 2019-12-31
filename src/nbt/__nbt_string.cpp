//
// Created by Jack Armstrong on 11/23/19.
//

#include "__nbt_string.h"

namespace nbt {

    nbt_string::nbt_string(std::istream &in, bool read_type) {
        read(in, read_type);
    }

    nbt_string::nbt_string(std::string value) {
        this->value = std::move(value);
    }

    void nbt_string::write(std::ostream &out) const {
        bin_write<int>(out, nbt_type_string);
        bin_write<int>(out, value.size());
        for (int i = 0; i < value.size(); i++) {
            bin_write<char>(out, value[i]);
        }
    }

    void nbt_string::read(std::istream &in, bool read_type) {
        if (read_type)assert_next_int(in, nbt_type_string);
        int size;
        bin_read<int>(in, &size);
        value = "";
        for (int i = 0; i < size; i++) {
            char ch;
            bin_read<char>(in, &ch);
            value += ch;
        }
    }

    std::string nbt_string::to_str(std::string) const {
        return "\"" + value + "\"";
    }

    nbt_type nbt_string::type() const {
        return nbt_type_string;
    }

    nbt_string::nbt_string() : value("") {

    }

    nbt_string::~nbt_string() {

    }

}