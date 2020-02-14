//
// Created by Jack Armstrong on 2/14/20.
//

#include "delayed_task.h"

namespace server {

    bool operator<(const delayed_task &a, const delayed_task &b) {
        return a.target_tick > b.target_tick;
    }

}