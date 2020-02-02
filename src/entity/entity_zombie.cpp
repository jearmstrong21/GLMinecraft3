//
// Created by Jack Armstrong on 1/26/20.
//

#include <server/game_room.h>
#include "entity_zombie.h"
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <glm/gtx/hash.hpp>

namespace entity {

    entity_zombie::entity_zombie() : entity(ENTITY_ID_ZOMBIE) {
        box.size = {0.6, 1.95, 0.6};
    }

    entity_ptr entity_zombie::spawn(std::string id, glm::vec3 pos, server::game_room *server) {
        std::shared_ptr<entity_zombie> p = std::make_shared<entity_zombie>();
        p->uuid = std::move(id);
        p->box.pos = pos;
        p->server = server;
        return std::dynamic_pointer_cast<entity>(p);
    }

    void entity_zombie::save_additional_information(const nbt::nbt_compound_ptr &tag) {

    }

    void entity_zombie::load_additional_information(const nbt::nbt_compound_ptr &tag) {

    }

    void entity_zombie::handle_ai() {
        entity_ptr player = nullptr;
        for (const auto &p:server->entities) {
            if (p.second->type_id == ENTITY_ID_PLAYER) {
                if (player == nullptr)player = p.second;
                if (glm::distance(box.pos, player->box.pos) < glm::distance(box.pos, p.second->box.pos)) {
                    player = p.second;
                }
            }
        }
        if (player == nullptr) {
            return;
        }

        motion = glm::vec3{0};

        glm::ivec3 start{box.pos};
        glm::ivec3 goal{player->box.pos};

        std::vector<glm::ivec3> total_path;

        auto h = [&](glm::ivec3 v) {
            return (v.x - start.x) * (v.x - start.x) + (v.y - start.y) * (v.y - start.y) +
                   (v.z - start.z) * (v.z - start.z);
        };
        auto g = [&](glm::ivec3 a, glm::ivec3 b) {
            return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
        };

        std::unordered_set<glm::ivec3> open_set;
        open_set.insert(start);
        std::unordered_map<glm::ivec3, glm::ivec3> came_from;
        std::unordered_map<glm::ivec3, int> gscore;//default infinite
        gscore[start] = 0;
        std::unordered_map<glm::ivec3, int> fscore;//default infinite
        fscore[start] = h(start);

        int i = 0;
        while (!open_set.empty()) {
            i++;
            if (i > 1000) {
                return;
            }
            glm::ivec3 current = *open_set.begin();
            for (glm::ivec3 v:open_set) {
                if (fscore.find(current) == fscore.end())continue;
                if (fscore[v] < fscore[current])current = v;
            }
            if (current == goal) {
                total_path.push_back(current);
                while (came_from.find(current) != came_from.end()) {
                    current = came_from[current];
                    total_path.push_back(current);
                }
                std::reverse(total_path.begin(), total_path.end());
                motion = 3.0F * glm::normalize(
                        glm::vec3(total_path[1]) + glm::vec3{0.5} - box.pos);//TODO: getattribute(walkspeed)
                if (motion.y > 0) {
                    jump();
                    motion.y = 0;
                }
                return;
            }

            open_set.erase(current);

            std::unordered_set<glm::ivec3> children;
            if (!can_go_through_block(server->world.get(current.x, current.y - 1, current.z))) {
                children.insert({current.x, current.y + 1, current.z});
            } else {
                children.insert({current.x - 1, current.y, current.z});
                children.insert({current.x + 1, current.y, current.z});
                children.insert({current.x, current.y, current.z - 1});
                children.insert({current.x, current.y, current.z + 1});
                children.insert({current.x, current.y - 1, current.z});
            }
            for (glm::ivec3 child:children) {
                if (!collides_with_block_at(glm::vec3{child} + glm::vec3{0.5})) {
                    int tentative_gscore = gscore[current] + g(current, child);
                    int child_gscore = 1000000000;
                    if (gscore.find(child) != gscore.end())child_gscore = gscore[child];
                    if (tentative_gscore < child_gscore) {
                        came_from[child] = current;
                        gscore[child] = tentative_gscore;
                        fscore[child] = gscore[child] + h(child);
                        open_set.insert(child);
                    }
                }
            }

        }

        if (motion.y > 0 && grounded) {
            velocity.y += 150;
            grounded = false;
        }
        motion.y = 0;

    }


}