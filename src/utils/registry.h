//
// Created by paladin on 11/8/19.
//

#ifndef GLMINECRAFT3_REGISTRY_H
#define GLMINECRAFT3_REGISTRY_H

#include <map>
#include <string>
#include <ostream>

void test_registry();

template<class K, class V>
class registry : public std::map<K, V> {
public:
    void export_(std::ostream stream);

    void put(K k, V v);

    template<class Q, class R>
    friend std::ostream &operator<<(std::ostream &os, const registry<Q, R> &dt);
};


#endif //GLMINECRAFT3_REGISTRY_H
