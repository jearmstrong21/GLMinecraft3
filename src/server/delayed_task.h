//
// Created by Jack Armstrong on 2/14/20.
//

#ifndef GLMINECRAFT3_DELAYED_TASK_H
#define GLMINECRAFT3_DELAYED_TASK_H

namespace server {

    struct delayed_task {
        int target_tick = 0;

        virtual ~delayed_task() = default;

        virtual void execute() = 0;
    };

    bool operator<(const delayed_task &a, const delayed_task &b);

}

#endif //GLMINECRAFT3_DELAYED_TASK_H
