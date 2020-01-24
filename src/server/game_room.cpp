//
// Created by Jack Armstrong on 1/17/20.
//

#include "game_room.h"

namespace server {

    game_room* game_room::instance;

    game_room::game_room(boost::asio::io_context &io_context) : timer(io_context,
                                                                      boost::posix_time::milliseconds(0)){
        instance=this;
        game_loop_is_over=false;
        world.generate_world();
        frame_handler(boost::system::error_code());
//        std::cout<<"\npress RETURN to exit\n";
//        std::string s;
//        std::getline(std::cin,s);
//        game_loop_is_over=true;
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
        if(game_loop_is_over)return;
        {
            std::lock_guard<std::mutex> guard(protect_game_state);

//                for (const auto &e:entities) {
//                    get_type(e.second)->update(e.second, this);
//                }

            broadcast_to_all(nbt::nbt_compound::make({
                                                             {"entities", get_entity_list()},
                                                             {"chat",     nbt::nbt_string::make(queued_chat)}
                                                     }));

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

    game_room::~game_room(){

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
        std::cout<<"game_room::join guard\n";
        ptr->send_world(world);
        std::string id = spawn_entity([](const std::string &id) {
            return std::dynamic_pointer_cast<entity::entity>(
                    std::make_shared<entity::entity_player>(id, glm::vec3{24, 150, 24}));
        });
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
            glm::vec3 newMotion{0, 0, 0};
            glm::vec3 curLook = ent->lookdir;
            glm::vec3 curVel = ent->velocity;
            glm::vec3 leftdir = glm::cross(curLook, glm::vec3{0, -1, 0});
            leftdir.y = 0;
            leftdir = glm::normalize(leftdir);
            glm::vec3 forward = curLook;
            forward.y = 0;
            forward = glm::normalize(forward);
            if (front)newMotion += forward * d * (float) (1 + sprint);
            if (back)newMotion -= forward * d;
            if (left)newMotion += leftdir * d;
            if (right)newMotion -= leftdir * d;
            if (jump)curVel.y += 2.5;//TODO: && entity.is grounded
            ent->motion = newMotion;
            ent->velocity = curVel;
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