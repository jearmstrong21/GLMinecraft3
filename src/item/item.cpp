//
// Created by Jack Armstrong on 1/27/20.
//

#include "item.h"

#include <utility>

namespace item {

    void item_properties::save(const nbt::nbt_compound_ptr &tag) {
        tag->value["name"]=nbt::nbt_string::make(name);
    }

    void item_properties::load(const nbt::nbt_compound_ptr &tag) {
        name=tag->value["name"]->as_string();
    }

    bool item_properties::operator==(const item_properties &other) {
        return name==other.name;
    }

    item::item(item_properties properties): properties(std::move(properties)) {

    }

    void item::save(item_stack* stack, const nbt::nbt_compound_ptr &tag) {
        tag->value["item_name"]=nbt::nbt_string::make(stack->item_name);
        tag->value["count"]=nbt::nbt_int::make(stack->count);
    }

    void item::load(item_stack*stack, const nbt::nbt_compound_ptr &tag) {
        stack->item_name=tag->value["item_name"]->as_string();
        stack->count=tag->value["count"]->as_int();
    }

    void item_registry::save(const nbt::nbt_compound_ptr &tag) {
        for(const auto&p:map){
            tag->value[p.first]=nbt::nbt_compound::make({});
            p.second->properties.save(nbt::cast_compound(tag->value[p.first]));
        }
    }

    bool item_registry::is_equivalent(const nbt::nbt_compound_ptr &tag) {
        for(const auto&p:tag->value){
            if(map.find(p.first)==map.end()){
                return false;
            }
            item_properties prop;
            prop.load(nbt::cast_compound(p.second));
            if(!(map[p.first]->properties==prop))return false;
        }
        for(const auto&p:map){
            if(tag->value.find(p.first)==tag->value.end())return false;
        }
        return true;
    }

    void item_registry::put(const std::shared_ptr<item>&item) {
        map[item->properties.name]=item;
    }

    std::map<std::string,std::shared_ptr<item>> item_registry::map;

}