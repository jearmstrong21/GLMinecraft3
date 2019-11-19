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

        struct IntTwister{

        private:
            static const int statesize=624;
            uint32_t state[statesize];
            int next;

            void twist();

        public:
            explicit IntTwister(uint32_t seed);

            [[nodiscard]] int get();

        };

        struct Perlin {

        private:
            int p[512];

            double lerp(double a,double b,double x);
            double fade(double t);
            double grad(int hash,double x,double y,double z);

            void initialize_permutation(IntTwister*twister);

        public:

            explicit Perlin(uint32_t seed);//seeded with IntTwister
            explicit Perlin(IntTwister*twister);

            [[nodiscard]] double perlin(double x,double y,double z);

        };

    }

}

#endif //GLMINECRAFT3_NOISE_H
