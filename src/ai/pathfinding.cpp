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

namespace ai::path {

    path_options default_opt(){
        path_options opt;
        opt.ok_distance=1.0F;
        opt.max_search_nodes=3000;
        opt.cardinal_directions={{-1,0,0},{1,0,0},{0,0,-1},{0,0,1}};
        opt.can_fall=true;
        opt.can_jump=true;
        opt.node_dist=1.0F;
        return opt;
    }

    glm::vec3 pathfind_astar(allowed_at_pos_func allowed_at,glm::vec3 start,glm::vec3 end,const path_options& opt){
        std::vector<glm::vec3>edgeNodes;
        std::vector<glm::vec3>processedNodes;
        std::unordered_map<glm::vec3,float>pathLengths;
        std::unordered_map<glm::vec3,glm::vec3>pathOrigins;
        //TODO: should edgeNodes and processedNodes be set/set or priority q/set

//        printf("A\n");

        auto fround=[](float f){
            return (float)((int)(f*2))/(float)2.0F;
        };

        auto heuristic=[](glm::vec3 a,glm::vec3 b){
            return abs(a.x-b.x)+abs(a.y-b.y)+abs(a.z-b.z);
        };

        auto neighbors=[&allowed_at,opt](glm::vec3 v){
            std::vector<glm::vec3>l;
            for(glm::vec3 c:opt.cardinal_directions){
                if(allowed_at(v+c*opt.node_dist)){
                    l.push_back(v+c*opt.node_dist);
                }
            }
            if(allowed_at(v+glm::vec3{0,1,0})){//TODO: &&can_jump
                l.push_back(v+glm::vec3{0,1,0});
            }
            if(allowed_at(v+glm::vec3{0,-1,0})){//TODO: &&can_fall
                l.push_back(v+glm::vec3{0,-1,0});
            }
            return l;
        };

        start.x=fround(start.x);
        start.y=fround(start.y);
        start.z=fround(start.z);

        end.x=fround(end.x);
        end.y=fround(end.y);
        end.z=fround(end.z);

        edgeNodes.push_back(start);
        pathLengths[start]=0;
        pathOrigins[start]=start;

//        printf("B\n");
        int i=0;
        while(!edgeNodes.empty()){
//            printf("C%i\n",i);
            if(i>opt.max_search_nodes){
                printf("TOO MANY SEARCH NODES: %i\n",i);
                return {0,0,0};
            }
            i++;
            int bestIndex=0;
            for(int n=1;n<edgeNodes.size();n++){
                if(heuristic(end,edgeNodes[n])<heuristic(end,edgeNodes[bestIndex]))bestIndex=n;
            }
            glm::vec3 v=edgeNodes[0];
            edgeNodes.erase(edgeNodes.begin());
            if(std::find(processedNodes.begin(),processedNodes.end(),v)!=processedNodes.end())continue;
            processedNodes.push_back(v);
            if(heuristic(v,end)<=opt.ok_distance){
                break;
            }
//            printf("D\n");
            std::vector<glm::vec3>children=neighbors(v);
            for(glm::vec3 child:children){
                float tentative=pathLengths[v]+1;
                if(pathLengths.find(child)==pathLengths.end()||pathLengths[child]>tentative){
                    pathLengths[child]=tentative;
                    pathOrigins[child]=v;
                    if(std::find(edgeNodes.begin(),edgeNodes.end(),child)==edgeNodes.end())edgeNodes.push_back(child);
                }
            }
//            printf("E\n");
        }
//        printf("F\n");
        std::vector<glm::vec3>path;
        glm::vec3 v=end;
        while(true){
            if(pathOrigins.find(v)!=pathOrigins.end()){
                glm::vec3 orig=v;
                v=pathOrigins[v];
                if(orig==v)break;
            }else break;
            path.push_back(v);
        }
//        printf("G\n");
        std::reverse(path.begin(),path.end());
        printf("PATH.SIZE()=%ld\n",path.size());
        if(path.size()<2){
            return {0,0,0};
        }
        return glm::normalize(path[1]-path[0]);
    }

}