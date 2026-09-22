Implement a C++ function named `cellHashKey` that takes three integer coordinates (`i`, `j`, `k`) and a positive integer `tableSize`, and returns a hash key value in the range `[0, tableSize-1]` using the formula: `abs(541*i + 79*j + 31*k) % tableSize`. The function must handle negative coordinates correctly by using the absolute value of the computed linear combination before applying the modulo operation. Additionally, write a helper function `sameCell` that checks if two coordinate triples are equal (to be used for collision verification). The solution should be a standalone, const-correct, and well-documented implementation that can be used as the core of a spatial hash map for 3D grid cells.
The core algorithm is a simple multiplicative hash function that combines three integer coordinates into a single key. The linear combination `541*i + 79*j + 31*k` uses prime-like coefficients to distribute coordinates evenly across the hash table. Taking the absolute value ensures that negative coordinates produce a non-negative result, and the modulo operation with `tableSize` confines the key to a valid index. The main edge case is integer overflow: for large coordinates, the sum might overflow a 32-bit `int`; therefore, we cast the coordinates to `long` before multiplication to ensure 64-bit arithmetic (assuming `long` is at least 64 bits, which is typical on modern platforms). If `tableSize` is not positive, we can clamp it to 1 to avoid division by zero, though the problem statement implies it is positive. The `sameCell` helper simply does a logical AND of three equality checks. Time complexity is O(1) for both functions, and space complexity is O(1) as no auxiliary storage is used.
#include <cstdlib>   // for std::abs
#include <cstddef>   // for std::size_t (optional)

// Returns a hash key in [0, tableSize-1] for the given 3D coordinates.
// Uses a linear combination with prime-like coefficients and absolute value.
// tableSize must be positive; if not, it is clamped to 1.
long cellHashKey(int i, int j, int k, int tableSize) {
    // Ensure a positive table size to avoid modulo by zero.
    if (tableSize <= 0) {
        tableSize = 1;
    }
    // Use long to avoid overflow on large coordinates.
    long combined = 541L * static_cast<long>(i) +
                    79L * static_cast<long>(j) +
                    31L * static_cast<long>(k);
    long absValue = std::abs(combined);
    return absValue % static_cast<long>(tableSize);
}

// Returns true if both coordinate triples are identical.
bool sameCell(int i1, int j1, int k1, int i2, int j2, int k2) {
    return (i1 == i2) && (j1 == j2) && (k1 == k2);
}
#include <cassert>

int main() {
    // Basic positive coordinates.
    assert(cellHashKey(0, 0, 0, 100) == 0);
    assert(cellHashKey(1, 2, 3, 100) == (541L*1 + 79L*2 + 31L*3) % 100);

    // Negative coordinates produce a non-negative result.
    long keyNeg = cellHashKey(-1, -2, -3, 100);
    assert(keyNeg >= 0 && keyNeg < 100);
    assert(cellHashKey(-1, -2, -3, 100) == std::abs(541L*(-1) + 79L*(-2) + 31L*(-3)) % 100);

    // Hash keys are within range for various table sizes.
    for (int tableSize : {1, 2, 7, 1000}) {
        for (int i = -10; i <= 10; ++i) {
            for (int j = -10; j <= 10; ++j) {
                for (int k = -10; k <= 10; ++k) {
                    long key = cellHashKey(i, j, k, tableSize);
                    assert(key >= 0 && key < static_cast<long>(tableSize));
                }
            }
        }
    }

    // Non-positive table size is clamped to 1.
    assert(cellHashKey(5, 5, 5, 0) == 0);
    assert(cellHashKey(5, 5, 5, -3) == 0);

    // sameCell checks equality correctly.
    assert(sameCell(1, 2, 3, 1, 2, 3));
    assert(!sameCell(1, 2, 3, 1, 2, 4));
    assert(!sameCell(1, 2, 3, 1, 3, 3));
    assert(!sameCell(1, 2, 3, 2, 2, 3));

    // Collision check: different coordinates may map to the same key,
    // but sameCell distinguishes them.
    int i1=1, j1=2, k1=3, i2=4, j2=5, k2=6;
    long key1 = cellHashKey(i1, j1, k1, 1000);
    long key2 = cellHashKey(i2, j2, k2, 1000);
    // If keys are equal, cells are different (unless coordinates are same).
    assert(!sameCell(i1, j1, k1, i2, j2, k2) || (key1 == key2));

    // Verify that identical coordinates always produce the same key.
    assert(cellHashKey(7, -8, 9, 123) == cellHashKey(7, -8, 9, 123));

    // Boundary check with large-ish coordinates (still safe with long).
    assert(cellHashKey(1000000, -2000000, 3000000, 100000) >= 0);
    assert(cellHashKey(1000000, -2000000, 3000000, 100000) < 100000);

    return 0;
}
