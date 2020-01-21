//
// Created by Jack Armstrong on 1/20/20.
//

#include <iostream>
#include <utility>
#include "profiler.h"

namespace utils {

    profiler::profiler(std::string name):profile_name(std::move(name)){

    }

    profiler::~profiler() = default;

    void profiler::push(const std::string& name) {
        type*entry=new type();
        entry->start=get_time();
        entry->name=name;
        entry->depth=0;
        if(!stack.empty())entry->depth=stack.top()->depth+1;
        entries.push_back(entry);
        stack.push(entry);
    }

    void profiler::pop(){
        if(stack.empty())return;
        type*entry=stack.top();
        stack.pop();
        entry->end=get_time();
        if(!stack.empty())stack.top()->end=get_time();
    }

    void profiler::cleanup(){
        for(int i=0;i<entries.size();i++){
            delete entries[i];
        }
    }

    void profiler::pop_all() {
        while(!stack.empty())pop();
    }

    void profiler::output(std::ostream &stream){
        stream<<"----- profile dump: "<<profile_name<<" -----\n";
        for(int i=0;i<entries.size();i++){
            stream<<"["<<std::setw(2)<<entries[i]->depth<<"] ";
            for(int _=0;_<entries[i]->depth;_++)stream<<"|   ";
            stream<<entries[i]->name<<": "<<entries[i]->end-entries[i]->start<<"ms\n";
        }
        stream<<"-----               ";
        for(int i=0;i<profile_name.size();i++)stream<<" ";
        stream<<" -----\n";
    }

}