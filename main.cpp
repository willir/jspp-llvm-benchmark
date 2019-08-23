#include <iostream>
#include <vector>
#include <fstream>

#include <benchmark/benchmark.h>

#include "murmur.h"


static constexpr size_t DATA_SIZE = 256 * 1024 + 3;
static constexpr uint32_t SEED = 1847734911;


static std::vector<uint8_t> readInput() {
    std::ifstream ifs("/home/willir/tmp/rand", std::ios::binary | std::ios::ate);

    if (!ifs.is_open()) {
        std::cerr << "Cannot open input file\n";
        exit(1);
    }

    const auto size = DATA_SIZE;//ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<uint8_t> res(size);
    if (ifs.read(reinterpret_cast<char *>(res.data()), size)) {
        return res;
    } else {
        std::cerr << "Cannot open input file\n";
        exit(1);
    }
}

static void bmJsppMurmur(benchmark::State& state) {
    auto vec = readInput();
    struct Arr arr {
        .data = vec.data(),
        .len = static_cast<int>(vec.size())
    };

    for (auto _ : state) {
        murmur(&arr, SEED);
    }
}

static void bmCppMurmur(benchmark::State& state) {
    auto vec = readInput();
    struct Arr arr {
        .data = vec.data(),
        .len = static_cast<int>(vec.size())
    };

    for (auto _ : state) {
        murmur3Cpp(arr, SEED);
    }
}

#if 0

BENCHMARK(bmJsppMurmur);
BENCHMARK(bmCppMurmur);

BENCHMARK_MAIN();

#else

int main() {
    auto vec = readInput();

    struct Arr arr {
        .data = vec.data(),
        .len = static_cast<int>(vec.size())
    };

    std::cout << murmur(&arr, SEED) << "\n";
    std::cout << murmur3Cpp(arr, SEED) << "\n";
    
    

    return 0;
}
#endif