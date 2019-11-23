//
// Created by Jack Armstrong on 11/23/19.
//

#include "__nbt_short.h"

namespace nbt {

    nbt_short::nbt_short():value(0){

    }

    nbt_short::nbt_short(std::istream &in,bool read_type):value(0) {
        read(in,read_type);
    }

    nbt_short::nbt_short(short value) {
        this->value=value;
    }

    nbt_short::~nbt_short() = default;

    void nbt_short::write(std::ostream &out)  const{
        bin_write<int>(out, nbt_type_short);
        bin_write<short>(out, value);
    }

    void nbt_short::read(std::istream&in,bool read_type){
        if(read_type)assert_next_int(in,nbt_type_short);
        bin_read<short>(in,&value);
    }

    nbt_type nbt_short::type() const{
        return nbt_type_short;
    }

    std::string nbt_short::to_str(std::string) const{
        return std::to_string(value)+"S";
    }

}