#include "Math.hpp"
#include <math.h>

namespace wfe {
    float32_t Sin(float32_t x) {
        return sin(x);
    }
    float32_t Cos(float32_t x) {
        return cos(x);
    }
    float32_t Tan(float32_t x) {
        return tan(x);
    }
    float32_t Asin(float32_t x) {
        return asin(x);
    }
    float32_t Acos(float32_t x) {
        return acos(x);
    }
    float32_t Atan(float32_t x) {
        return atan(x);
    }

    float32_t Sqrt(float32_t x) {
        return sqrt(x);
    }
    float32_t InvSqrt(float32_t x) {
        uint32_t i;
        float32_t halfX = x * .5f, invSqrtX = x;

        // Copy the float's value into an unsigned int
        i = *(uint32_t*)&invSqrtX;

        // Perform dark magic
        i = 0x5f3759df - (i >> 1);

        // Copy the unsigned int's value into a float
        invSqrtX = *(float32_t*)&i;

        // Apply two Newton iterations
        invSqrtX = invSqrtX * (1.5f - (halfX * invSqrtX * invSqrtX));
        invSqrtX = invSqrtX * (1.5f - (halfX * invSqrtX * invSqrtX));

        return invSqrtX;
    }

    float32_t Absolute(float32_t x) {
        // Apply an IEEE 754 bit hack to the number
        (*(uint32_t*)&x) &= ~(1 << 31);

        // Return the number
        return x;
    }

    bool8_t IsPowerOf2(uint64_t x) {
        return x && !(x & (x - 1));
    }

    void RandomSeed(uint32_t seed) {
        srand(seed);
    }
    int32_t Random() {
        return rand();
    }
    int32_t RandomInRange(int32_t min, int32_t max) {
        return (rand() % (max - min + 1)) + min;
    }
    float32_t RandomFloat() {
        return (float32_t)Random() / (float32_t)RAND_MAX;
    }
    float32_t RandomFloatInRange(float32_t min, float32_t max) {
        return ((float32_t)Random() / ((float32_t)RAND_MAX / (max - min))) + min;
    }
}