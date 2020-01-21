//
// Created by Jack Armstrong on 1/20/20.
//

#ifndef GLMINECRAFT3_PROFILER_H
#define GLMINECRAFT3_PROFILER_H

#include <stack>
#include <utility>
#include <sstream>
#include <iomanip>
#include <vector>

namespace utils {

    struct profiler{
    private:
        typedef uint64_t time_t;

        time_t get_time(){
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        }

        struct type{
            std::string name;
            int depth;
            time_t start;
            time_t end;
        };

        std::stack<type*>stack;
        std::vector<type*>entries;
        std::string profile_name;

    public:
        profiler(std::string name);

        ~profiler();

        void push(const std::string& name="noname");

        void pop();

        void pop_all();

        void cleanup();

        void output(std::ostream& stream);

    };

}

#endif //GLMINECRAFT3_PROFILER_H
