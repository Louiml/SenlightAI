// Write a C++ function named `maximumTripletValue` that takes a vector of integers `nums` (with length at least 3) and returns the maximum value of `(nums[i] - nums[j]) * nums[k]` for all triplets of indices `i < j < k`. If all possible products are negative, the function must return 0 (as the problem considers “no valid positive value” as equivalent to a maximum of 0). The function should be `long long`-returning to handle potentially large intermediate and final products. The input vector is not modified, and the solution must run in a single pass over the array.

#include <cassert>
#include <vector>

// Declaration from the solution (already included above).
long long maximumTripletValue(const std::vector<int>& nums);

int main() {
    // Basic example from typical problem.
    assert(maximumTripletValue({12, 6, 1, 2, 7}) == 77);
    // All negative products -> return 0.
    assert(maximumTripletValue({1, 2, 3}) == 0);
    // Singly increasing, best triplet uses (i=0, j=1, k=2) gives negative -> 0.
    assert(maximumTripletValue({1, 10, 2}) == 0);
    // Larger positive difference and large third element.
    assert(maximumTripletValue({10, 1, 100}) == 900);
    // Negative numbers: ( -5 - (-10) ) * 20 = 100.
    assert(maximumTripletValue({-5, -10, 20}) == 100);
    // Duplicates.
    assert(maximumTripletValue({5, 5, 5}) == 0);
    // Minimum length 3.
    assert(maximumTripletValue({3, 2, 1}) == 0);
    // Longer array, multiple candidates.
    assert(maximumTripletValue({100, 200, 300, 400, 500}) == 0);
    // Large values to check long long overflow handling.
    assert(maximumTripletValue({1000000000, 0, 1000000000}) == 1000000000000000000LL);
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum value of (nums[i] - nums[j]) * nums[k] for i < j < k.
// Returns 0 if all possible products are negative.
long long maximumTripletValue(const std::vector<int>& nums) {
    long long result = 0;
    long long maxDiff = 0;       // best (nums[i] - nums[j]) seen so far
    long long maxi = nums[0];    // best nums[i] seen so far

    for (size_t k = 1; k < nums.size(); ++k) {
        // Use current k as the third index.
        result = std::max(result, maxDiff * static_cast<long long>(nums[k]));
        // Update best difference using current k as the second index.
        maxDiff = std::max(maxDiff, maxi - static_cast<long long>(nums[k]));
        // Update best first element.
        maxi = std::max(maxi, static_cast<long long>(nums[k]));
    }

    return result;
}

// The key insight is to avoid enumerating all O(n³) triplets. We process the array from left to right, maintaining three running values:  
// - `maxi`: the maximum value seen so far (for position `i`), initialized to `nums[0]`.  
// - `maxDiff`: the maximum `(nums[i] - nums[j])` seen so far where `i < j` (both strictly before the current index), initialized to `maxi - nums[1]` (or we can initialize to a very negative value and update as we go).  
// - `result`: the best product found so far, initialized to 0.
//
// For each new element `nums[k]` (as the third element of the triplet), the best product using this `k` is `maxDiff * nums[k]`. We update `result` with that candidate. Then, we update `maxDiff` to consider this element as the second element: `maxDiff = max(maxDiff, maxi - nums[k])`. Finally, we update `maxi` to consider this element as the first element: `maxi = max(maxi, nums[k])`.  
//
// Because we update `maxDiff` *after* computing the product for the current `k`, we ensure that `maxDiff` represents differences with `j < k`. This is a greedy dynamic programming style: we keep the best possible difference available for each future `k`.  
//
// Edge cases:  
// - If all products are negative, `result` remains 0, matching the specification.  
// - If the array has exactly 3 elements, the loop correctly considers only that triplet.  
// - Negative numbers are handled correctly because we always take `max`, which preserves the largest (least negative) difference and product.  
// - The use of `long long` prevents overflow when multiplying large values (e.g., 10^9 * 10^9 = 10^18, which fits in `long long`).  
//
// Time complexity: O(n) for a single pass. Space complexity: O(1) auxiliary space.
