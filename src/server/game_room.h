//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_GAME_ROOM_H
#define GLMINECRAFT3_GAME_ROOM_H

#include <set>
#include <map>
#include "world/world.h"
#include <boost/asio.hpp>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.

namespace server {

    struct game_room {
        block::world world;
        std::map<std::string, std::shared_ptr<nbt::nbt>> entities;
        std::set<server_player_ptr> players;

        boost::asio::deadline_timer timer;

        entity_type_player entity_type_player;

        std::mutex protect_game_state;
        std::string queued_chat;

        std::shared_ptr<nbt::nbt> get_entity_list() {
            auto nbt_entities = new nbt::nbt_compound();
            for (const auto& e:entities) {
                nbt_entities->value[e.first] = e.second;
            }
            return std::shared_ptr<nbt::nbt>(nbt_entities);
        }

        void frame_handler(boost::system::error_code err) {
            {
                std::lock_guard<std::mutex> guard(protect_game_state);
                broadcast_to_all(nbt::make_compound({
                    {"entities", get_entity_list()},
                    {"chat",nbt::make_string(queued_chat)}
                }));
                queued_chat="";
            }
            //NO LOGIC OUTSIDE OF THIS
            int FPS = 30;
            timer.expires_at(timer.expires_at() + boost::posix_time::milliseconds((int) (1000 / FPS)));
            timer.async_wait([this](boost::system::error_code err) {
                frame_handler(err);
            });
        }

        explicit game_room(boost::asio::io_context& io_context) : timer(io_context,
                                                                        boost::posix_time::milliseconds(0)) {
            world.generate_world();
            frame_handler(boost::system::error_code());
        }

        std::string get_next_entity_id() {
            std::stringstream str;
            str << boost::uuids::random_generator()();
            return str.str();
        }

        std::string spawn_entity(const std::shared_ptr<nbt::nbt>& e) {
            std::string i = get_next_entity_id();
            nbt::cast_compound(e)->value["id"] = nbt::make_string(i);
            entities[i] = e;
            return i;
        }

        void join(const server_player_ptr& ptr) {
            std::lock_guard<std::mutex> guard(protect_game_state);
            ptr->send_world(world);
            std::shared_ptr<nbt::nbt> entity = entity_type_player.initialize();
            nbt::cast_compound(entity)->value["position"] = nbt::make_list(
                    {nbt::make_float(24), nbt::make_float(150), nbt::make_float(24)});
            std::string id = spawn_entity(entity);
            nbt::cast_compound(entity)->value["name"] = nbt::make_string(id);
            ptr->deliver(nbt::make_compound({
                {"player_id", nbt::make_string(id)},
                {"entities",  get_entity_list()}
            }));
            players.insert(ptr);
            ptr->entity_id = id;
            std::cout << "PLAYER " << ptr->entity_id << " JOINED\n";
            queued_chat=ptr->entity_id+" joined the game";
        }

        void handle_player_interaction_packet(const server_player_ptr& player, const std::shared_ptr<nbt::nbt> data) {
            std::shared_ptr<nbt::nbt_compound> compound = nbt::cast_compound(data);
            std::lock_guard<std::mutex> guard(protect_game_state);
            std::shared_ptr<nbt::nbt_compound> ent = nbt::cast_compound(entities[player->entity_id]);
            {
                std::shared_ptr<nbt::nbt_compound> movement = nbt::cast_compound(compound->value["movement"]);
                bool left = nbt::cast_short(movement->value["left"])->value;
                bool right = nbt::cast_short(movement->value["right"])->value;
                bool front = nbt::cast_short(movement->value["front"])->value;
                bool back = nbt::cast_short(movement->value["back"])->value;
                bool sprint = nbt::cast_short(movement->value["sprint"])->value;

                std::shared_ptr<nbt::nbt_list> pos = nbt::cast_list(ent->value["position"]);
                std::shared_ptr<nbt::nbt_list>look=nbt::cast_list(ent->value["lookdir"]);
                float d = 0.1F;
                glm::vec3 curPos{nbt::cast_float(pos->value[0])->value,nbt::cast_float(pos->value[1])->value,nbt::cast_float(pos->value[2])->value};
                glm::vec3 curLook{nbt::cast_float(look->value[0])->value,nbt::cast_float(look->value[1])->value,nbt::cast_float(look->value[2])->value};
                glm::vec3 leftdir=glm::cross(curLook,glm::vec3{0,-1,0});
                if(front)curPos+=curLook*d*(float)(1+sprint);
                if(back)curPos-=curLook*d;
                if(left)curPos+=leftdir*d;
                if(right)curPos-=leftdir*d;
                pos->value[0]=nbt::make_float(curPos.x);
                pos->value[1]=nbt::make_float(curPos.y);
                pos->value[2]=nbt::make_float(curPos.z);
            }
            {
                std::shared_ptr<nbt::nbt_list>nlook=nbt::cast_list(compound->value["lookdir"]);
                std::shared_ptr<nbt::nbt_list>look=nbt::cast_list(ent->value["lookdir"]);
                nbt::cast_float(look->value[0])->value=nbt::cast_float(nlook->value[0])->value;
                nbt::cast_float(look->value[1])->value=nbt::cast_float(nlook->value[1])->value;
                nbt::cast_float(look->value[2])->value=nbt::cast_float(nlook->value[2])->value;
            }
            {
                std::string chat=nbt::cast_string(compound->value["chat"])->value;
                //TODO: commands will be parsed here
                if(!chat.empty())queued_chat="<"+player->entity_id+"> "+chat;
            }
        }

        void kill_entity(const std::string& id) {
            entities.erase(id);
        }

        void leave(const server_player_ptr& ptr) {
            players.erase(ptr);
            kill_entity(ptr->entity_id);
            std::cout << "PLAYER " << ptr->entity_id << " LEFT\n";
            queued_chat=ptr->entity_id+" left the game";
        }

        void broadcast_to_all(const std::shared_ptr<nbt::nbt>& msg) {
            for (const auto& p:players)p->deliver(msg);
        }
    };

}

#endif //GLMINECRAFT3_GAME_ROOM_H
