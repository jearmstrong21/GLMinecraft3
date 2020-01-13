//
// Created by Jack Armstrong on 11/23/19.
//

#include "nbt.h"

namespace nbt {

    const std::function<std::shared_ptr<nbt>(const short &value)> make_short = make<nbt_short, short>;
    const std::function<std::shared_ptr<nbt>(const int &value)> make_int = make<nbt_int, int>;
    const std::function<std::shared_ptr<nbt>(const float &value)> make_float = make<nbt_float, float>;
    const std::function<std::shared_ptr<nbt>(const long &value)> make_long = make<nbt_long, long>;
    const std::function<std::shared_ptr<nbt>(const std::string &value)> make_string = make<nbt_string, std::string>;
    const std::function<std::shared_ptr<nbt>(
            const std::map<std::string, std::shared_ptr<nbt>> &value)> make_compound = make<nbt_compound, std::map<std::string, std::shared_ptr<nbt>>>;
    const std::function<std::shared_ptr<nbt>(
            const std::vector<std::shared_ptr<nbt>> &value)> make_list = make<nbt_list, std::vector<std::shared_ptr<nbt>>>;

    const std::function<std::shared_ptr<nbt_short>(const std::shared_ptr<nbt> &value)> cast_DEPRECATED_SUN_JAN_12_2020short = cast<nbt_short>;
    const std::function<std::shared_ptr<nbt_int>(const std::shared_ptr<nbt> &value)> cast_DEPRECATED_SUN_JAN_12_2020int = cast<nbt_int>;
    const std::function<std::shared_ptr<nbt_float>(const std::shared_ptr<nbt> &value)> cast_DEPRECATED_SUN_JAN_12_2020float = cast<nbt_float>;
    const std::function<std::shared_ptr<nbt_long>(const std::shared_ptr<nbt> &value)> cast_DEPRECATED_SUN_JAN_12_2020long = cast<nbt_long>;
    const std::function<std::shared_ptr<nbt_string>(const std::shared_ptr<nbt> &value)> cast_DEPRECATED_SUN_JAN_12_2020string = cast<nbt_string>;
    const std::function<std::shared_ptr<nbt_compound>(
            const std::shared_ptr<nbt> &value)> cast_DEPRECATED_SUN_JAN_12_2020compound = cast<nbt_compound>;
    const std::function<std::shared_ptr<nbt_list>(const std::shared_ptr<nbt> &value)> cast_DEPRECATED_SUN_JAN_12_2020list = cast<nbt_list>;

    std::shared_ptr<nbt> read_nbt(std::istream &in) noexcept {
        int type;
        bin_read(in, &type);
        switch (type) {
            case nbt_type_short:
                return std::shared_ptr<nbt>(new nbt_short(in, false));
            case nbt_type_int:
                return std::shared_ptr<nbt>(new nbt_int(in, false));
            case nbt_type_long:
                return std::shared_ptr<nbt>(new nbt_long(in, false));
            case nbt_type_float:
                return std::shared_ptr<nbt>(new nbt_float(in, false));
            case nbt_type_string:
                return std::shared_ptr<nbt>(new nbt_string(in, false));
            case nbt_type_compound:
                return std::shared_ptr<nbt>(new nbt_compound(in, false));
            case nbt_type_list:
                return std::shared_ptr<nbt>(new nbt_list(in, false));
            default:
                return nullptr;
        }
    }

}