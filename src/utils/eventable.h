//
// Created by paladin on 12/19/19.
//

#ifndef GLMINECRAFT3_EVENTABLE_H
#define GLMINECRAFT3_EVENTABLE_H

#include<atomic>
#include<functional>
#include<map>
#include<memory>
#include<mutex>

template<class K>
struct eventable {
    void bind(K bind, void(*fn)(std::shared_ptr<eventable<K>>));

protected:
    void push(K bind);

    std::mutex _queue_mutex;
    std::multimap<K, void(std::shared_ptr<eventable<K>>)> queue;
};

#endif //GLMINECRAFT3_EVENTABLE_H
