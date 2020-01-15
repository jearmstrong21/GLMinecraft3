//
// Created by Jack Armstrong on 1/13/20.
//

#include "pathfinding.h"
#include <utility>
#include <map>
#include <algorithm>
#include <random>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <unordered_map>
#include <set>
#include <unordered_set>

namespace ai::path {

    path_options default_opt(){
        path_options opt{};
        opt.max_nodes=1000;
        return opt;
    }

    glm::vec3 pathfind_astar(const allowed_at_pos_func& allowed_at,glm::vec3 _start,glm::vec3 _end,const path_options& opt){
        glm::ivec3 start{_start};
        glm::ivec3 end{_end};

//        printf("PATHFIND_ASTAR:\n\t%i,%i,%i to %i,%i,%i\n",start.x,start.y,start.z,end.x,end.y,end.z);

        auto heuristic=[](glm::ivec3 a,glm::ivec3 b){
            return abs(a.x-b.x)+abs(a.y-b.y)+abs(a.z-b.z);
        };

        std::unordered_set<glm::ivec3>open;
        std::unordered_set<glm::ivec3>closed;
        std::unordered_map<glm::ivec3,glm::ivec3>previous;
        std::unordered_map<glm::ivec3,int>len;
        open.insert(start);
        previous[start]=start;
        len[start]=0;

        int i=0;
        while(!open.empty()){
            i++;
            if(i>opt.max_nodes){
//                printf("\tTOOMANYNODES\n");
                break;
            }
            glm::ivec3 v=*open.begin();
            for(glm::ivec3 t:open){
                if(heuristic(v,end)>heuristic(t,end))v=t;
            }
//            printf("\t%i %i %i\n",v.x,v.y,v.z);
            open.erase(v);
            closed.insert(v);
            if(v==end){
//                printf("\tFOUND END\n");
                break;
            }
            std::unordered_set<glm::ivec3>children;
            children.insert({v.x-1,v.y,v.z});
            children.insert({v.x+1,v.y,v.z});
            children.insert({v.x,v.y-1,v.z});//TODO delet sucky hax
            children.insert({v.x,v.y+1,v.z});//TODO delet sucky hax
            children.insert({v.x,v.y,v.z-1});
            children.insert({v.x,v.y,v.z+1});
            for(glm::ivec3 child:children){
                if(closed.find(child)!=closed.end())continue;
                if(len.find(child)==len.end()||len[child]>len[v]+1){
                  len[child]=len[v]+1;
                    previous[child]=v;
                    open.insert(child);
                }
            }
        }

        std::vector<glm::ivec3>path;
        glm::ivec3 v=end;
        while(true){
            if(previous.find(v)==previous.end())continue;
            glm::ivec3 orig=v;
            v=previous[v];
            if(orig==v)break;//found start
            path.push_back(v);
        }
        if(path.size()<2)return glm::vec3{0};
        std::reverse(path.begin(),path.end());
        return glm::normalize(glm::vec3(path[1]-path[0]));
    }

}