// Write a C++ function that takes two vectors of integers, `a` and `b`, of possibly different lengths, and returns a vector of integers containing the 1-based positions (indices) where the two vectors have equal elements, but only for indices that exist in both vectors (i.e., stop at the smaller length). The returned vector should be sorted in ascending order. If no positions match, return an empty vector. The function must not modify the input vectors.
The core idea is to compare the two vectors element‑by‑element only up to the minimum of their sizes, because any index beyond the shorter vector cannot be compared. For each valid index `i` from 0 to `min(n, m) - 1`, check if `a[i] == b[i]`. If true, push `i + 1` (converting to 1‑based) into the result vector. This is a straightforward linear scan. Edge cases include: one or both vectors empty (result is empty), vectors of equal length but no equal elements (result empty), and vectors where the shorter one ends before any mismatch (only compare up to shorter length). Time complexity is `O(min(n, m))` because we only loop that many times. Space complexity is `O(k)` for the output, where `k` is the number of matching positions, plus `O(1)` auxiliary space.
#include <vector>
#include <algorithm>

// Given two vectors of integers, return the 1-based positions (indices) where both vectors have equal elements.
// Only indices that exist in both vectors are considered (up to the smaller size).
// Returns a vector of matching positions in ascending order (naturally sorted).
std::vector<int> equalPositions(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> matches;
    const size_t common = std::min(a.size(), b.size());
    for (size_t i = 0; i < common; ++i) {
        if (a[i] == b[i]) {
            matches.push_back(static_cast<int>(i + 1)); // 1-based
        }
    }
    return matches;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic equal-length vectors with some matches
    std::vector<int> a1 = {1, 2, 3, 4};
    std::vector<int> b1 = {1, 0, 3, 5};
    std::vector<int> r1 = equalPositions(a1, b1);
    assert(r1 == std::vector<int>({1, 3}));

    // Test 2: Different lengths - only compare up to shorter
    std::vector<int> a2 = {5, 6, 7};
    std::vector<int> b2 = {5, 6, 100, 200};
    std::vector<int> r2 = equalPositions(a2, b2);
    assert(r2 == std::vector<int>({1, 2}));

    // Test 3: No matches
    std::vector<int> a3 = {1, 2, 3};
    std::vector<int> b3 = {4, 5, 6};
    std::vector<int> r3 = equalPositions(a3, b3);
    assert(r3.empty());

    // Test 4: One vector empty
    std::vector<int> a4 = {};
    std::vector<int> b4 = {1, 2};
    assert(equalPositions(a4, b4).empty());

    // Test 5: Negative numbers and zeros
    std::vector<int> a5 = {-1, 0, 2, -3};
    std::vector<int> b5 = {-1, -1, 2, -3};
    std::vector<int> r5 = equalPositions(a5, b5);
    assert(r5 == std::vector<int>({1, 3, 4}));

    // Test 6: All equal
    std::vector<int> a6 = {7, 7, 7};
    std::vector<int> b6 = {7, 7, 7};
    std::vector<int> r6 = equalPositions(a6, b6);
    assert(r6 == std::vector<int>({1, 2, 3}));

    // Test 7: Shorter first vector ends before mismatch
    std::vector<int> a7 = {1, 2};
    std::vector<int> b7 = {1, 2, 999};
    assert(equalPositions(a7, b7) == std::vector<int>({1, 2}));

    // Test 8: Both empty
    assert(equalPositions({}, {}).empty());

    return 0;
}
