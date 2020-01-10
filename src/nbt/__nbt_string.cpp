//
// Created by Jack Armstrong on 11/23/19.
//

#include <utils/utils.h>
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

    std::map<std::string, std::shared_ptr<nbt> > nbt_string::as_compound() const {
        RAISE_ERROR("nbt::nbt_string as_compound");
    }

    float nbt_string::as_float() const {
        try{
            return boost::lexical_cast<float>(value);
        }catch(boost::bad_lexical_cast&){
            RAISE_ERROR("nbt::nbt_string as_float");
        }
    }

    int nbt_string::as_int() const {
        try{
            return boost::lexical_cast<int>(value);
        }catch(boost::bad_lexical_cast&){
            RAISE_ERROR("nbt::nbt_string as_int")
        }
    }

    std::vector<std::shared_ptr<nbt>> nbt_string::as_list() const {
        RAISE_ERROR("nbt::nbt_string as_list")
    }

    long nbt_string::as_long() const {
        try{
            return boost::lexical_cast<long>(value);
        }catch(boost::bad_lexical_cast&){
            RAISE_ERROR("nbt::nbt_string as_long");
        }
    }

    short nbt_string::as_short() const {
        try{
            return boost::lexical_cast<short>(value);
        }catch(boost::bad_lexical_cast&){
            RAISE_ERROR("nbt::nbt_string as_short");
        }
    }

    std::string nbt_string::as_string() const {
        return value;
    }

    std::map<std::string, std::shared_ptr<nbt>> &nbt_string::compound_ref() {
        RAISE_ERROR("nbt::nbt_string compound_ref")
    }

    float &nbt_string::float_ref() {
        RAISE_ERROR("nbt::nbt_string float_ref")
    }

    int &nbt_string::int_ref() {
        RAISE_ERROR("nbt::nbt_string int_ef")
    }

    std::vector<std::shared_ptr<nbt>> &nbt_string::list_ref() {
        RAISE_ERROR("nbt::nbt_string list_ref")
    }

    long &nbt_string::long_ref() {
        RAISE_ERROR("nbt::nbt_string long_ref")
    }

    short &nbt_string::short_ref() {
        RAISE_ERROR("nbt::nbt_string short_ref")
    }

    std::string &nbt_string::string_ref() {
        return value;
    }

}