/*
Write a C++ function that takes a non-empty vector of integers and returns the smallest absolute difference between any two distinct elements in the sorted order. If the vector has fewer than two elements, return -1. The function must operate on a `const std::vector<int>&` and return an `int`. The absolute difference is defined as `|a - b|` for any two indices `i != j`. You should not modify the input vector, and must handle negative and duplicate values correctly. For example, given `{5, 2, 5}`, the minimum absolute difference is `0` (between the two 5's). Given `{10, 3, 7}`, the answer is `3` (between 7 and 10). Given a single element, return `-1`.
*/

#include <vector>
#include <algorithm>
#include <cstdlib>
#include <climits>
#include <cmath> // for std::abs (though cstdlib also has it)

// Returns the smallest absolute difference between any two distinct elements,
// or -1 if fewer than two elements exist.
int minAbsoluteDifference(const std::vector<int>& nums) {
    const std::size_t n = nums.size();
    if (n < 2) {
        return -1;
    }

    // Copy and sort to allow O(n) scanning after sort without modifying input.
    std::vector<int> sorted(nums);
    std::sort(sorted.begin(), sorted.end());

    int best = INT_MAX;
    for (std::size_t i = 0; i + 1 < n; ++i) {
        const int diff = std::abs(sorted[i + 1] - sorted[i]);
        best = std::min(best, diff);
    }

    return best;
}

#include <cassert>
#include <vector>

int minAbsoluteDifference(const std::vector<int>& nums);

int main() {
    assert(minAbsoluteDifference({1, 2, 3, 4}) == 1);
    assert(minAbsoluteDifference({5, 2, 5}) == 0);
    assert(minAbsoluteDifference({10, 3, 7}) == 3);
    assert(minAbsoluteDifference({-5, -1, -10}) == 4); // -5 - (-1) = 4
    assert(minAbsoluteDifference({100}) == -1);
    assert(minAbsoluteDifference({0, 1000000}) == 1000000);
    assert(minAbsoluteDifference({-3, -3, 5}) == 0);
    assert(minAbsoluteDifference({1, 1, 1}) == 0);
    assert(minAbsoluteDifference({4, 1, 8, 1}) == 0);
    assert(minAbsoluteDifference({-1000000000, 1000000000}) == 2000000000);
    return 0;
}

// The solution sorts a copy of the input vector to bring adjacent elements together. After sorting, the minimum absolute difference between any two distinct elements must occur between some adjacent pair in the sorted sequence, because if the minimum were between non-adjacent elements, then at least one element between them would be even closer to one of them, contradicting minimality. Initialize the answer to a large positive value (e.g., `INT_MAX`) to handle cases where the difference might be large. Iterate through the sorted vector from index `0` to `n-2`, computing `abs(sorted[i+1] - sorted[i])` and updating the answer with `std::min`. If the vector size is less than 2, return `-1` immediately. Edge cases include duplicate values (which produce a difference of `0`), negative numbers (absolute value handles sign), and large integer ranges (using `std::abs` on `int` safely – if differences could overflow `int`, it's better to cast to `long long` before subtracting, but typical constraints fit within `int`). Time complexity is `O(n log n)` due to sorting, and space complexity is `O(n)` for the copy (or `O(1)` extra if we sort in-place, but we must not modify the original, so we make a copy).
