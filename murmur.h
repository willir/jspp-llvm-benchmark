#include <cstdint>

struct Arr {
    uint8_t *data;
    int32_t len;
};

extern "C" {
    uint32_t murmur(Arr *arr, uint32_t seed);
}

extern uint32_t murmur3Cpp(const Arr &arr, uint32_t seed);
