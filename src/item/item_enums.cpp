//
// Created by Jack Armstrong on 2/1/20.
//

#include "item_enums.h"

namespace item::enums {

    material material::WOOD = {client::atlas_texture::WOOD_SWORD, "wood"};
    material material::STONE = {client::atlas_texture::STONE_SWORD, "stone"};
    material material::IRON = {client::atlas_texture::IRON_SWORD, "iron"};
    material material::GOLD = {client::atlas_texture::GOLD_SWORD, "gold"};
    material material::DIAMOND = {client::atlas_texture::DIAMOND_SWORD, "diamond"};

}