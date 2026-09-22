Write a C++ function that takes a non-empty vector of 64-bit signed integers and returns a new vector containing only the elements that appear exactly once in the input, preserving their original relative order. For example, given `{1, 2, 3, 2, 4, 1, 5}`, the output should be `{3, 4, 5}`. The function must handle negative numbers, zeros, large values (up to ±10^18), and duplicate entries that may appear any number of times. If all elements are duplicates, return an empty vector.
The core idea is to count the frequency of each value using an unordered_map from `long long` to `int`. Since we need to preserve order, we iterate through the input vector once to populate the frequency map, then iterate again to collect elements whose frequency is exactly 1. An important edge case is the empty vector — although the problem states non-empty, the function should still handle it gracefully by returning an empty vector. Duplicate handling is automatic via the map. Time complexity is O(n) on average due to hash map operations, and O(n) in the worst case if collisions occur. Space complexity is O(n) for the map and the result vector.
#include <vector>
#include <unordered_map>
#include <cstdint>

// Return a vector containing elements that appear exactly once, preserving original order.
std::vector<long long> uniqueOnce(const std::vector<long long>& input) {
    std::unordered_map<long long, int> frequency;
    for (const auto& num : input) {
        ++frequency[num];
    }

    std::vector<long long> result;
    result.reserve(input.size());
    for (const auto& num : input) {
        if (frequency[num] == 1) {
            result.push_back(num);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// The solution function is expected to be declared above this test block.

int main() {
    // Basic mixed duplicates
    std::vector<long long> v1 = {1, 2, 3, 2, 4, 1, 5};
    assert(uniqueOnce(v1) == std::vector<long long>({3, 4, 5}));

    // All unique
    std::vector<long long> v2 = {10, -20, 30};
    assert(uniqueOnce(v2) == std::vector<long long>({10, -20, 30}));

    // All duplicates
    std::vector<long long> v3 = {7, 7, 7, 7};
    assert(uniqueOnce(v3) == std::vector<long long>());

    // Single element
    std::vector<long long> v4 = {42};
    assert(uniqueOnce(v4) == std::vector<long long>({42}));

    // Negative and zero mixed
    std::vector<long long> v5 = {-1, 0, -1, 5, 0, 3, 3, 3};
    assert(uniqueOnce(v5) == std::vector<long long>({5}));

    // Large values (within long long range)
    std::vector<long long> v6 = {1000000000000000000LL, -999999999999999999LL, 1000000000000000000LL};
    assert(uniqueOnce(v6) == std::vector<long long>({-999999999999999999LL}));

    // Preserving order with distant duplicates
    std::vector<long long> v7 = {5, 9, 5, 8, 9, 4, 8, 7};
    assert(uniqueOnce(v7) == std::vector<long long>({4, 7}));

    // Empty vector (edge case)
    std::vector<long long> v8 = {};
    assert(uniqueOnce(v8) == std::vector<long long>());

    // Duplicates with more than two occurrences
    std::vector<long long> v9 = {1, 2, 1, 1, 3, 2, 4, 2, 5, 5, 5};
    assert(uniqueOnce(v9) == std::vector<long long>({3, 4}));

    // Two-element vector with both same
    std::vector<long long> v10 = {8, 8};
    assert(uniqueOnce(v10) == std::vector<long long>());

    return 0;
}
