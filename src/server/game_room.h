//
// Created by Jack Armstrong on 12/29/19.
//

#ifndef GLMINECRAFT3_GAME_ROOM_H
#define GLMINECRAFT3_GAME_ROOM_H

#include <set>
#include <map>
#include "world/world.h"
#include "server_player.h"
#include <boost/asio.hpp>
#include <utility>
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>         // streaming operators etc.
#include "entity/entity.h"

namespace server {

    struct game_room {

        static game_room* instance;

        utils::profiler profiler;
        bool game_loop_is_over;

        block::world world;
        std::set<server_player_ptr> players;

        boost::asio::deadline_timer timer;

        std::mutex protect_game_state;
        std::string queued_chat;

        std::map<std::string, entity::entity_ptr> entities;

        std::shared_ptr<nbt::nbt> get_entity_list();

        void frame_handler(boost::system::error_code err);

        explicit game_room(boost::asio::io_context &io_context);

        ~game_room();

        std::string get_next_entity_id();

        std::string spawn_entity(const std::function<entity::entity_ptr(const std::string &)> &create);

        void join(const server_player_ptr &ptr);

        void handle_player_interaction_packet(const server_player_ptr &player, const std::shared_ptr<nbt::nbt> &data);

        void kill_entity(const std::string &id);

        void leave(const server_player_ptr &ptr);

        void broadcast_to_all(const std::shared_ptr<nbt::nbt> &msg);
    };

    utils::profiler* profiler();

}

#endif //GLMINECRAFT3_GAME_ROOM_H
