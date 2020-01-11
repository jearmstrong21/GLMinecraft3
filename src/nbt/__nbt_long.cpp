//
// Created by Jack Armstrong on 11/23/19.
//

#include <utils/utils.h>
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

    std::map<std::string, std::shared_ptr<nbt> > nbt_long::as_compound() const {
        RAISE_ERROR("nbt::nbt_long as_compound")
    }

    float nbt_long::as_float() const {
        return value;
    }

    int nbt_long::as_int() const {
        return value;
    }

    std::vector<std::shared_ptr<nbt>> nbt_long::as_list() const {
        RAISE_ERROR("nbt::nbt_long as_list")
    }

    long nbt_long::as_long() const {
        return value;
    }

    short nbt_long::as_short() const {
        return value;
    }

    std::string nbt_long::as_string() const {
        return std::to_string(value);
    }

    std::map<std::string, std::shared_ptr<nbt>> &nbt_long::compound_ref() {
        RAISE_ERROR("nbt::nbt_long compound_ref")
    }

    float &nbt_long::float_ref() {
        RAISE_ERROR("nbt::nbt_long float_ref")
    }

    int &nbt_long::int_ref() {
        RAISE_ERROR("nbt::nbt_long int_ref")
    }

    std::vector<std::shared_ptr<nbt>> &nbt_long::list_ref() {
        RAISE_ERROR("nbt::nbt_long list_ref")
    }

    long &nbt_long::long_ref() {
        return value;
    }

    short &nbt_long::short_ref() {
        RAISE_ERROR("nbt::nbt_long short_ref")
    }

    std::string &nbt_long::string_ref() {
        RAISE_ERROR("nbt::nbt_long string_ref")
    }

}