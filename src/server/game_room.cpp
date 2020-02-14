//
// Created by Jack Armstrong on 1/17/20.
//

#include <boost/thread.hpp>
#include <utility>
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
        tick_number = 0;
        instance = this;
        game_loop_is_over = false;
        world.generate_world();
        std::cout << "start tick\n";
        tick();
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

    void game_room::tick() {
        if (game_loop_is_over)return;
        {
            std::lock_guard<std::mutex> guard(protect_game_state);
            profiler.start_tick();

            tick_number++;

            //delayed tasks will do their own profiler magik
            while (!tasks.empty() && tasks.top()->target_tick <= tick_number) {
                delayed_task *task = tasks.top();
                tasks.pop();
                task->execute();
                delete task;
            }

            profiler.push("update entities");
            for (const auto &e:entities) {
                e.second->update();
            }
            profiler.pop();

            profiler.push("broadcast to clients");

            nbt::nbt_ptr nbt_world_ops = nbt::nbt_list::make({});
            while (!world_ops.empty()) {
                block::world_op op = world_ops.top();
                world_ops.pop();
                world.apply(op);
                nbt::nbt_compound_ptr cmpnd = std::make_shared<nbt::nbt_compound>();
                op.save(cmpnd);
                nbt_world_ops->list_ref().push_back(cmpnd);
            }

            broadcast_to_all(nbt::nbt_compound::make({
                                                             {"entities",  get_entity_list()},
                                                             {"chat",      nbt::nbt_string::make(queued_chat)},
                                                             {"world_ops", nbt_world_ops}
                                                     }));

            profiler.pop();

            profiler.end_tick();

            queued_chat = "";
        }
        //NO LOGIC OUTSIDE OF THIS
        int FPS = 30;
        timer.expires_at(timer.expires_at() + boost::posix_time::milliseconds((int) (1000 / FPS)));
        timer.async_wait([this](boost::system::error_code err) {
            tick();
        });
    }

    game_room::~game_room() {
        profiler.print();
        while (tasks.size() > 0) {
            delayed_task *t = tasks.top();
            tasks.pop();
            delete t;
        }
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
        std::shared_ptr<entity::entity_player> ent = std::dynamic_pointer_cast<entity::entity_player>(
                entities[player->entity_id]);
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
            ent->motion *= 3;
            if (jump && ent->grounded) {
                ent->jump();
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
        {
            int s = data->compound_ref()["new_selected_pos"]->as_int();
            if (s != -1) {
                ent->selected_item = s <= -1 ? 0 : (s >= 9 ? 8 : s);
            }
        }
        {
            bool leftclick = data->compound_ref()["leftclick"]->as_short();
            bool rightclick = data->compound_ref()["rightclick"]->as_short();
            if (leftclick && ent->leftclick)
                schedule(-10, new entity::task_player_click_interact(ent.get(),
                                                                     entity::LEFT_CONTINUE));
            if (leftclick && !ent->leftclick)
                schedule(-10, new entity::task_player_click_interact(ent.get(),
                                                                     entity::LEFT_START));
            if (!leftclick && ent->leftclick)
                schedule(-10, new entity::task_player_click_interact(ent.get(),
                                                                     entity::LEFT_END));
            if (rightclick && ent->rightclick)
                schedule(-10, new entity::task_player_click_interact(ent.get(),
                                                                     entity::RIGHT_CONTINUE));
            if (rightclick && !ent->rightclick)
                schedule(-10, new entity::task_player_click_interact(ent.get(),
                                                                     entity::RIGHT_START));
            if (!rightclick && ent->rightclick)
                schedule(-10, new entity::task_player_click_interact(ent.get(),
                                                                     entity::RIGHT_END));
            ent->leftclick = leftclick;
            ent->rightclick = rightclick;
        }
        {
            ent->firstperson = data->compound_ref()["firstperson"]->as_short();
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

    void game_room::schedule(int delay, delayed_task *task) {
        task->target_tick = tick_number + delay;
        tasks.push(task);
    }

}