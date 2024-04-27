#pragma once

#include <bitset>
#include <extmath.hpp>
#include <glad/glad.h>

constexpr size_t chunkSize = 128u;
constexpr size_t chunkCells = chunkSize * chunkSize * chunkSize;

inline size_t flatIdx(uint16_t x, uint16_t y) {
    return y * chunkSize + x;
}

inline size_t flatIdx(uint16_t x, uint16_t y, uint16_t z) {
    return z * (chunkSize * chunkSize) + flatIdx(x, y);
}

// Generate a table mapping cube configuration to edge indices
std::array<GLuint, 256> generateEdgeTable();