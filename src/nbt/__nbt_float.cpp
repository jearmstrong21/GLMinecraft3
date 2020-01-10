//
// Created by Jack Armstrong on 11/23/19.
//

#include <utils/utils.h>
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

    std::map<std::string, std::shared_ptr<nbt> > nbt_float::as_compound() const {
        RAISE_ERROR("nbt::nbt_float as_compound");
    }

    float nbt_float::as_float() const {
        return value;
    }

    int nbt_float::as_int() const {
        return value;
    }

    std::vector<std::shared_ptr<nbt>> nbt_float::as_list() const {
        RAISE_ERROR("nbt::nbt_float as_list");
    }

    long nbt_float::as_long() const {
        return value;
    }

    short nbt_float::as_short() const {
        return value;
    }

    std::string nbt_float::as_string() const {
        return std::to_string(value);
    }

    std::map<std::string, std::shared_ptr<nbt>> &nbt_float::compound_ref() {
        RAISE_ERROR("nbt::nbt_float compound_ref");
    }

    float &nbt_float::float_ref() {
        return value;
    }

    int &nbt_float::int_ref() {
        RAISE_ERROR("nbt::nbt_float int_ref");
    }

    std::vector<std::shared_ptr<nbt>> &nbt_float::list_ref() {
        RAISE_ERROR("nbt::nbt_float list_ref");
    }

    long &nbt_float::long_ref() {
        RAISE_ERROR("nbt::nbt_float long_ref");
    }

    short &nbt_float::short_ref() {
        RAISE_ERROR("nbt::nbt_float short_ref");
    }

    std::string &nbt_float::string_ref() {
        RAISE_ERROR("nbt::nbt_float string_ref");
    }
}