//
// Created by paladin on 12/14/19.
//

#include <algorithm>
#include <iostream>
#include <boost/asio.hpp>
#include <nbt/__nbt_float.h>
#include <nbt/nbt.h>
#include "world/world.h"
#include "both.h"


namespace networking {

    struct game_player{
        virtual ~game_player()= default;
        virtual void deliver(const nbt::nbt& msg)=0;
        virtual void send_world(const block::world&world)=0;
    };

    typedef std::shared_ptr<game_player>game_player_ptr;

    struct game_room {
        block::world world;

        game_room(){
            world.generate_world();
        }

        void join(game_player_ptr ptr){
            players.insert(ptr);
            ptr->send_world(world);
            //send world and shit
        }
        void leave(game_player_ptr ptr){
            players.erase(ptr);
        }
        void deliver(const nbt::nbt&msg){
            for(auto p:players)p->deliver(msg);
        }

        std::set<game_player_ptr>players;
    };

    struct network_session:game_player,std::enable_shared_from_this<network_session>{
        boost::asio::ip::tcp::socket socket;
        game_room&room;
        network_session(boost::asio::ip::tcp::socket socket,game_room&room):socket(std::move(socket)),room(room){

        }

        ~network_session() override =default;

        void start(){
            room.join(shared_from_this());
        }

        void deliver(const nbt::nbt&msg) override {
            std::stringstream a;
            msg.write(a);
            boost::array<unsigned long,1>size{a.str().length()};
            boost::system::error_code err1,err2;
            size_t l1=boost::asio::write(socket,boost::asio::buffer(size),err1);
            size_t l2=boost::asio::write(socket,boost::asio::buffer(a.str()),err2);
            std::cout<<"Delivered "<<(l1+l2)<<" bytes to client: "<<l1<<" bytes in head, "<<l2<<" bytes in body.\n";
            if(err1){
                std::cout<<"\tERROR head send"<<err1<<"\n";
            }
            if(err2){
                std::cout<<"\tERROR body send"<<err2<<"\n";
            }
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

    struct network_server{

        boost::asio::ip::tcp::acceptor acceptor;
        game_room room;

        void do_accept(){
            acceptor.async_accept([this](boost::system::error_code err,boost::asio::ip::tcp::socket socket){
                std::cout<<"async_accept\n ";
                if(!err){
                    std::cout<<"\t!err\n";
                    std::make_shared<network_session>(std::move(socket),room)->start();
                }else{
                    std::cout<<"\terr "<<err<<"\n";
                }
                do_accept();
            });
        }

        network_server(boost::asio::io_context&ioc,const boost::asio::ip::tcp::endpoint&endpoint):acceptor(ioc,endpoint){
            do_accept();
        }

    };

    int server(int port) {
        try {
            boost::asio::io_context io_context;
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

            network_server kneon_nights(io_context,endpoint);

            io_context.run();
//            while (sqrt(5) > 0) {
//                boost::asio::ip::tcp::socket socket(io_context);
//                acceptor.accept(socket);
//
//                boost::system::error_code err;
////                boost::array<long,4096> arrs[16*WORLD_SIZE*WORLD_SIZE];
//                printf("SEND WORLD\n");
////                int i=0;
//                for (int x = 0; x < WORLD_SIZE; x++) {
//                    for (int y = 0; y < 16; y++) {
//                        for (int z = 0; z < WORLD_SIZE; z++) {
//                            boost::array<long, 4096> arr{};
//                            world.map[x][z]->serialize(y, arr);
////                            i++;
//                            std::cout << "Writing " << x << ", " << y << ", " << z << std::endl;
//                            size_t len = boost::asio::write(socket, boost::asio::buffer(arr), err);
//                            std::cout << "Wrote " << x << ", " << y << ", " << z << std::endl;
//                        }
//                    }
//                }
//
//                std::shared_ptr<nbt::nbt> test = nbt::make_compound({
//                                                                            {"a", nbt::make_float(1.2f)},
//                                                                            {"b", nbt::make_string("asdf")},
//                                                                            {"c", nbt::make_int(47)}
//                                                                    });
//                std::stringstream a;
//                test->write(a);
//
////                for(int i = 0; i < a.str().length(); i++) {
////                    printf("%x\n", a.str()[i]);
////                }
//
//                std::cout << "Writing value " << test->to_str(" ") << std::endl;
//
//                boost::array<unsigned long, 1> sz{
//                        a.str().length()
//                };
//
//                std::cout << "Length: " << sz[0] << std::endl;
//                std::cout << "Data: " << a.str() << std::endl;
////                sz[0] = a.str().length();
//                boost::asio::write(socket, boost::asio::buffer(sz));
//                boost::asio::write(socket, boost::asio::buffer(a.str()));
//                std::cout << "wrote" << std::endl;
////                boost::asio::write(socket,boost::asio::buffer(arrs),err);
//            }
        } catch (std::exception& e) {
            std::cerr << "server ex caught: " << e.what() << "\n";
        }

        return 0;
    }
}