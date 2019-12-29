//
// Created by Jack Armstrong on 11/23/19.
//

#include <csignal>
#include "__nbt_utils.h"

namespace nbt {

    void assert_next_int(std::istream &in, int expected) {
        int i;
        bin_read<int>(in,&i);
        if(i!=expected){
            printf("NBT PARSE ERROR %i %i\n",i,expected);
            std::exit(11);
        }
    }

}
