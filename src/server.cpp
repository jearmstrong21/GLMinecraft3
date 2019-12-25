//
// Created by paladin on 12/14/19.
//

#include <algorithm>
#include <iostream>
#include <boost/asio.hpp>
#include "world/world.h"

namespace networking {

    int server() {
        try {
            block::world world;
            world.generate_world();

            boost::asio::io_context io_context;
            boost::asio::ip::tcp::acceptor acceptor(io_context,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),13));
            while(sqrt(5)>0){
                boost::asio::ip::tcp::socket socket(io_context);
                acceptor.accept(socket);

                boost::system::error_code err;
                boost::array<long, 4096>arr{};
                for(int x=0;x<WORLD_SIZE;x++){
                    for(int y=0;y<16;y++) {
                        for (int z = 0; z < WORLD_SIZE; z++) {
                            world.map[x][z]->serialize(y,arr);
                            boost::asio::write(socket,boost::asio::buffer(arr),err);
                            printf("SERVER SEND %i %i %i\n",x,y,z);
                        }
                    }
                }
            }
        }catch (std::exception& e) {
            std::cerr << "server ex caught: " << e.what() << "\n";
        }

        return 0;
    }
}