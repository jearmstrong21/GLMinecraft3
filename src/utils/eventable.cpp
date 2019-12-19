//
// Created by paladin on 12/19/19.
//

#include <iostream>
#include "eventable.h"

template<class K>
void eventable<K>::bind(K bind, void(*fn)(std::shared_ptr<eventable<K>>)) {
    const std::lock_guard<std::mutex> guard(this->_queue_mutex);
    this->queue.insert(bind, fn);

    //TEST
    this->push(bind);
}

template<class K>
void eventable<K>::push(K bind) {
//    for (auto pair : this->queue.find(bind)) {
//        std::cout << typeid(pair).name();
//    }
}