//
// Created by Jack Armstrong on 11/26/19.
//

#include <iostream>
#include "entity.h"
#include "server/game_room.h"

namespace server {

    std::shared_ptr<nbt::nbt> entity_type_base::initialize() const {
        return nbt::make_compound({
            {"id",nbt::make_string("well this is awkward")},
            {"position",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})},
            {"motion",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})},
            {"lookdir",nbt::make_list({nbt::make_float(0),nbt::make_float(0),nbt::make_float(0)})}
            //todo health
        });
    }

    void entity_type_base::update(std::shared_ptr<nbt::nbt> data, server::game_room *room) const {
        std::shared_ptr<nbt::nbt_compound>compound=nbt::cast_compound(data);
        glm::vec3 curPos{nbt::cast_float(nbt::cast_list(compound->value["position"])->value[0])->value,nbt::cast_float(nbt::cast_list(compound->value["position"])->value[1])->value,nbt::cast_float(nbt::cast_list(compound->value["position"])->value[2])->value};
        glm::vec3 curMotion{nbt::cast_float(nbt::cast_list(compound->value["motion"])->value[0])->value,nbt::cast_float(nbt::cast_list(compound->value["motion"])->value[1])->value,nbt::cast_float(nbt::cast_list(compound->value["motion"])->value[2])->value};

        glm::vec3 bbSize={1,1,1};
        glm::vec3 bbMin=curPos-bbSize/2.0F;
        glm::vec3 bbMax=curPos+bbSize/2.0F;

        auto is_allowed_position=[&](glm::vec3 epos){

            int x0=(int)(epos.x-bbSize.x/2.0F);
            int y0=(int)(epos.y-bbSize.y/2.0F);
            int z0=(int)(epos.z-bbSize.z/2.0F);

            int x1=(int)(epos.x+bbSize.x/2.0F);
            int y1=(int)(epos.y+bbSize.y/2.0F);
            int z1=(int)(epos.z+bbSize.z/2.0F);

            for(int x=x0;x<=x1;x++){
                for(int y=y0;y<=y1;y++){
                    for(int z=z0;z<=z1;z++){
                        if(x<0||y<0||z<0||x>=WORLD_SIZE*16||y>=256||z>=WORLD_SIZE*16)continue;
                        if(room->world.get(x,y,z)!=0){
                            printf("oops %i %i %i = %li\n",x,y,z,room->world.get(x,y,z));
                            return false;
                        }
                    }
                }
            }

            return true;
        };

        float dt=0.1F;

        glm::bvec3 allowDir{false};
        if(is_allowed_position(curPos+glm::vec3{curMotion.x*dt,0,0}))allowDir.x=true;
        if(is_allowed_position(curPos+glm::vec3{0,curMotion.y*dt,0}))allowDir.y=true;
        if(is_allowed_position(curPos+glm::vec3{0,0,curMotion.z*dt}))allowDir.z=true;

        if(!allowDir.x)curMotion.x=0;
        if(!allowDir.y)curMotion.y=0;
        if(!allowDir.z)curMotion.z=0;

        std::cout<<allowDir.x<<","<<allowDir.y<<","<<allowDir.z<<"\n";

        curPos+=curMotion*dt;

        compound->value["position"]=nbt::make_list({nbt::make_float(curPos.x),nbt::make_float(curPos.y),nbt::make_float(curPos.z)});
        compound->value["motion"]=nbt::make_list({nbt::make_float(curMotion.x),nbt::make_float(curMotion.y),nbt::make_float(curMotion.z)});


    }

    std::shared_ptr<nbt::nbt> entity_type_player::initialize() const {
        std::shared_ptr<nbt::nbt>res=entity_type_base::initialize();
        nbt::merge(res,nbt::make_compound({
            //nothing to see here move along
        }));
        return res;
    }

    void entity_type_player::update(std::shared_ptr<nbt::nbt> data, game_room *room) const {
        std::cout<<"update player\n";
    }

}