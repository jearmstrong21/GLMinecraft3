//
// Created by paladin on 12/29/19.
//

#include <memory>
#include "block/blocks.h"
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <server/game_room.h>
#include "utils/noise/noise.h"
#include "utils/utils.h"
#include "world_generator.h"
#include "../world.h"
#include "world/gen/surface/simple_grass_surface.h"
#include "world/gen/carver/simple_carver.h"

#include "thirdparty/anl.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;

namespace block {
    anl::TArray3D<anl::SRGBA> generate_data(bool nether) {
        std::cout << "running test" << std::endl;

        // terraintree=
        //{
        //	{name="ground_gradient",               type="gradient",         x1=0, x2=0, y1=0, y2=1},
        auto ground_gradient = anl::CImplicitGradient();
        ground_gradient.setGradient(0, 0, 0, 1);
        //	{name="lowland_shape_fractal",         type="fractal",          fractaltype=anl.BILLOW, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=2, frequency=0.25},
        auto lowland_shape_fractal = anl::CImplicitFractal(anl::BILLOW, anl::GRADIENT, anl::QUINTIC);
        lowland_shape_fractal.setNumOctaves(2);
        lowland_shape_fractal.setFrequency(0.25);
        //	{name="lowland_autocorrect",           type="autocorrect",      source="lowland_shape_fractal", low=0, high=1},
        auto lowland_autocorrect = anl::CImplicitAutoCorrect(0, 1);
        lowland_autocorrect.setSource(&lowland_shape_fractal);
        //	{name="lowland_scale",                 type="scaleoffset",      source="lowland_autocorrect", scale=0.125, offset=-0.45},
        auto lowland_scale = anl::CImplicitScaleOffset(0.125, -0.45);
        lowland_scale.setSource(&lowland_autocorrect);
        //	{name="lowland_y_scale",               type="scaledomain",      source="lowland_scale", scaley=0},
        auto lowland_y_scale = anl::CImplicitScaleDomain(1, 0);
        lowland_y_scale.setSource(&lowland_scale);
        //	{name="lowland_terrain",               type="translatedomain",  source="ground_gradient", ty="lowland_y_scale"},
        auto lowland_terrain = anl::CImplicitTranslateDomain();
        lowland_terrain.setSource(&ground_gradient);
        lowland_terrain.setYAxisSource(&lowland_y_scale);
        //
        //	{name="highland_shape_fractal",        type="fractal",          fractaltype=anl.FBM, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=4, frequency=2},
        auto highland_shape_fractal = anl::CImplicitFractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
        highland_shape_fractal.setNumOctaves(4);
        highland_shape_fractal.setFrequency(2);
        //	{name="highland_autocorrect",          type="autocorrect",      source="highland_shape_fractal", low=-1, high=1},
        auto highland_autocorrect = anl::CImplicitAutoCorrect(-1, 1);
        highland_autocorrect.setSource(&highland_shape_fractal);
        //	{name="highland_scale",                type="scaleoffset",      source="highland_autocorrect", scale=0.25, offset=0},
        auto highland_scale = anl::CImplicitScaleOffset(0.25, 0);
        highland_scale.setSource(&highland_autocorrect);
        //	{name="highland_y_scale",              type="scaledomain",      source="highland_scale", scaley=0},
        auto highland_y_scale = anl::CImplicitScaleDomain(1, 0);
        highland_y_scale.setSource(&highland_scale);
        //	{name="highland_terrain",              type="translatedomain",  source="ground_gradient", ty="highland_y_scale"},
        auto highland_terrain = anl::CImplicitTranslateDomain();
        highland_terrain.setSource(&ground_gradient);
        highland_terrain.setYAxisSource(&highland_y_scale);
        //
        //	{name="mountain_shape_fractal",        type="fractal",          fractaltype=anl.RIDGEDMULTI, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=8, frequency=1},
        auto mountain_shape_fractal = anl::CImplicitFractal(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
        mountain_shape_fractal.setNumOctaves(8);
        mountain_shape_fractal.setFrequency(1);
        //	{name="mountain_autocorrect",          type="autocorrect",      source="mountain_shape_fractal", low=-1, high=1},
        auto mountain_autocorrect = anl::CImplicitAutoCorrect(-1, 1);
        mountain_autocorrect.setSource(&mountain_shape_fractal);
        //	{name="mountain_scale",                type="scaleoffset",      source="mountain_autocorrect", scale=0.45, offset=0.15},
        auto mountain_scale = anl::CImplicitScaleOffset(0.45, 0.15);
        mountain_scale.setSource(&mountain_autocorrect);
        //	{name="mountain_y_scale",              type="scaledomain",      source="mountain_scale", scaley=0.25},
        auto mountain_y_scale = anl::CImplicitScaleDomain(1, 0.25);
        mountain_y_scale.setSource(&mountain_scale);
        //	{name="mountain_terrain",              type="translatedomain",  source="ground_gradient", ty="mountain_y_scale"},
        auto mountain_terrain = anl::CImplicitTranslateDomain();
        mountain_terrain.setSource(&ground_gradient);
        mountain_terrain.setYAxisSource(&mountain_y_scale);
        //
        //	{name="terrain_type_fractal",          type="fractal",          fractaltype=anl.FBM, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=3, frequency=0.125},
        auto terrain_type_fractal = anl::CImplicitFractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
        mountain_shape_fractal.setNumOctaves(3);
        mountain_shape_fractal.setFrequency(0.125);
        //	{name="terrain_autocorrect",           type="autocorrect",      source="terrain_type_fractal", low=0, high=1},
        auto terrain_autocorrect = anl::CImplicitAutoCorrect(0, 1);
        terrain_autocorrect.setSource(&terrain_type_fractal);
        //	{name="terrain_type_y_scale",          type="scaledomain",      source="terrain_autocorrect", scaley=0},
        auto terrain_type_y_scale = anl::CImplicitScaleDomain(1, 0);
        terrain_type_y_scale.setSource(&terrain_autocorrect);
        //	{name="terrain_type_cache",            type="cache",            source="terrain_type_y_scale"},
        auto terrain_type_cache = anl::CImplicitCache();
        terrain_type_cache.setSource(&terrain_type_y_scale);
        //	{name="highland_mountain_select",      type="select",           low="highland_terrain", high="mountain_terrain", control="terrain_type_cache", threshold=0.55, falloff=0.2},
        auto highland_mountain_select = anl::CImplicitSelect();
        highland_mountain_select.setLowSource(&highland_terrain);
        highland_mountain_select.setHighSource(&mountain_terrain);
        highland_mountain_select.setControlSource(&terrain_type_cache);
        highland_mountain_select.setThreshold(0.55);
        highland_mountain_select.setFalloff(0.2);
        //	{name="highland_lowland_select",       type="select",           low="lowland_terrain", high="highland_mountain_select", control="terrain_type_cache", threshold=0.25, falloff=0.15},
        auto highland_lowland_select = anl::CImplicitSelect();
        highland_lowland_select.setLowSource(&lowland_terrain);
        highland_lowland_select.setHighSource(&highland_mountain_select);
        highland_lowland_select.setControlSource(&terrain_type_cache);
        highland_lowland_select.setThreshold(0.25);
        highland_lowland_select.setFalloff(0.15);
        //	{name="highland_lowland_select_cache", type="cache",            source="highland_lowland_select"},
        auto highland_lowland_select_cache = anl::CImplicitCache();
        highland_lowland_select_cache.setSource(&highland_lowland_select);
        //	{name="ground_select",                 type="select",           low=0, high=1, threshold=0.5, control="highland_lowland_select_cache"},
        auto ground_select = anl::CImplicitSelect();
        ground_select.setLowSource(0.0);
        ground_select.setHighSource(1.0);
        ground_select.setControlSource(&highland_lowland_select_cache);
        ground_select.setThreshold(0.5);
        //
        // NEW: {name="cave_shape1",                   type="fractal",           fractaltype=anl.RIDGEDMULTI, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=1, frequency=4},
        //	{name="cave_shape2",                   type="fractal",           fractaltype=anl.RIDGEDMULTI, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=1, frequency=4},
        //	{name="cave_shape_attenuate",         type="combiner",           operation=anl.MULT, source_0="cave_shape1", source_1="cave_attenuate_bias", source_2="cave_shape2"},

        //	{name="cave_shape1",                    type="fractal",           fractaltype=anl.RIDGEDMULTI, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=1, frequency=4},
        auto cave_shape1 = anl::CImplicitFractal(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
        cave_shape1.setNumOctaves(1);
        cave_shape1.setFrequency(0.4);

        auto cave_shape2 = anl::CImplicitFractal(anl::RIDGEDMULTI, anl::GRADIENT, anl::QUINTIC);
        cave_shape2.setNumOctaves(1);
        cave_shape2.setFrequency(0.4);
        //	{name="cave_attenuate_bias",           type="bias",              source="highland_lowland_select_cache", bias=0.45},
        auto cave_attenuate_bias = anl::CImplicitBias(nether ? 0.045 : 0.45);
        cave_attenuate_bias.setSource(&highland_lowland_select_cache);
        //	{name="cave_shape_attenuate",          type="combiner",          operation=anl.MULT, source_0="cave_shape", source_1="cave_attenuate_bias"},
        auto cave_shape_attenuate = anl::CImplicitCombiner(anl::MULT);
        cave_shape_attenuate.setSource(0, &cave_shape1);
        cave_shape_attenuate.setSource(1, &cave_attenuate_bias);
        cave_shape_attenuate.setSource(2, &cave_shape2);
        //	{name="cave_perturb_fractal",          type="fractal",           fractaltype=anl.FBM, basistype=anl.GRADIENT, interptype=anl.QUINTIC, octaves=6, frequency=3},
        auto cave_perturb_fractal = anl::CImplicitFractal(anl::FBM, anl::GRADIENT, anl::QUINTIC);
        cave_perturb_fractal.setNumOctaves(6);
        cave_perturb_fractal.setFrequency(3);
        //	{name="cave_perturb_scale",            type="scaleoffset",       source="cave_perturb_fractal", scale=0.5, offset=0},
        auto cave_perturb_scale = anl::CImplicitScaleOffset(0.5, 0);
        cave_perturb_scale.setSource(&cave_perturb_fractal);
        //	{name="cave_perturb",                  type="translatedomain",   source="cave_shape_attenuate", tx="cave_perturb_scale"},
        auto cave_perturb = anl::CImplicitTranslateDomain();
        cave_perturb.setSource(&cave_shape_attenuate);
        cave_perturb.setXAxisSource(&cave_perturb_scale);
        //	{name="cave_select",                   type="select",            low=1, high=0, control="cave_perturb", threshold=0.48, falloff=0},
        auto cave_select = anl::CImplicitSelect();
        cave_select.setLowSource(1.0);
        cave_select.setHighSource(0.0);
        cave_select.setControlSource(&cave_perturb);
        cave_select.setThreshold(0.17); // was 0.48
        cave_select.setFalloff(0.0);
        //
        //	{name="ground_cave_multiply",          type="combiner",          operation=anl.MULT, source_0="cave_select", source_1="ground_select"}
        auto ground_cave_multiply = anl::CImplicitCombiner(anl::MULT);
        ground_cave_multiply.setSource(0, &cave_select);
        ground_cave_multiply.setSource(1, &ground_select);
        //}

        auto value = ground_cave_multiply;

        anl::CRGBACompositeChannels compose1(anl::RGB);

        compose1.setRedSource(&value);
        compose1.setGreenSource(&value);
        compose1.setBlueSource(&value);
        compose1.setAlphaSource(0.0);

        anl::TArray3D<anl::SRGBA> img(WORLD_SIZE * 16, 256, WORLD_SIZE * 16);

        anl::SMappingRanges ranges;
        mapRGBA3D(anl::SEAMLESS_NONE, img, compose1, ranges);
        return img;
    }

    void world_generator::generate_world(world* world) {
        auto data = generate_data(false);
        int seed = time(nullptr);

        std::shared_ptr<utils::noise::perlin> elevNoise = std::make_shared<utils::noise::perlin>(seed + 0);
        double elevZoom = 0.0125;
        int SCALE_FACTOR = 16;
        const int SIZE = WORLD_SIZE * 16 / SCALE_FACTOR + 1;
        double elevPoints[SIZE][SIZE];

        for (int x = 0; x < SIZE; x++) {
            for (int z = 0; z < SIZE; z++) {
                int rx = x * SCALE_FACTOR;
                int rz = z * SCALE_FACTOR;
                elevPoints[x][z] = elevNoise->get(rx * elevZoom, 0, rz * elevZoom);
            }
        }

        auto interpolate = [&](int x, int z) {
            int x0 = x / SCALE_FACTOR;
            int z0 = z / SCALE_FACTOR;
            double h00 = -1, h01 = -1, h10 = -1, h11 = -1;
            h00 = elevPoints[x0][z0];
            h01 = elevPoints[x0][z0 + 1];
            h10 = elevPoints[x0 + 1][z0];
            h11 = elevPoints[x0 + 1][z0 + 1];


            double dx = (x % SCALE_FACTOR) / (double) SCALE_FACTOR;
            double dz = (z % SCALE_FACTOR) / (double) SCALE_FACTOR;

            double hx0 = utils::noise::lerp(h00, h10, dx);
            double hx1 = utils::noise::lerp(h01, h11, dx);

            double hf = utils::noise::lerp(hx0, hx1, dz);
            return hf;
        };


        ::world::gen::simple_grass_surface surface(seed + 1);

        for (int x = 0; x < WORLD_SIZE * 16; x++) {
            for (int z = 0; z < WORLD_SIZE * 16; z++) {
                double elev = interpolate(x, z);
                int h = (int) (elev * 64 + 64);
                if (h < 0)h = 0;
                if (h >= 256)h = 256;
                for (int y_ = 0; y_ <= h; y_++) {
                    int y = y_ * 255 / h;
                    block_state bs = surface.get_for_location(x, y_, z, h);
                    if (data.get(x, y, z).length() > 0.1f || bs == 0)continue;
                    world->set(x, y_, z, bs);
                }
            }
        }
    }
}