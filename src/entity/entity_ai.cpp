//
// Created by Jack Armstrong on 2/22/20.
//

#include "entity_ai.h"
#include <unordered_set>
#include <unordered_map>
#include <server/game_room.h>
#include <entity/entity.h>
#include <glm/gtx/hash.hpp>

namespace entity::ai {

    glm::vec3 get_motion(entity *agent, glm::vec3 fgoal) {
        glm::vec3 motion{0, 0, 0};

        glm::ivec3 start{agent->box.pos};
        glm::ivec3 goal{fgoal};

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
            if (i > 100) {
                return glm::vec3{0, 0, 0};
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
                        glm::vec3(total_path[2]) + glm::vec3{0.5} - agent->box.pos);//TODO: getattribute(walkspeed)
                return motion;
            }

            open_set.erase(current);

            std::unordered_set<glm::ivec3> children;
            if (!agent->can_go_through_block(
                    server::game_room::instance->world.get(current.x, current.y - 1, current.z))) {
                children.insert({current.x, current.y + 1, current.z});
            } else {
                children.insert({current.x - 1, current.y, current.z});
                children.insert({current.x + 1, current.y, current.z});
                children.insert({current.x, current.y, current.z - 1});
                children.insert({current.x, current.y, current.z + 1});
                children.insert({current.x, current.y - 1, current.z});
            }
            for (glm::ivec3 child:children) {
                if (!agent->collides_with_block_at(glm::vec3{child} + glm::vec3{0.5})) {
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
        return {0, 0, 0};
    }

}