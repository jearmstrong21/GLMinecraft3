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
#include <memory>
#include <deque>

namespace networking {

    struct game_player{
        virtual ~game_player()= default;
        virtual void deliver(const nbt::nbt& msg)=0;
        virtual void send_world(const block::world&world)=0;
    };

    typedef std::shared_ptr<game_player>game_player_ptr;

    struct game_room {
        block::world world;

        boost::asio::deadline_timer timer;

        void handle_async_boop(boost::system::error_code err){
            std::cout<<"Async boop\n";
            deliver(*nbt::make_compound({
                                               {"value",nbt::make_int(5)}
            }));
            timer.expires_at(timer.expires_at()+boost::posix_time::seconds(1));
            timer.async_wait([this](boost::system::error_code err){
                handle_async_boop(err);
            });
        }

        explicit game_room(boost::asio::io_context&io_context):timer(io_context,boost::posix_time::seconds(0)){
            world.generate_world();
            timer.async_wait([this](boost::system::error_code err){
                handle_async_boop(err);
            });
        }

        void join(game_player_ptr ptr){
            ptr->send_world(world);
            players.insert(ptr);
            //send world and shit
        }
        void leave(game_player_ptr ptr){
            players.erase(ptr);
        }
        void deliver(const nbt::nbt&msg){
            for(const auto& p:players)p->deliver(msg);
        }

        std::set<game_player_ptr>players;
    };

    struct network_session: game_player, std::enable_shared_from_this<network_session> {
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
            boost::asio::write(socket,boost::asio::buffer(size));
            boost::asio::write(socket,boost::asio::buffer(a.str()));
            std::cout<<"Attempt to deliver "<<msg.to_str("")<<"\n";
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

        void here_it_comes_again(){}
        void cant_remember_when_we_came_so_close_to_love_before(){}

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

        network_server(boost::asio::io_context&ioc,const boost::asio::ip::tcp::endpoint&endpoint):acceptor(ioc,endpoint),room(ioc){
            do_accept();
        }

    };

    int server(int port) {
        try {
            boost::asio::io_context io_context;
            boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

            network_server kneon_nights(io_context,endpoint);
            kneon_nights.here_it_comes_again();
            kneon_nights.cant_remember_when_we_came_so_close_to_love_before();

            io_context.run();
        } catch (std::exception& e) {
            std::cerr << "server ex caught: " << e.what() << "\n";
        }

        return 0;
    }
}