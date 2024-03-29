
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include "murmur.h"

#if 0

uint32_t murmur3Cpp(const Arr &arr, uint32_t seed)
{
    uint32_t h = seed;

    auto key = arr.data;
    const auto len = arr.len;

    if (len > 3) {
        size_t i = len >> 2;
        do {
            uint32_t k;
            memcpy(&k, key, sizeof(uint32_t));

            key += sizeof(uint32_t);
            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;

            h ^= k;
            h = (h << 13) | (h >> 19);
            h = h * 5 + 0xe6546b64;
        } while (--i);
    }
    if (len & 3) {
        size_t i = len & 3;
        uint32_t k = 0;
        do {
            k <<= 8;
            k |= key[i - 1];
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

#else

uint32_t murmur3Cpp(const Arr &arr, uint32_t seed)
{
    uint32_t h = seed;

    auto key = arr.data;
    const auto len = arr.len;

    if (len > 3) {
        size_t i = len >> 2;
        do {
            uint32_t k = key[3];

            k <<= 8;
            k |= key[2];

            k <<= 8;
            k |= key[1];

            k <<= 8;
            k |= key[0];

            key += sizeof(uint32_t);

            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;

            h ^= k;
            h = (h << 13) | (h >> 19);
            h = h * 5 + 0xe6546b64;
        } while (--i);
    }
    if (len & 3) {
        size_t i = len & 3;
        uint32_t k = 0;
        do {
            k <<= 8;
            k |= key[i - 1];
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }
    h ^= len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

#endif