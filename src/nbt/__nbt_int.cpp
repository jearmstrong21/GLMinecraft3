//
// Created by Jack Armstrong on 11/23/19.
//

#include <utils/utils.h>
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

    std::map<std::string, std::shared_ptr<nbt> > nbt_int::as_compound() const {
        RAISE_ERROR("nbt::nbt_int as_compound");
    }

    float nbt_int::as_float() const {
        return value;
    }

    int nbt_int::as_int() const {
        return value;
    }

    std::vector<std::shared_ptr<nbt>> nbt_int::as_list() const {
        RAISE_ERROR("nbt::nbt_int as_list");
    }

    long nbt_int::as_long() const {
        return value;
    }

    short nbt_int::as_short() const {
        return value;
    }

    std::string nbt_int::as_string() const {
        return std::to_string(value);
    }

    std::map<std::string, std::shared_ptr<nbt>> &nbt_int::compound_ref() {
        RAISE_ERROR("nbt::nbt_int compound_ref");
    }

    float &nbt_int::float_ref() {
        RAISE_ERROR("nbt::nbt_int float_ref");
    }

    int &nbt_int::int_ref() {
        return value;
    }

    std::vector<std::shared_ptr<nbt>> &nbt_int::list_ref() {
        RAISE_ERROR("nbt::nbt_int list_ref");
    }

    long &nbt_int::long_ref() {
        RAISE_ERROR("nbt::nbt_int long_ref");
    }

    short &nbt_int::short_ref() {
        RAISE_ERROR("nbt::nbt_int short_ref");
    }

    std::string &nbt_int::string_ref() {
        RAISE_ERROR("nbt::nbt_int string_ref");
    }

}