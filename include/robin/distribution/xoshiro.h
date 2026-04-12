#pragma once

#include <cstdint>

class Xoshiro256 {
    uint64_t s[4];

    inline uint64_t rotl(const uint64_t x, int k) {
        return (x << k) | (x >> (64 - k));
    }

    uint64_t splitmix64(uint64_t& state) {
        uint64_t z = (state += 0x9e3779b97f4a7c15);
        z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9;
        z = (z ^ (z >> 27)) * 0x94d049bb133111eb;
        return z ^ (z >> 31);
    }

public:
    static constexpr uint64_t min() {
        return 0; 
    }

    static constexpr uint64_t max() { 
        return UINT64_MAX; 
    }

    Xoshiro256(uint64_t seed) {
        s[0] = splitmix64(seed);
        s[1] = splitmix64(seed);
        s[2] = splitmix64(seed);
        s[3] = splitmix64(seed);
    }

    inline uint64_t operator()() {
        const uint64_t result{ rotl(s[1] * 5, 7) * 9 };
        const uint64_t t{ s[1] << 17 };

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = rotl(s[3], 45);

        return result;
    }
};
