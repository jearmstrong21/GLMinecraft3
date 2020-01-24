//
// Created by Jack Armstrong on 1/23/20.
//

#ifndef GLMINECRAFT3_PROFILER_H
#define GLMINECRAFT3_PROFILER_H

#include <ostream>
#include <vector>
#include <map>
#include <chrono>

namespace utils {

    class profiler {

    private:
        const char SEPERATOR='\u001e';//nobody will type this one out in a path-name so we can use it as a node delimiter in the sequence XD

        typedef std::chrono::nanoseconds time_t;

        time_t now(){
            return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch());
        }

        std::map<std::string,int>depth;
        std::map<std::string,time_t>times;
        int tick_count;
        std::stack<std::string>path_stack;
        std::stack<time_t>start_time_stack;

    public:

        profiler():tick_count(0){

        }

        void start_tick(){
            path_stack.push("tick");
            depth["tick"]=0;
            times["tick"]=time_t::zero();
            start_time_stack.push(now());
        }

        void end_tick(){
            path_stack.pop();
            start_time_stack.pop();
        }

        void push(const std::string& name){
            std::string path_o=path_stack.top();
            path_stack.push(path_stack.top()+SEPERATOR+name);
            start_time_stack.push(now());
            std::string path=path_stack.top();
            if(depth[path]<=depth[path_o])depth[path]=depth[path_o]+1;
        }

        void pop(){
            time_t end=now();
            time_t start=start_time_stack.top();
            start_time_stack.pop();
            std::string path=path_stack.top();
            path_stack.pop();
            if(times.find(path)==times.end())times[path]=time_t::zero();
            times[path]+=end-start;
        }

        void print(){
            std::cout<<"---------- PROFILER RESULTS ----------\n";
            for(const auto& p:times){
                std::string path=p.first;
                for(int i=0;i<path.size();i++){
                    if(path[i]==SEPERATOR)path[i]='.';
                }
                std::cout<<depth[p.first]<<" "<<path<<" "<<times[p.first].count()<<"ns\n";
            }
            std::cout<<"----------                  ----------\n";
        }

    };

}

#endif //GLMINECRAFT3_PROFILER_H
