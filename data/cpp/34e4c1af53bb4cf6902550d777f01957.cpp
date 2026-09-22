// Write a C++ function that, given a vector of 64-bit signed integers and a non-negative integer `k`, returns a vector containing the `k` largest distinct values from the input in descending order. If `k` is 0 or there are fewer than `k` distinct values, the function should return the distinct values sorted in descending order (if any). The input vector may contain duplicates, negative numbers, and may be empty. The function must not modify the input vector and must be named `topKDistinct`.

// The solution approach is to first collect all distinct values from the input using a set (e.g., `std::set<int64_t>`), which automatically sorts elements in ascending order and removes duplicates. Since the set is sorted ascending, we can iterate from the end (largest elements) backwards to extract up to `k` values, pushing them into the result vector in descending order. If `k` is greater than or equal to the number of distinct elements, we simply take all of them (which is equivalent to returning the distinct values in descending order). Edge cases: an empty input yields an empty result; `k == 0` yields an empty result; duplicates are ignored. Time complexity is `O(n log n)` due to the set insertion where `n` is the number of elements in the input, and space complexity is `O(d)` where `d` is the number of distinct elements (for the set and the result vector).

#include <vector>
#include <set>
#include <cstdint>
#include <algorithm>

// Return the k largest distinct values from the input in descending order.
// If k is 0 or there are fewer than k distinct values, return all distinct values.
std::vector<int64_t> topKDistinct(const std::vector<int64_t>& values, int k) {
    // Collect distinct values in ascending order using a set.
    std::set<int64_t> distinct(values.begin(), values.end());

    // Determine how many elements to return.
    int take = std::min(k, static_cast<int>(distinct.size()));
    if (take <= 0) {
        return {};
    }

    // Iterate from the largest elements backwards to collect descending order.
    std::vector<int64_t> result;
    result.reserve(take);
    auto it = distinct.end();
    for (int i = 0; i < take; ++i) {
        --it;
        result.push_back(*it);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is declared here (as if from the header).
std::vector<int64_t> topKDistinct(const std::vector<int64_t>& values, int k);

int main() {
    // Basic case with duplicates and negative numbers.
    assert(topKDistinct({3, 1, 4, 1, 5, 9, 2, 6, 5}, 3) == std::vector<int64_t>({9, 6, 5}));
    
    // k larger than distinct count returns all distinct values descending.
    assert(topKDistinct({-1, -2, -3}, 10) == std::vector<int64_t>({-1, -2, -3}));
    
    // k == 0 returns empty.
    assert(topKDistinct({7, 7, 7}, 0).empty());
    
    // Empty input returns empty.
    assert(topKDistinct({}, 2).empty());
    
    // Single distinct value.
    assert(topKDistinct({42, 42}, 1) == std::vector<int64_t>({42}));
    
    // k == number of distinct values exactly.
    assert(topKDistinct({5, 3, 8}, 3) == std::vector<int64_t>({8, 5, 3}));
    
    // All duplicates.
    assert(topKDistinct({0, 0, 0}, 5) == std::vector<int64_t>({0}));
    
    // Large values within 64-bit range.
    assert(topKDistinct({INT64_MAX, INT64_MIN, 0}, 2) == std::vector<int64_t>({INT64_MAX, 0}));
    
    // k is negative treated same as 0.
    assert(topKDistinct({1, 2}, -3).empty());
    
    return 0;
}
