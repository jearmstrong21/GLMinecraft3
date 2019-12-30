//
// Created by paladin on 12/30/19.
//

#ifndef GLMINECRAFT3_BIOME_H
#define GLMINECRAFT3_BIOME_H

#include "world/gen/features/base_feature.h"
#include "world/gen/carver/base_carver.h"
#include "world/gen/surface/base_surface.h"
#include "world/gen/structures/base_structure.h"
#include <memory>
#include <vector>

namespace world::gen {
        struct biome {
            double heat;
            double humidity;

            double min_height;
            double noise_scale;
            double slope;

            std::shared_ptr<base_carver> carver;
            std::vector<std::shared_ptr<base_feature>> features;
            std::vector<std::shared_ptr<base_structure>> structures;
            std::shared_ptr<base_surface> surface;
        };
    }


#endif //GLMINECRAFT3_BIOME_H
