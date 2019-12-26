//
// Created by paladin on 12/14/19.
//

#include <algorithm>
#include <iostream>
#include <boost/asio.hpp>
#include "world/world.h"
#include "both.h"

namespace networking {

    int server(int port) {
        try {
            block::world world;
            world.generate_world();

            boost::asio::io_context io_context;
            boost::asio::ip::tcp::acceptor acceptor(io_context,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port));
            while(sqrt(5)>0){
                boost::asio::ip::tcp::socket socket(io_context);
                acceptor.accept(socket);

                boost::system::error_code err;
//                boost::array<long,4096> arrs[16*WORLD_SIZE*WORLD_SIZE];
                printf("SEND WORLD\n");
//                int i=0;
                for(int x=0;x<WORLD_SIZE;x++){
                    for(int y=0;y<16;y++) {
                        for (int z = 0; z < WORLD_SIZE; z++) {
                            boost::array<long, 4096>arr{};
                            world.map[x][z]->serialize(y,arr);
//                            i++;
                            size_t len = boost::asio::write(socket, boost::asio::buffer(arr), err);
                        }
                    }
                }
//                boost::asio::write(socket,boost::asio::buffer(arrs),err);
            }
        }catch (std::exception& e) {
            std::cerr << "server ex caught: " << e.what() << "\n";
        }

        return 0;
    }
}