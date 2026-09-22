Write a C++ function `int countMinimalGapPairs(const std::vector<int>& prices)` that computes the number of adjacent pairs in the sorted version of the input vector whose difference equals the global minimum absolute difference between any two distinct elements in the original vector. Note that the input may contain duplicate values; if all elements are identical, the minimum difference is 0, and every adjacent pair in the sorted list qualifies (so the count is `size - 1`). The function must handle empty input (return 0), and must treat the difference as the absolute value between distinct elements in the original multiset (even if they are equal, the difference is 0). Use `const` correctly and avoid modifying the input.

The core idea is to sort the input to bring close values together, because the minimum absolute difference between any two elements in a multiset will always be achieved by some pair of adjacent elements after sorting (including duplicates, which give difference 0). After sorting, we compute `min_gap` as the smallest difference between consecutive elements. Then we count how many adjacent pairs in the sorted array have exactly that difference. Edge cases: empty vector → return 0; single element → there are no adjacent pairs, so return 0. All identical values → `min_gap` = 0 and all `n-1` adjacent pairs count. Time complexity is \(O(n \log n)\) due to sorting, with \(O(1)\) extra space (ignoring the sort's internal cost) if we copy the input; space complexity is \(O(n)\) for the copy of the input vector. The approach is straightforward and robust.

#include <vector>
#include <algorithm>
#include <cstddef>

// Counts the number of adjacent pairs (in sorted order) whose difference
// equals the minimum absolute difference between any two elements of prices.
int countMinimalGapPairs(const std::vector<int>& prices) {
    if (prices.size() < 2) {
        return 0;
    }

    std::vector<int> sorted = prices;
    std::sort(sorted.begin(), sorted.end());

    int min_gap = sorted[1] - sorted[0];
    for (std::size_t i = 2; i < sorted.size(); ++i) {
        min_gap = std::min(min_gap, sorted[i] - sorted[i - 1]);
    }

    int count = 0;
    for (std::size_t i = 1; i < sorted.size(); ++i) {
        if (sorted[i] - sorted[i - 1] == min_gap) {
            ++count;
        }
    }

    return count;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case with distinct values
    assert(countMinimalGapPairs({1, 3, 7, 5, 12}) == 1); // min gap = 2 (3-1), only one pair

    // Duplicate values produce gap 0
    assert(countMinimalGapPairs({90, 98, 90, 90, 1, 1}) == 4); // sorted: 1,1,90,90,90,98; min gap 0 appears 4 times (1-1, 90-90, 90-90)

    // All identical values
    assert(countMinimalGapPairs({5, 5, 5, 5}) == 3); // min gap 0, three adjacent pairs

    // Two elements
    assert(countMinimalGapPairs({10, 2}) == 1); // min gap 8, one pair

    // Empty input
    assert(countMinimalGapPairs({}) == 0);

    // Single element
    assert(countMinimalGapPairs({42}) == 0);

    // Negative and positive mixed
    assert(countMinimalGapPairs({-5, -1, -10, 2}) == 1); // sorted: -10,-5,-1,2; min gap 4 (-5 - -10), only one pair

    // Multiple pairs with same min gap
    assert(countMinimalGapPairs({1, 2, 3, 4}) == 3); // min gap 1, all three adjacent pairs

    // Large gap case
    assert(countMinimalGapPairs({100, 1, 50}) == 1); // sorted: 1,50,100; min gap 49 (50-1), only one pair

    return 0;
}
