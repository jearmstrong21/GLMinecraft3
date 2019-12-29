//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_SESSION_H
#define GLMINECRAFT3_SESSION_H

#include <boost/asio.hpp>
#include "game_room.h"
#include <memory>
#include "server_player.h"

namespace server {

    struct session: server_player, std::enable_shared_from_this<session> {
        boost::asio::ip::tcp::socket socket;
        game_room&room;
        session(boost::asio::ip::tcp::socket socket,game_room&room):socket(std::move(socket)),room(room){

        }

        ~session() override =default;

        void start(){
            room.join(shared_from_this());
            boost::thread t([this](){
                while(sqrt(5)>0) {
                    boost::array<long, 1> arr{};
                    boost::system::error_code err;
                    boost::asio::read(socket, boost::asio::buffer(arr),err);
                    if(err){
                        room.leave(shared_from_this());
                        return;
                    }
//                    std::cout << "READ_PACKET\n";
                    unsigned long length_of_nbt = arr[0];
                    boost::asio::streambuf read_buffer;
                    boost::asio::read(socket, read_buffer, boost::asio::transfer_exactly(length_of_nbt),err);
                    if(err){
                        room.leave(shared_from_this());
                        return;
                    }
                    boost::asio::streambuf::const_buffers_type bufs = read_buffer.data();
                    std::string str(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + length_of_nbt);
                    std::istringstream stream(str);
                    std::shared_ptr<nbt::nbt> obj = nbt::read_nbt(stream);
                    std::cout << obj->to_str("") << "\n";
                    // TODO: THIS IS WHERE WE HANDLE ANY PACKETS FROM THE CLIENT: OBJ
                }
            });
        }

        void deliver(const std::shared_ptr<nbt::nbt>&msg) override {
            std::stringstream a;
            msg->write(a);
            boost::array<unsigned long,1>size{a.str().length()};
            boost::asio::write(socket,boost::asio::buffer(size));
            boost::asio::write(socket,boost::asio::buffer(a.str()));
        }

        void send_world(const block::world&world) override{
            for(int x=0;x<WORLD_SIZE;x++){
                for(int y=0;y<16;y++){
                    for(int z=0;z<WORLD_SIZE;z++){
                        boost::array<long,4096>arr{};
                        boost::system::error_code err;
                        world.map[x][z]->serialize(y,arr);
                        boost::asio::write(socket,boost::asio::buffer(arr),err);
                        if(err){
                            std::cout<<"ERROR SENDING WORLD DATA "<<x<<","<<y<<","<<z<<". Error: "<<err<<"\n";
                        }
                    }
                }
            }
        }
    };


}

#endif //GLMINECRAFT3_SESSION_H
