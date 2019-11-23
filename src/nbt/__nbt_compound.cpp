//
// Created by Jack Armstrong on 11/23/19.
//

#include "__nbt_compound.h"


#include "__nbt_compound.h"

namespace nbt {

    nbt_compound::nbt_compound(std::istream &in,bool read_type) {
        read(in,read_type);
    }

    nbt_compound::nbt_compound(std::map<std::string,std::shared_ptr<nbt>>value){
        this->value=std::move(value);
    }

    void nbt_compound::write(std::ostream &out) const {
        bin_write<int>(out,nbt_type_compound);
        bin_write<int>(out,value.size());
        for(auto&pair:value){
            nbt_string(pair.first).write(out);
            pair.second->write(out);
        }
    }

    void nbt_compound::read(std::istream &in,bool read_type) {
        if(read_type)assert_next_int(in,nbt_type_compound);
        int size;
        bin_read<int>(in,&size);
        value.clear();
        for(int i=0;i<size;i++){
            nbt_string name(in,true);
            value[name.value]=std::shared_ptr<nbt>(read_nbt(in));
        }
    }

    nbt_type nbt_compound::type() const{
        return nbt_type_compound;
    }

    std::string nbt_compound::to_str(std::string indent) const{
        std::string s="{\n";
        for(auto&p:value){
            s+="\t"+indent+p.first+": "+p.second->to_str(indent+"\t")+",\n";
        }
        s+=indent+"}";
        return s;
    }

    nbt_compound::nbt_compound() {

    }

    nbt_compound::~nbt_compound() {

    }

}