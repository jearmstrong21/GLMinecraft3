//
// Created by paladin on 1/17/20.
//

#ifndef GLMINECRAFT3_ITEM_H
#define GLMINECRAFT3_ITEM_H


#include <nbt/nbt.h>

namespace server { struct game_room; }
namespace entity { struct entity; }

namespace item {

    struct item;

    struct item_stack{
        std::string item_name;
        int count;
        item*item;
    };

    struct item_use_context {

        item_stack stack;
        std::shared_ptr<entity::entity>source;
        server::game_room*room;
        //TODO: std::vector<item_attribute>attributes

    };

    struct item_properties {

        std::string name;

        void save(const nbt::nbt_compound_ptr&tag);
        void load(const nbt::nbt_compound_ptr&tag);

        bool operator==(const item_properties&other);

    };

    struct item{

        item_properties properties;

        explicit item(item_properties properties);//TODO: int id, item_props props, etc

        virtual void attack(const item_use_context&ctx)=0;
        virtual void use(const item_use_context&ctx)=0;

        virtual void save_additional_information(item_stack stack,const nbt::nbt_compound_ptr &tag) = 0;

        virtual void load_additional_information(item_stack stack,const nbt::nbt_compound_ptr &tag) = 0;

        void save(item_stack* stack,const nbt::nbt_compound_ptr &tag);

        void load(item_stack* stack,const nbt::nbt_compound_ptr &tag);

        virtual item_stack make()=0;

    };

    struct item_registry {

        static void save(const nbt::nbt_compound_ptr &tag);

        bool is_equivalent(const nbt::nbt_compound_ptr &tag);

        static std::map<std::string,std::shared_ptr<item>>map;

        static void put(const std::shared_ptr<item>&item);

    };

}


#endif //GLMINECRAFT3_ITEM_H
