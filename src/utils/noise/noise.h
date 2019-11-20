//
// Created by Jack Armstrong on 11/17/19.
//

#ifndef GLMINECRAFT3_NOISE_H
#define GLMINECRAFT3_NOISE_H

#include <cstdlib>
#include <utility>
#include <algorithm>

namespace utils {

    namespace noise {

        struct int_twister{

        private:
            static const int state_size=624;
            uint32_t state[state_size];
            int next;

            void twist();

        public:
            explicit int_twister(uint32_t seed);

            [[nodiscard]] int get();

        };

        struct perlin {

        private:
            int p[512];

            double lerp(double a,double b,double x);
            double fade(double t);
            double grad(int hash,double x,double y,double z);

            void initialize_permutation(int_twister*twister);

        public:

            explicit perlin(uint32_t seed);//seeded with int_twister
            explicit perlin(int_twister*twister);

            [[nodiscard]] double get(double x,double y,double z);

        };

    }

}

#endif //GLMINECRAFT3_NOISE_H
