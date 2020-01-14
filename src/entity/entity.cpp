//
// Created by Jack Armstrong on 11/26/19.
//

#include <iostream>
#include "ai/pathfinding.h"
#include "entity.h"
#include "server/game_room.h"

namespace server {

    std::shared_ptr<nbt::nbt> entity_type_base::initialize() const {
        return nbt::make_compound({
                                          {"id",             nbt::make_string("well this is awkward")},
                                          {"entity_type_id", nbt::make_int(-1)},
                                          {"position",       utils::cast3(glm::vec3{0, 0, 0})},
                                          {"motion",         utils::cast3(glm::vec3{0, 0, 0})},
                                          {"lookdir",        utils::cast3(glm::vec3{0, 0, 0})},
                                          {"bbsize",         utils::cast3(glm::vec3{0, 0, 0})},
                                          {"velocity",       utils::cast3(glm::vec3{0, 0, 0})}
//                                          todo health
                                  });
    }

    bool entity_type_base::is_allowed_at_position(const std::shared_ptr<nbt::nbt> &data, glm::vec3 epos,
                                                  server::game_room *room) const {
        glm::vec3 bbSize = utils::cast3(data->compound_ref()["bbsize"]);

        int x0 = (int) (epos.x - bbSize.x / 2.0F);
        int y0 = (int) (epos.y - bbSize.y / 2.0F);
        int z0 = (int) (epos.z - bbSize.z / 2.0F);

        int x1 = (int) (epos.x + bbSize.x / 2.0F);
        int y1 = (int) (epos.y + bbSize.y / 2.0F);
        int z1 = (int) (epos.z + bbSize.z / 2.0F);

        for (int x = x0; x <= x1; x++) {
            for (int y = y0; y <= y1; y++) {
                for (int z = z0; z <= z1; z++) {
                    if (x < 0 || y < 0 || z < 0 || x >= WORLD_SIZE * 16 || y >= 256 || z >= WORLD_SIZE * 16)
                        continue;
                    if (room->world.get(x, y, z) != 0) {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    void entity_type_base::update(std::shared_ptr<nbt::nbt> data, server::game_room *room) const {
        glm::vec3 curPos = utils::cast3(data->compound_ref()["position"]);
        glm::vec3 curMotion = utils::cast3(data->compound_ref()["motion"]);
        glm::vec3 curVel = utils::cast3(data->compound_ref()["velocity"]);
        glm::vec3 bbSize = utils::cast3(data->compound_ref()["bbsize"]);

        glm::vec3 bbMin = curPos - bbSize / 2.0F;
        glm::vec3 bbMax = curPos + bbSize / 2.0F;

        float dt = 0.1F;

        auto fixdir = [&](glm::vec3 &dir) {
            glm::bvec3 allowDir{false};

            if (is_allowed_at_position(data, curPos + glm::vec3{dir.x * dt, 0, 0}, room))allowDir.x = true;
            if (is_allowed_at_position(data, curPos + glm::vec3{0, dir.y * dt, 0}, room))allowDir.y = true;
            if (is_allowed_at_position(data, curPos + glm::vec3{0, 0, dir.z * dt}, room))allowDir.z = true;

            if (!allowDir.x)dir.x = 0;
            if (!allowDir.y)dir.y = 0;
            if (!allowDir.z)dir.z = 0;
        };

        curVel.y -= dt * 4;
        fixdir(curVel);
        fixdir(curMotion);

        curPos += curMotion * dt;
        curPos += curVel * dt;

        data->compound_ref()["position"] = utils::cast3(curPos);
        data->compound_ref()["motion"] = utils::cast3(curMotion);
        data->compound_ref()["velocity"] = utils::cast3(curVel);
    }

    std::shared_ptr<nbt::nbt> entity_type_player::initialize() const {
        std::shared_ptr<nbt::nbt> res = entity_type_base::initialize();
        res->compound_ref()["entity_type_id"]=nbt::make_int(1);
        res->compound_ref()["bbsize"]=utils::cast3({0.6,1.5,0.6});
        return res;
    }

    void entity_type_player::update(std::shared_ptr<nbt::nbt> data, game_room *room) const {
        entity_type_base::update(data, room);
    }

    std::shared_ptr<nbt::nbt> entity_type_zombie::initialize() const {
        std::shared_ptr<nbt::nbt> res = entity_type_base::initialize();
        res->compound_ref()["entity_type_id"]=nbt::make_int(2);
        res->compound_ref()["bbsize"]=utils::cast3({0.6,1.95,0.6});
        return res;
    }

    void entity_type_zombie::update(std::shared_ptr<nbt::nbt> data, game_room *room) const {
        glm::vec3 target{-1, -1, -1};
        for (auto p:room->entities) {
            if(p.second->compound_ref()["entity_type_id"]->as_int()==1){
                target=utils::cast3(p.second->compound_ref()["position"]);
            }
        }
        entity_type_base::update(data, room);

        if (target == glm::vec3{-1, -1, -1})return;

        glm::vec3 curPos=utils::cast3(data->compound_ref()["position"]);
        glm::vec3 bestMotion=ai::path::pathfind_astar([this,data,room](glm::vec3 v){
            return is_allowed_at_position(data,v,room);
        },curPos,target,ai::path::default_opt());
        printf("BESTMOTION %f,%f,%f\n",bestMotion.x,bestMotion.y,bestMotion.z);
        bestMotion.y=0.1F;
    //        glm::vec3 bestMotion = glm::normalize(target - curPos);
//
//        float jumpVel = 0;
//        if (bestMotion.y > 0 && !is_allowed_at_position(data, curPos - glm::vec3{0, 1.0F / 30.0F, 0}, room))jumpVel = 5;
//        bestMotion.y = 0;

        data->compound_ref()["motion"]=utils::cast3(bestMotion);
//        data->compound_ref()["velocity"]->list_ref()[1]->float_ref()+=jumpVel;

    }
}