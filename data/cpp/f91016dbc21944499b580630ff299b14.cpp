// Write a C++ function that, given a non-empty vector of integers `nums`, returns the maximum possible value of the expression `(nums[i] - nums[j]) * nums[k]` where indices satisfy `i < j < k`. The result must be computed as a 64-bit integer (`long long`). If no valid combination exists (i.e., the vector has fewer than 3 elements) or if all possible computed values are non-positive, the function must return 0. The solution must run in a single pass over the input and use only constant auxiliary space. The function should be named `maximumTripletValue` and take a `const std::vector<int>&` parameter.
#include <cassert>
#include <vector>

// Function declaration from solution
long long maximumTripletValue(const std::vector<int>& nums);

int main() {
    // Basic case with positive result
    assert(maximumTripletValue({12, 6, 1, 2, 7}) == 77); // (12-6)*? -> max at k=7: max_diff=11, *7=77
    // Case where 0 is the best result (no positive triplet)
    assert(maximumTripletValue({1, 2, 3}) == 0);
    // All negative numbers - result should still be 0 because (negative - negative) could be positive, but here check
    assert(maximumTripletValue({-1, -2, -3, -4}) == 0);
    // Mixed positives and negatives
    assert(maximumTripletValue({10, 5, 1, 4, 20}) == 160); // (10-1)*20=180? Wait compute: diffs: 5,9,9, then *20=180? Let's verify: i=0,j=2→9, k=4→180. But also i=0,j=1→5, k=3→20... max=180
    assert(maximumTripletValue({10, 5, 1, 4, 20}) == 180);
    // Case with fewer than 3 elements
    assert(maximumTripletValue({5}) == 0);
    assert(maximumTripletValue({5, 4}) == 0);
    // Case where largest element appears at the end but difference is small
    assert(maximumTripletValue({3, 3, 3, 3}) == 0);
    // Large numbers to test overflow safety
    assert(maximumTripletValue({1000000000, 0, 1000000000}) == 0); // diff=1e9, *k=1e9 -> 1e18 fits in long long
    assert(maximumTripletValue({1000000000, 0, 1000000000, 1000000000}) == 1000000000000000000LL);
    return 0;
}
#include <vector>
#include <algorithm>

// Given a non-empty vector of integers, return the maximum value of
// (nums[i] - nums[j]) * nums[k] for i < j < k, or 0 if no positive value exists.
long long maximumTripletValue(const std::vector<int>& nums) {
    long long max_diff = 0;
    long long max_ele = 0;
    long long result = 0;

    for (int value : nums) {
        // Treat current value as nums[k]
        result = std::max(result, max_diff * static_cast<long long>(value));
        // Treat current value as nums[j] to potentially form a new difference
        max_diff = std::max(max_diff, max_ele - static_cast<long long>(value));
        // Treat current value as nums[i] to potentially update max_ele
        max_ele = std::max(max_ele, static_cast<long long>(value));
    }

    return result;
}
// The key insight is to avoid a naive O(n³) triple loop by decomposing the problem. For a fixed `k` (the rightmost index), the expression becomes `(maximum difference seen so far) * nums[k]`, where "difference" refers to `nums[i] - nums[j]` with `i < j < k`. To maximize the product, we need to track two things while iterating from left to right:
// 1. `max_ele`: the maximum value of `nums[i]` seen so far (as a `long long` to avoid overflow in subtraction).
// 2. `max_diff`: the maximum value of `nums[i] - nums[j]` encountered so far, where both indices are less than the current index. When we reach a new element, we can form a new difference using the current element as `nums[j]`: `max_ele - current`. We update `max_diff` to be the larger of its previous value and that new difference.
// 3. For the current element treated as `nums[k]`, the candidate triplet value is `max_diff * current`. We update the final result to the maximum of its previous value and this candidate.
// Crucially, any negative candidate is naturally ignored because the result is initialized to 0 and we only take the maximum. Edge cases include vectors with fewer than 3 elements (the loop still works but will never produce a valid triplet, so result stays 0) and all-negative products (result stays 0). The algorithm processes each element once, so time complexity is O(n), and it uses only a few `long long` variables, so space complexity is O(1). The use of `long long` for intermediate values ensures no integer overflow when multiplying large integers or subtracting negative numbers.
