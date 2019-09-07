#!/bin/bash

rustc -O --crate-type=staticlib murmur.rs  -o /tmp/murmurRust.a
clang++ -std=c++17 -O3 main.cpp murmur.cpp murmur-optional.cpp /tmp/murmur.o /tmp/murmurRust.a -o /tmp/murmur -lbenchmark -lpthread -ldl
