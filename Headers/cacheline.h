#pragma once

#include <cstdint>

#define CACHELINE_SIZE 64
#define ITEM_COUNT (CACHELINE_SIZE / sizeof(int32_t))

static inline uint32_t murmur_32_scramble(uint32_t k) {
    /// Murmur3 32bit https://en.wikipedia.org/wiki/MurmurHash
    k *= 0xcc9e2d51;
    k = (k << 15) | (k >> 17);
    k *= 0x1b873593;
    return k;
}

static inline uint32_t murmur_32(uint32_t val) {
    /// Murmur3 32bit https://en.wikipedia.org/wiki/MurmurHash
    auto h = 19553029U;

    /// Scramble
    h ^= murmur_32_scramble(val);
    h = (h << 13U) | (h >> 19U);
    h = h * 5U + 0xe6546b64;

    /// Finalize
    h ^= sizeof(uint32_t);
    h ^= h >> 16U;
    h *= 0x85ebca6b;
    h ^= h >> 13U;
    h *= 0xc2b2ae35;
    h ^= h >> 16U;

    return h;
}

uint32_t do_some_work(uint32_t val) {
    return murmur_32(val) + murmur_32(val >> 2U);
}

class alignas(CACHELINE_SIZE) cacheLine {
   public:
    int32_t values[ITEM_COUNT];

    cacheLine& operator=(int64_t n) {
        for (auto& val : values) {
            val = n;
            n >>= 1;
        }
        return *this;
    }

    uint64_t compute() const {
        return do_some_work(values[0]) + do_some_work(values[(ITEM_COUNT-1)]);
    }
};

