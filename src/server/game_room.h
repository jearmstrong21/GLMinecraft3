//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_GAME_ROOM_H
#define GLMINECRAFT3_GAME_ROOM_H

#include <set>
#include <map>
#include "world/world.h"
#include <boost/asio.hpp>

namespace server {

    struct game_room {
        block::world world;
        std::map<std::string,std::shared_ptr<nbt::nbt>>entities;
        std::set<server_player_ptr>players;

        boost::asio::deadline_timer timer;

        entity_type_player entity_type_player;

        void frame_handler(boost::system::error_code err){
            auto nbt_entities=new nbt::nbt_compound();
            for(const auto& e:entities){
                nbt_entities->value[e.first]=e.second;
                nbt::cast_float(nbt::cast_list(nbt::cast_compound(e.second)->value["position"])->value[0])->value+=0.1F;
            }
            broadcast_to_all(nbt::make_compound({
                                                        {"entities",std::shared_ptr<nbt::nbt>(nbt_entities)}
                                                }));
            int FPS=30;
            timer.expires_at(timer.expires_at()+boost::posix_time::milliseconds((int)(1000/FPS)));
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

        void join(const server_player_ptr& ptr){
            ptr->send_world(world);
            std::shared_ptr<nbt::nbt>entity=entity_type_player.initialize();
            nbt::cast_compound(entity)->value["position"]=nbt::make_list({nbt::make_float(8),nbt::make_float(8),nbt::make_float(8)});
            std::string id=spawn_entity(entity);
            nbt::cast_compound(entity)->value["name"]=nbt::make_string(id);
            ptr->deliver(nbt::make_compound({
                                                    {"player_id",nbt::make_string(id)}
                                            }));
            players.insert(ptr);
            ptr->entity_id=id;
            std::cout<<"PLAYER "<<ptr->entity_id<<" JOINED\n";
        }

        void kill_entity(const std::string& id) {
            entities.erase(id);
        }

        void leave(const server_player_ptr& ptr){
            players.erase(ptr);
            kill_entity(ptr->entity_id);
            std::cout<<"PLAYER "<<ptr->entity_id<<" LEFT\n";
        }
        void broadcast_to_all(const std::shared_ptr<nbt::nbt>&msg){
            for(const auto& p:players)p->deliver(msg);
        }
    };

}

#endif //GLMINECRAFT3_GAME_ROOM_H
