#include <cstdint>

struct Arr {
    const uint8_t *data;
    int32_t len;
};

extern "C" {
    uint32_t murmur(Arr *arr, uint32_t seed);
    uint32_t murmur_rust(Arr *arr, uint32_t seed);
    uint32_t murmur_rust2(Arr *arr, uint32_t seed);
}

extern uint32_t murmur3Cpp(const Arr &arr, uint32_t seed);
extern uint32_t murmur3CppOpt(const Arr &arr, uint32_t seed);
