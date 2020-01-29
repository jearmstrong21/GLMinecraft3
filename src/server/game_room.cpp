//
// Created by Jack Armstrong on 1/17/20.
//

#include <boost/thread.hpp>
#include "game_room.h"
#include "utils/profiler.h"
#include "acceptor.h"

namespace server {

    game_room *game_room::instance;

    game_room::game_room(boost::asio::io_context &io_context, struct acceptor *a) : timer(
            io_context,//oo look struct acceptor is this C
            boost::posix_time::milliseconds(0)),
                                                                                    acceptor(a) {
        boost::thread t([this]() {
            std::cout << "\nENTER to exit\n";
            std::string s;
            std::getline(std::cin, s);
            game_loop_is_over = true;
            std::cout << "game_loop_is_over\n";
            acceptor->stop();
        });
        instance = this;
        game_loop_is_over = false;
        world.generate_world();
        frame_handler(boost::system::error_code());
    }

    std::shared_ptr<nbt::nbt> game_room::get_entity_list() {
        auto nbt_entities = new nbt::nbt_compound();
        for (const auto &e:entities) {
            nbt::nbt_compound_ptr compound = std::make_shared<nbt::nbt_compound>();
            e.second->save(compound);
            nbt_entities->value[e.first] = compound;
        }
        return std::shared_ptr<nbt::nbt>(nbt_entities);
    }

    void game_room::frame_handler(boost::system::error_code err) {
        if (game_loop_is_over)return;
        {
            std::lock_guard<std::mutex> guard(protect_game_state);
            profiler.start_tick();

            profiler.push("update entities");
            for (const auto &e:entities) {
                e.second->update();
            }
            profiler.pop();

            profiler.push("broadcast to clients");

            broadcast_to_all(nbt::nbt_compound::make({
                                                             {"entities", get_entity_list()},
                                                             {"chat",     nbt::nbt_string::make(queued_chat)}
                                                     }));

            profiler.pop();

            profiler.end_tick();
            //TODO: physics XD
            queued_chat = "";
        }
        //NO LOGIC OUTSIDE OF THIS
        int FPS = 30;
        timer.expires_at(timer.expires_at() + boost::posix_time::milliseconds((int) (1000 / FPS)));
        timer.async_wait([this](boost::system::error_code err) {
            frame_handler(err);
        });
    }

    game_room::~game_room() {
        profiler.print();
    }

    std::string game_room::get_next_entity_id() {
        std::stringstream str;
        str << boost::uuids::random_generator()();
        return str.str();
    }

    std::string game_room::spawn_entity(const std::function<entity::entity_ptr(const std::string &)> &create) {
        std::string id = get_next_entity_id();
        entities[id] = create(id);
        return id;
    }

    void game_room::join(const server_player_ptr &ptr) {
        std::lock_guard<std::mutex> guard(protect_game_state);
        std::cout << "game_room::join guard\n";
        ptr->send_world(world);
        std::string id = spawn_entity([&](const std::string &id) {
            return entity::entity_player::spawn(id, glm::vec3{24 + (rand() % 10000) / 10000.0F, 150,
                                                              24 + (rand() % 10000) / 10000.0F}, this);
        });
//        spawn_entity([&](const std::string &id) {
//            return entity::entity_zombie::spawn(id, glm::vec3{24 + (rand() % 10000) / 10000.0F, 150,
//                                                              24 + (rand() % 10000) / 10000.0F}, this);
//        });
        ptr->entity_id = id;
        ptr->deliver(nbt::nbt_compound::make({
                                                     {"player_id", nbt::nbt_string::make(id)},
                                                     {"entities",  get_entity_list()}
                                             }));
        players.insert(ptr);
        std::cout << "PLAYER " << ptr->entity_id << " JOINED\n";
        queued_chat = ptr->entity_id + " joined the game";
    }

    void game_room::handle_player_interaction_packet(const server_player_ptr &player,
                                                     const std::shared_ptr<nbt::nbt> &data) {
        std::lock_guard<std::mutex> guard(protect_game_state);
        entity::entity_ptr ent = entities[player->entity_id];
        {
            std::shared_ptr<nbt::nbt> movement = data->compound_ref()["movement"];
            bool left = movement->compound_ref()["left"]->as_short();
            bool right = movement->compound_ref()["right"]->as_short();
            bool front = movement->compound_ref()["front"]->as_short();
            bool back = movement->compound_ref()["back"]->as_short();
            bool sprint = movement->compound_ref()["sprint"]->as_short();
            bool jump = movement->compound_ref()["jump"]->as_short();

            float d = 1;
            glm::vec3 leftdir = glm::cross(ent->lookdir, glm::vec3{0, -1, 0});
            leftdir.y = 0;
            leftdir = glm::normalize(leftdir);
            glm::vec3 forward = ent->lookdir;
            forward.y = 0;
            forward = glm::normalize(forward);
            ent->motion = glm::vec3{0};
            if (front)ent->motion += forward * d * (float) (1 + sprint);
            if (back)ent->motion -= forward * d;
            if (left)ent->motion += leftdir * d;
            if (right)ent->motion -= leftdir * d;
            if (jump && ent->grounded) {
                ent->velocity.y = 0;
                ent->velocity.y += 150;//TODO: && entity.is grounded
                ent->grounded = false;
            }
        }
        {
            ent->lookdir = utils::cast3(data->compound_ref()["lookdir"]);
        }
        {
            std::string chat = data->compound_ref()["chat"]->as_string();
            //TODO: commands will be parsed here
            if (!chat.empty())queued_chat = "<" + player->entity_id + "> " + chat;
        }
    }

    void game_room::kill_entity(const std::string &id) {
        //TODO: animation
        entities.erase(id);
    }

    void game_room::leave(const server_player_ptr &ptr) {
        players.erase(ptr);
        kill_entity(ptr->entity_id);
        std::cout << "PLAYER " << ptr->entity_id << " LEFT\n";
        queued_chat = ptr->entity_id + " left the game";
    }

    void game_room::broadcast_to_all(const std::shared_ptr<nbt::nbt> &msg) {
        for (const auto &p:players)p->deliver(msg);
    }

}