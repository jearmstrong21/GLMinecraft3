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
#include "server/entity.h"
#include <boost/thread.hpp>

namespace networking {

    struct game_player {
        std::string entity_id;
        virtual ~game_player()= default;
        virtual void deliver(const std::shared_ptr<nbt::nbt>& msg)=0;
        virtual void send_world(const block::world&world)=0;
    };

    typedef std::shared_ptr<game_player>game_player_ptr;

    struct game_room {
        block::world world;
        std::map<std::string,std::shared_ptr<nbt::nbt>>entities;
        std::set<game_player_ptr>players;

        boost::asio::deadline_timer timer;

        server::entity_type_player entity_type_player;

        void frame_handler(boost::system::error_code err){
            std::cout<<"Frame\n";
            auto nbt_entities=new nbt::nbt_compound();
            for(const auto& e:entities){
                nbt_entities->value[e.first]=e.second;
                nbt::cast_float(nbt::cast_list(nbt::cast_compound(e.second)->value["position"])->value[0])->value+=1;
            }
            broadcast_to_all(nbt::make_compound({
                {"entities",std::shared_ptr<nbt::nbt>(nbt_entities)}
            }));
            std::cout<<"Broadcast entity list\n";
            timer.expires_at(timer.expires_at()+boost::posix_time::seconds(1));
            timer.async_wait([this](boost::system::error_code err){
                frame_handler(err);
            });
        }

        explicit game_room(boost::asio::io_context&io_context):timer(io_context,boost::posix_time::seconds(0)){
            world.generate_world();
            frame_handler(boost::system::error_code());
        }

        std::string get_next_entity_id(){
            return std::to_string(rand()%100000);//used as player ID and entity ID
        }

        std::string spawn_entity(const std::shared_ptr<nbt::nbt>&e){
            std::string i=get_next_entity_id();
            nbt::cast_compound(e)->value["id"]=nbt::make_string(i);
            entities[i]=e;
            return i;
        }

        void join(const game_player_ptr& ptr){
            ptr->send_world(world);
            std::string id=spawn_entity(entity_type_player.initialize());
            ptr->deliver(nbt::make_compound({
                {"player_id",nbt::make_string(id)}
            }));
            players.insert(ptr);
            ptr->entity_id=id;
            std::cout<<"PLAYER "<<ptr->entity_id<<" JOINED\n";
        }

        void kill_entity(std::string id) {
            entities.erase(id);
        }

        void leave(const game_player_ptr& ptr){
            players.erase(ptr);
            kill_entity(ptr->entity_id);
            std::cout<<"PLAYER "<<ptr->entity_id<<" LEFT\n";
        }
        void broadcast_to_all(const std::shared_ptr<nbt::nbt>&msg){
            for(const auto& p:players)p->deliver(msg);
        }
    };

    struct network_session: game_player, std::enable_shared_from_this<network_session> {
        boost::asio::ip::tcp::socket socket;
        game_room&room;
        network_session(boost::asio::ip::tcp::socket socket,game_room&room):socket(std::move(socket)),room(room){

        }

        ~network_session() override =default;

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
//                    room.deliver(nbt::make_compound({
//                                                             {"from",   nbt::make_string("server")},
//                                                             {"reason", nbt::make_string("revenge boop")}
//                                                     }));
                }
            });
        }

        void deliver(const std::shared_ptr<nbt::nbt>&msg) override {
            std::stringstream a;
            msg->write(a);
            boost::array<unsigned long,1>size{a.str().length()};
            boost::asio::write(socket,boost::asio::buffer(size));
            boost::asio::write(socket,boost::asio::buffer(a.str()));
//            std::cout<<"Attempt to deliver "<<msg.to_str("")<<"\n";
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