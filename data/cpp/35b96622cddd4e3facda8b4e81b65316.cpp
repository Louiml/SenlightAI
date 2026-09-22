// Write a C++ function `std::pair<int, int> findFirstAdjacentGap(const std::vector<int>& values)` that takes a vector of integers (size ≥ 1) and returns a pair of 1-based indices `{i, i+1}` of the **first** adjacent pair where the absolute difference between the two elements is strictly greater than 1. If no such adjacent pair exists (meaning all adjacent differences are 0 or 1), return `{-1, -1}`. The function must be efficient and should not modify the input vector. You may assume the input vector contains at least one element, but handle edge cases such as a single element (return `{-1, -1}`) and vectors with negative numbers.

// The core idea is a single linear scan through the vector, comparing each consecutive pair `values[i]` and `values[i+1]`. As soon as we find `abs(values[i] - values[i+1]) > 1`, we return the 1-based pair `{i+1, i+2}`. Since we stop at the first occurrence, we do not need to examine the rest of the vector. If we finish the loop without finding any such pair, we return `{-1, -1}`. Edge cases: a vector with only one element has no adjacent pairs, so we return `{-1, -1}` immediately (the loop naturally handles this because `i` ranges from 0 to `size-2`, which is empty). Negative numbers: `abs` works fine with `int` for typical inputs; if extreme values like `INT_MIN` could cause overflow, we could cast to `long long` first, but for standard test cases `abs(int)` is sufficient. Time complexity is O(n) in the worst case, but often O(1) average due to early exit. Space complexity is O(1) beyond the input vector.

#include <vector>
#include <utility>
#include <cstdlib>

// Returns the first adjacent pair {i, i+1} (1-based indices) where |a[i] - a[i+1]| > 1.
// Returns {-1, -1} if no such pair exists.
std::pair<int, int> findFirstAdjacentGap(const std::vector<int>& values) {
    for (size_t i = 0; i + 1 < values.size(); ++i) {
        if (std::abs(values[i] - values[i + 1]) > 1) {
            return {static_cast<int>(i + 1), static_cast<int>(i + 2)};
        }
    }
    return {-1, -1};
}

#include <cassert>
#include <vector>
#include <utility>

// Assume findFirstAdjacentGap is declared above (not repeated here).

int main() {
    // Basic case with a gap at the beginning
    assert(findFirstAdjacentGap({3, 1, 2}) == std::make_pair(1, 2));
    
    // Gap in the middle
    assert(findFirstAdjacentGap({1, 2, 4, 5}) == std::make_pair(2, 3));
    
    // No gap (differences are 0 or 1)
    assert(findFirstAdjacentGap({1, 2, 2, 3, 4}) == std::make_pair(-1, -1));
    
    // Single element
    assert(findFirstAdjacentGap({5}) == std::make_pair(-1, -1));
    
    // All equal elements
    assert(findFirstAdjacentGap({7, 7, 7}) == std::make_pair(-1, -1));
    
    // Negative numbers with a large gap
    assert(findFirstAdjacentGap({-2, -4, -3}) == std::make_pair(1, 2));
    
    // Multiple gaps, return the first one
    assert(findFirstAdjacentGap({0, 5, 6, 10}) == std::make_pair(1, 2));
    
    // Large positive values
    assert(findFirstAdjacentGap({1000000, 1000001, 1000003}) == std::make_pair(2, 3));
    
    // Gap at the very end
    assert(findFirstAdjacentGap({1, 1, 1, 5}) == std::make_pair(3, 4));
    
    // Two elements with a gap
    assert(findFirstAdjacentGap({10, 20}) == std::make_pair(1, 2));
    
    return 0;
}
