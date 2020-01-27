//
// Created by Jack Armstrong on 1/16/20.
//

#ifndef GLMINECRAFT3_ENTITY_H
#define GLMINECRAFT3_ENTITY_H

#include <utility>

#include "aabb.h"
#include "nbt/nbt.h"
#include "entity_id.h"
#include "utils/utils.h"
#include <memory>
#include <world/world.h>

namespace entity {

    struct entity {

        aabb box{};
        glm::vec3 velocity{};
        glm::vec3 motion{};
        std::string id;
        glm::vec3 lookdir{};
        block::world*world;

        explicit entity(std::string id,block::world*w) : id(std::move(id)),world(w) {

        }

        virtual void save_additional_information(const nbt::nbt_compound_ptr &tag) = 0;

        virtual void load_additional_information(const nbt::nbt_compound_ptr &tag) = 0;

        virtual bool can_go_through_block(block::block_state state){
            return state==0;
        }

        void save(const nbt::nbt_compound_ptr &tag) {
            tag->value["id"] = nbt::nbt_string::make(id);
            tag->value["position"] = utils::cast3(box.pos);
            tag->value["bbsize"] = utils::cast3(box.size);
            tag->value["motion"] = utils::cast3(motion);
            tag->value["lookdir"] = utils::cast3(lookdir);
            tag->value["velocity"] = utils::cast3(velocity);
            save_additional_information(tag);
        }

        void load(const nbt::nbt_compound_ptr &tag) {
            id = tag->value["id"]->as_string();
            box.pos = utils::cast3(tag->value["position"]);
            box.size = utils::cast3(tag->value["bbsize"]);
            motion = utils::cast3(tag->value["motion"]);
            lookdir = utils::cast3(tag->value["lookdir"]);
            velocity = utils::cast3(tag->value["velocity"]);
            load_additional_information(tag);
        }

        TRANSIENT std::chrono::nanoseconds last_time=std::chrono::nanoseconds::zero();
        TRANSIENT bool first_update=true;
        TRANSIENT bool grounded=false;

        void update(){
            float dt=0;
            std::chrono::nanoseconds cur_time=std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
            if(first_update){
                first_update=false;
            }else{
                dt=(float)(cur_time-last_time).count()/1000000000.0f;
            }
            last_time=cur_time;
            velocity.y-=500*dt;
            motion+=velocity*dt;
            auto check_pos=[&](glm::vec3 pos)->bool{
                bool collide=false;
                aabb{pos,box.size}.foreach([&](glm::ivec3 i){
                    if(!can_go_through_block(world->get(i)))collide=true;
                });
                return !collide;
            };
            auto trace_pos=[&](glm::vec3 start,glm::vec3 dir,float cur_dt,int iters)->glm::vec3{
                for(int _=0;_<iters;_++){
                    if(check_pos(start+dir*cur_dt))start+=dir*cur_dt;
                    cur_dt*=0.5;
                }
                return start;
            };

            int iters=10;
            if(motion.y!=0){
                glm::vec3 orig=box.pos;
                box.pos=trace_pos(box.pos,{0,motion.y,0},dt,iters);
                if(orig==box.pos)velocity.y=0;
                grounded=orig==box.pos;
            }
//            std::cout<<grounded<<"\n";
            if(motion.x!=0){
                box.pos=trace_pos(box.pos,{motion.x,0,0},dt,iters);
            }
            if(motion.z!=0){
                box.pos=trace_pos(box.pos,{0,0,motion.z},dt,iters);
            }

//            std::cout<<collide_x<<collide_y<<collide_z<<"\n";
        }

    };

    struct entity_player : entity {

        entity_player(std::string id, block::world*w,glm::vec3 spawnpos) : entity(std::move(id),w) {
            box.pos = spawnpos;
            box.size = glm::vec3{0.6, 1.5, 0.6};
        }

        void save_additional_information(const nbt::nbt_compound_ptr &tag) override {
            tag->value["entity_type_id"] = nbt::nbt_int::make(ENTITY_ID_PLAYER);
        }

        void load_additional_information(const nbt::nbt_compound_ptr &tag) override {
            ASSERT_OR_EXIT(tag->value["entity_type_id"]->as_int() == ENTITY_ID_PLAYER,
                           "Entity type id as loaded by entity_player was incorrect.");
        }

    };

    typedef std::shared_ptr<entity> entity_ptr;

}

#endif //GLMINECRAFT3_ENTITY_H
