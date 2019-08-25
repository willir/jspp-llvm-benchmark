#include <iostream>
#include <vector>
#include <fstream>
#include <tuple>

#include <benchmark/benchmark.h>

#include "murmur.h"


static constexpr size_t DATA_SIZE = 256 * 1024 + 0;
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

static std::tuple<std::vector<uint8_t>, Arr> readAndParseInput() {
    auto vec = readInput();
    struct Arr arr {
        .data = vec.data(),
        .len = static_cast<int>(vec.size())
    };

    return std::make_tuple(std::move(vec), std::move(arr));
}

static void bmJsppMurmur(benchmark::State& state) {
    auto [vec, arr] = readAndParseInput();

    for (auto _ : state) {
        murmur(&arr, SEED);
    }
}

static void bmCppMurmur(benchmark::State& state) {
    auto [vec, arr] = readAndParseInput();

    for (auto _ : state) {
        murmur3Cpp(arr, SEED);
    }
}

static void bmCppOptMurmur(benchmark::State& state) {
    auto [vec, arr] = readAndParseInput();

    for (auto _ : state) {
        murmur3CppOpt(arr, SEED);
    }
}

static void bmRustMurmur(benchmark::State& state) {
    auto [vec, arr] = readAndParseInput();

    for (auto _ : state) {
        murmur_rust(&arr, SEED);
    }
}

static void bmRustRustiWayMurmur(benchmark::State& state) {
    auto [vec, arr] = readAndParseInput();

    for (auto _ : state) {
        murmur_rust2(&arr, SEED);
    }
}

#if 1

BENCHMARK(bmJsppMurmur);
BENCHMARK(bmCppMurmur);
BENCHMARK(bmCppOptMurmur);
BENCHMARK(bmRustMurmur);
BENCHMARK(bmRustRustiWayMurmur);

BENCHMARK_MAIN();

#else

int main() {
    auto [vec, arr] = readAndParseInput();

    std::cout << "JS++:  \t\t\t" << murmur(&arr, SEED) << "\n";
    std::cout << "C:  \t\t\t" << murmur3Cpp(arr, SEED) << "\n";
    std::cout << "C++ with std:optional:\t" << murmur3CppOpt(arr, SEED) << "\n";
    std::cout << "Rust:  \t\t\t" << murmur_rust(&arr, SEED) << "\n";   
    std::cout << "Rust rusti way: \t" << murmur_rust2(&arr, SEED) << "\n";   

    return 0;
}
#endif