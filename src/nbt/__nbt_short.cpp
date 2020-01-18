//
// Created by Jack Armstrong on 11/23/19.
//

#include <utils/utils.h>
#include "__nbt_short.h"

namespace nbt {

    nbt_short::nbt_short() : value(0) {

    }

    nbt_short::nbt_short(std::istream &in, bool read_type) : value(0) {
        read(in, read_type);
    }

    nbt_short::nbt_short(short value) {
        this->value = value;
    }

    nbt_short::~nbt_short() = default;

    void nbt_short::write(std::ostream &out) const {
        bin_write<int>(out, nbt_type_short);
        bin_write<short>(out, value);
    }

    void nbt_short::read(std::istream &in, bool read_type) {
        if (read_type)assert_next_int(in, nbt_type_short);
        bin_read<short>(in, &value);
    }

    nbt_type nbt_short::type() const {
        return nbt_type_short;
    }

    std::string nbt_short::to_str(std::string) const {
        return std::to_string(value) + "S";
    }

    std::map<std::string, nbt_ptr> nbt_short::as_compound() const {
        RAISE_ERROR("nbt::nbt_short as_compound")
    }

    float nbt_short::as_float() const {
        return value;
    }

    int nbt_short::as_int() const {
        return value;
    }

    std::vector<nbt_ptr> nbt_short::as_list() const {
        RAISE_ERROR("nbt::nbt_short as_list")
    }

    long nbt_short::as_long() const {
        return value;
    }

    short nbt_short::as_short() const {
        return value;
    }

    std::string nbt_short::as_string() const {
        return std::to_string(value);
    }

    std::map<std::string, nbt_ptr> &nbt_short::compound_ref() {
        RAISE_ERROR("nbt::nbt_short compound_ref")
    }

    float &nbt_short::float_ref() {
        RAISE_ERROR("nbt::nbt_short float_ref")
    }

    int &nbt_short::int_ref() {
        RAISE_ERROR("nbt::nbt_short int_ref")
    }

    std::vector<nbt_ptr> &nbt_short::list_ref() {
        RAISE_ERROR("nbt::nbt_short list_ref")
    }

    long &nbt_short::long_ref() {
        RAISE_ERROR("nbt::nbt_short long_ref")
    }

    short &nbt_short::short_ref() {
        return value;
    }

    std::string &nbt_short::string_ref() {
        RAISE_ERROR("nbt::nbt_short string_ref")
    }

}