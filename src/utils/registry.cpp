//
// Created by paladin on 11/8/19.
//

#include <iostream>
#include "registry.h"
#include "jsonformat.h"

void test_registry() {
    registry<std::string, int> reg{};
    for (char a = 'a'; a <= 'z'; a++) {
        reg.put(std::string() + a, a);
    }
    std::cout << reg << std::endl;
}

template<class K, class V>
void registry<K, V>::export_(std::ostream stream) {
    stream << *this;
}

template<class K, class V>
void registry<K, V>::put(K k, V v) {
    this->insert_or_assign(k, v);
}

template<class K, class V>
std::ostream& operator<<(std::ostream& os, registry<K, V> const& dt) {
    os << "{";
    int counter = 0; // hack to avoid iterator manipulation
    for (std::pair<K, V> val : dt) {
        counter++;
        os << json::json_format(val.first);

        os << ": ";

        os << json::json_format(val.second);
        if (counter != dt.size())
            os << ", ";
    }
    os << "}";
    return os;
}
