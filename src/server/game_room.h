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
#include "entity/entity_player.h"
#include "entity/entity_zombie.h"
#include "utils/profiler.h"
#include <future>
#include "item/item.h"
#include <queue>

namespace server {

    struct acceptor;

    struct delayed_task{
        int target_tick;
        std::function<void()> execute;
    };

    bool operator <(const delayed_task&a,const delayed_task&b);

    struct game_room {

        static game_room *instance;

        TRANSIENT acceptor *acceptor;

        TRANSIENT utils::profiler profiler;

        TRANSIENT bool game_loop_is_over;

        DATA block::world world;
        DATA std::set<server_player_ptr> players;

        TRANSIENT boost::asio::deadline_timer timer;

        TRANSIENT std::mutex protect_game_state;
        TRANSIENT std::string queued_chat;

        TRANSIENT int tick_number;
        TRANSIENT std::priority_queue<delayed_task>tasks;
        TRANSIENT std::stack<block::world_op>world_ops;

        DATA std::map<std::string, entity::entity_ptr> entities;

        DATA std::shared_ptr<nbt::nbt> get_entity_list();

        void frame_handler(boost::system::error_code err);

        explicit game_room(boost::asio::io_context &io_context, struct acceptor *acceptor);

        ~game_room();

        std::string get_next_entity_id();

        std::string spawn_entity(const std::function<entity::entity_ptr(const std::string &)> &create);

        void join(const server_player_ptr &ptr);

        void handle_player_interaction_packet(const server_player_ptr &player, const std::shared_ptr<nbt::nbt> &data);

        void kill_entity(const std::string &id);

        void leave(const server_player_ptr &ptr);

        void broadcast_to_all(const std::shared_ptr<nbt::nbt> &msg);
    };

}

#endif //GLMINECRAFT3_GAME_ROOM_H
