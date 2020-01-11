//
// Created by Jack Armstrong on 11/23/19.
//

#include "utils/utils.h"
#include "__nbt_compound.h"
#include <type_traits>

namespace nbt {

    nbt_compound::nbt_compound(std::istream &in, bool read_type) {
        read(in, read_type);
    }

    nbt_compound::nbt_compound(std::map<std::string, std::shared_ptr<nbt>> value) {
        this->value = std::move(value);
    }

    void nbt_compound::write(std::ostream &out) const {
        bin_write<int>(out, nbt_type_compound);
        bin_write<int>(out, value.size());
        for (auto &pair:value) {
            nbt_string(pair.first).write(out);
            pair.second->write(out);
        }
    }

    void nbt_compound::read(std::istream &in, bool read_type) {
        if (read_type)assert_next_int(in, nbt_type_compound);
        int size;
        bin_read<int>(in, &size);
        value.clear();
        for (int i = 0; i < size; i++) {
            nbt_string name(in, true);
            value[name.value] = std::shared_ptr<nbt>(read_nbt(in));
        }
    }

    nbt_type nbt_compound::type() const {
        return nbt_type_compound;
    }

    std::string nbt_compound::to_str(std::string indent) const {
        std::string s = "{\n";
        for (auto &p:value) {
            s += "\t" + indent + p.first + ": " + p.second->to_str(indent + "\t") + ",\n";
        }
        s += indent + "}";
        return s;
    }

    nbt_compound::nbt_compound() {

    }

    nbt_compound::~nbt_compound() {

    }

    std::map<std::string, std::shared_ptr<nbt> > nbt_compound::as_compound() const {
        return value;
    }

    float nbt_compound::as_float() const {
        RAISE_ERROR("nbt::nbt_compound as_float")
    }

    int nbt_compound::as_int() const {
        RAISE_ERROR("nbt::nbt_compound as_int")
    }

    std::vector<std::shared_ptr<nbt>> nbt_compound::as_list() const {
        RAISE_ERROR("nbt::nbt_compound as_list")
    }

    long nbt_compound::as_long() const {
        RAISE_ERROR("nbt::nbt_compound as_long")
    }

    short nbt_compound::as_short() const {
        RAISE_ERROR("nbt::nbt_compound as_short")
    }

    std::string nbt_compound::as_string() const {
        RAISE_ERROR("nbt::nbt_compound as_string")
    }

    std::map<std::string, std::shared_ptr<nbt>> &nbt_compound::compound_ref() {
        return value;
    }

    float &nbt_compound::float_ref() {
        RAISE_ERROR("nbt::nbt_compound float_ref")
    }

    int &nbt_compound::int_ref() {
        RAISE_ERROR("nbt::nbt_compound int_ref")
    }

    std::vector<std::shared_ptr<nbt>> &nbt_compound::list_ref() {
        RAISE_ERROR("nbt::nbt_compound list_ref")
    }

    long &nbt_compound::long_ref() {
        RAISE_ERROR("nbt::nbt_compound long_ref")
    }

    short &nbt_compound::short_ref() {
        RAISE_ERROR("nbt::nbt_compound short_ref")
    }

    std::string &nbt_compound::string_ref() {
        RAISE_ERROR("nbt::nbt_compound string_ref")
    }

}