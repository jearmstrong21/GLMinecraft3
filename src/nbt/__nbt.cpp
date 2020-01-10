//
// Created by Jack Armstrong on 1/9/20.
//

#include "__nbt.h"
#include "utils/utils.h"

namespace nbt {

    std::map<std::string,std::shared_ptr<nbt> > nbt::as_compound()const {
        RAISE_ERROR("nbt::nbt as_compound");
    }

    float nbt::as_float()const {
        RAISE_ERROR("nbt::nbt as_float");
    }

    int nbt::as_int() const{
        RAISE_ERROR("nbt::nbt as_int");
    }

    std::vector<std::shared_ptr<nbt>> nbt::as_list() const{
        RAISE_ERROR("nbt::nbt as_list");
    }

    long nbt::as_long() const{
        RAISE_ERROR("nbt::nbt as_long");
    }

    short nbt::as_short() const{
        RAISE_ERROR("nbt::nbt as_short");
    }

    std::string nbt::as_string()const {
        RAISE_ERROR("nbt::nbt as_string");
    }

    const std::map<std::string, std::shared_ptr<nbt>> &nbt::compound_ref()const {
        RAISE_ERROR("nbt::nbt compound_ref");
    }

    const float &nbt::float_ref()const {
        RAISE_ERROR("nbt::nbt float_ref");
    }

    const int &nbt::int_ref()const {
        RAISE_ERROR("nbt::nbt int_ref");
    }

    const std::vector<std::shared_ptr<nbt>> nbt::list_ref()const {
        RAISE_ERROR("nbt::nbt list_ref");
    }

    const long &nbt::long_ref()const {
        RAISE_ERROR("nbt::nbt long_ref");
    }

    const short &nbt::short_ref()const {
        RAISE_ERROR("nbt::nbt short_ref");
    }

    const std::string &nbt::string_ref() const{
        RAISE_ERROR("nbt::nbt string_ref");
    }

}