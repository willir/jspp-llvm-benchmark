#include <optional>
#include <cstdint>
#include <cstdlib>

#include "murmur.h"

static std::optional<uint8_t> get_my_arr(const Arr &arr, int32_t i) {
    if (i < arr.len) {
        return arr.data[i];
    } else {
        return std::nullopt;
    }
}

uint32_t murmur3CppOpt(const Arr &arr, uint32_t seed) {
    uint32_t h = seed;

    if (arr.len > 3) {
        const auto len4 = (arr.len >> 2) << 2;
        for (size_t i = 0; i < len4; i += 4) {
            uint32_t k = get_my_arr(arr, i + 3).value_or(0);

            k <<= 8;
            k |= get_my_arr(arr, i + 2).value_or(0);

            k <<= 8;
            k |= get_my_arr(arr, i + 1).value_or(0);

            k <<= 8;
            k |= get_my_arr(arr, i).value_or(0);

            k *= 0xcc9e2d51;
            k = (k << 15) | (k >> 17);
            k *= 0x1b873593;
            h ^= k;
            h = (h << 13) | (h >> 19);
            h = h * 5 + 0xe6546b64;
        }
    }
    if (arr.len & 3) {
        size_t i = arr.len & 3;
        size_t last_even_i = (arr.len >> 2) << 2;
        uint32_t k = 0;
        do {
            k <<= 8;
            k |= get_my_arr(arr, last_even_i + i - 1).value_or(0);
        } while (--i);
        k *= 0xcc9e2d51;
        k = (k << 15) | (k >> 17);
        k *= 0x1b873593;
        h ^= k;
    }
    h ^= arr.len;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}
