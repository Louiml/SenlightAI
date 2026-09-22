Write a C++ function `bool canFormPairs(std::vector<int> values, int x)` that takes an even-length vector of integers and a non-negative integer `x`. The function must return `true` if the integers can be split into `n/2` pairs such that for every pair `(a, b)` with `a ≤ b`, the condition `b ≥ a + x` holds; otherwise return `false`. You may reorder the elements freely. The input vector length is guaranteed to be even and at least 2. The answer must be deterministic and based solely on the multiset of values.

#include <cassert>
#include <vector>

// Declaration of the solution function
bool canFormPairs(std::vector<int> values, int x);

int main() {
    // Basic even array with x=1
    assert(canFormPairs({1, 2, 3, 4}, 1) == true);  // pair (1,2), (3,4)
    assert(canFormPairs({1, 1, 1, 1}, 1) == false); // differences 0 < 1
    assert(canFormPairs({1, 1, 1, 1}, 0) == true);  // x=0 always true
    
    // Larger difference requirement
    assert(canFormPairs({10, 1, 5, 20}, 9) == false); // best: (1,10) diff 9, (5,20) diff 15, but need >=9 for all? actually (1,10) diff 9 OK, but (5,20) diff 15 OK, but sorted pair: (1,5) diff 4 fails, (10,20) diff 10 OK → false
    assert(canFormPairs({1, 5, 10, 20}, 9) == true); // pairs (1,10) diff 9, (5,20) diff 15
    
    // Duplicates and zero
    assert(canFormPairs({2, 2, 2, 2}, 2) == false); // all diffs 0
    assert(canFormPairs({2, 2, 2, 2}, 0) == true);
    
    // Negative values? The condition is b >= a + x, x non-negative, so negatives work
    assert(canFormPairs({-5, -1, 0, 4}, 3) == true); // sorted: -5,-1,0,4 → pairs (-5,0) diff 5, (-1,4) diff 5, both >=3
    assert(canFormPairs({-5, -1, 0, 4}, 6) == false); // pair (-5,0) diff 5 < 6
    
    // Odd number of elements? Not allowed, but we don't test.
    
    // Single pair
    assert(canFormPairs({3, 10}, 7) == true);
    assert(canFormPairs({3, 10}, 8) == false);
    
    // Large vector with many duplicates
    std::vector<int> big(1000, 1);
    assert(canFormPairs(big, 0) == true);
    assert(canFormPairs(big, 1) == false);
    
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstddef>

// Determine if the vector can be partitioned into pairs (a,b) with b >= a + x.
// Assumes values.size() is even and >= 2.
bool canFormPairs(std::vector<int> values, int x) {
    const std::size_t n = values.size();
    std::sort(values.begin(), values.end());

    const std::size_t half = n / 2;
    for (std::size_t i = 0; i < half; ++i) {
        if (values[i + half] < values[i] + x) {
            return false;
        }
    }
    return true;
}

// The key observation is that to satisfy the condition for all pairs, the optimal pairing strategy is to sort the array and pair the smallest half with the largest half in order. Specifically, after sorting the array `a[0..n-1]` (0-indexed), pair `a[i]` with `a[i + n/2]` for `i = 0..n/2-1`. This maximizes the minimum difference across all pairs because it pairs the smallest element with the smallest possible partner that is at least as large as the middle element, ensuring no larger element is wasted on a too-small partner that could have been used to satisfy a tighter constraint. If this greedy pairing fails (some pair has `a[i+n/2] < a[i] + x`), then no other pairing can succeed, because any alternative pairing would only decrease the minimum difference for some pair. Edge cases: duplicate values are allowed; `x` can be 0 (then any pairing works, so always true); the vector size is even by guarantee. Complexity: sorting runs in `O(n log n)`, checking takes `O(n)`, so total time `O(n log n)` and auxiliary space `O(1)` (excluding the sort’s internal stack).
