// Write a C++ function named `sortIntegersByBitCount` that takes a non-empty vector of integers and returns a new vector with the same integers sorted according to the following rule: integers with fewer set bits (1s in their binary representation) come first; if two integers have the same number of set bits, the smaller integer value comes first. The function must not modify the original input vector. Assume that integers are 32‑bit signed, so negative numbers are represented in two's complement, but for the purpose of counting set bits, you should count the bits in the 32‑bit representation (use a built‑in popcount if available, or implement your own). The returned vector must preserve all duplicate values. If the input vector contains only one element, the output must equal the input.
#include <cassert>
#include <vector>

int main() {
    // Basic sorting by bit count and value.
    std::vector<int> v1 = {2, 3, 1, 4};
    std::vector<int> e1 = {1, 2, 4, 3};  // bits: 1 has 1, 2 has 1, 4 has 1, 3 has 2
    assert(sortIntegersByBitCount(v1) == e1);
    // Original unchanged.
    assert(v1 == std::vector<int>({2, 3, 1, 4}));

    // Equal bit counts sorted by value.
    std::vector<int> v2 = {5, 9, 2, 8};
    std::vector<int> e2 = {2, 8, 5, 9};  // 2 and 8 have 1 bit, then 5 and 9 have 2 bits
    assert(sortIntegersByBitCount(v2) == e2);

    // Negative numbers: -1 has all 32 bits set.
    std::vector<int> v3 = {-1, 0, 1};
    std::vector<int> e3 = {0, 1, -1};  // 0 has 0 bits, 1 has 1, -1 has 32
    assert(sortIntegersByBitCount(v3) == e3);

    // Duplicates preserved.
    std::vector<int> v4 = {3, 3, 1, 3};
    std::vector<int> e4 = {1, 3, 3, 3};
    assert(sortIntegersByBitCount(v4) == e4);

    // Single element.
    std::vector<int> v5 = {42};
    assert(sortIntegersByBitCount(v5) == std::vector<int>({42}));

    // All same bit count, sorted by value.
    std::vector<int> v6 = {4, 1, 2, 8};
    std::vector<int> e6 = {1, 2, 4, 8};
    assert(sortIntegersByBitCount(v6) == e6);

    // Larger mixed case.
    std::vector<int> v7 = {7, 8, 3, 5, 16, 1};
    std::vector<int> e7 = {1, 8, 16, 3, 5, 7};  // popcounts: 1 (1), 8(1),16(1),3(2),5(2),7(3)
    assert(sortIntegersByBitCount(v7) == e7);

    // Check empty input (though spec says non-empty, function handles it).
    std::vector<int> v8;
    assert(sortIntegersByBitCount(v8).empty());
}
#include <vector>
#include <algorithm>

// Helper to count set bits in a 32-bit integer (portable fallback).
int countSetBits(int value) {
    unsigned int bits = static_cast<unsigned int>(value);
    int count = 0;
    while (bits) {
        count += bits & 1;
        bits >>= 1;
    }
    return count;
}

// Return a new vector sorted by number of set bits (ascending), then by value.
std::vector<int> sortIntegersByBitCount(const std::vector<int>& arr) {
    std::vector<int> result = arr;  // copy to avoid modifying input
    std::sort(result.begin(), result.end(), [](int a, int b) {
        int bitsA = __builtin_popcount(static_cast<unsigned int>(a));
        int bitsB = __builtin_popcount(static_cast<unsigned int>(b));
        if (bitsA == bitsB) {
            return a < b;
        }
        return bitsA < bitsB;
    });
    return result;
}
// The solution uses `std::sort` with a custom comparator. The comparator first counts the set bits of two integers `a` and `b` using `__builtin_popcount` (GCC/Clang) or a manual loop for portability. If the popcounts are equal, it returns `a < b` (lexicographic ordering by value); otherwise it returns the one with the smaller popcount first. This comparator is strict weak ordering because it compares by a pair `(popcount, value)`. Edge cases include duplicates (they compare equal via `a < b` returning false in both directions, so their relative order remains stable in practice), negative numbers (popcount works on the 32‑bit bit pattern, so e.g., -1 has 32 set bits), and a single element (sort does nothing). Time complexity is `O(n log n)` due to sorting, where `n` is the number of elements, and the comparator’s popcount is `O(1)` for built‑in or `O(32)` for a manual loop. Auxiliary space is `O(1)` besides the output vector itself, because sorting is in‑place on a copy.
