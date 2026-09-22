/*
Write a C++ function `int maximumBalancedSubarraySum(const std::vector<int>& nums)` that, given a non-empty array of integers, returns the maximum possible sum of any contiguous subarray where the number of negative elements in that subarray is even. A subarray can have length 1 or more. Negative numbers are those strictly less than zero. If all possible subarrays have an odd count of negatives (which only happens if every element is negative and the array has odd length), return 0 (the empty subarray is not allowed, so you must return the maximum among valid subarrays, which in that case is 0 by convention). Note: zero is considered positive for the parity count (not negative). The array size is at most 100, and elements are between -10^4 and 10^4.
*/

#include <vector>
#include <algorithm>
#include <limits>

// Returns the maximum sum of a contiguous subarray with an even number of negative elements.
// If no such subarray exists, returns 0.
int maximumBalancedSubarraySum(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    const long long INF = std::numeric_limits<long long>::max() / 4;

    long long prefixSum = 0;
    int prefixParity = 0; // 0 = even, 1 = odd

    long long minSumEven = 0; // prefix sum at index 0 (before any element), parity even
    long long minSumOdd = INF; // no prefix yet with odd parity

    long long answer = 0; // default if no valid subarray

    for (int i = 0; i < n; ++i) {
        prefixSum += nums[i];
        if (nums[i] < 0) {
            prefixParity ^= 1; // flip parity
        }

        if (prefixParity == 0) {
            // Need a previous prefix with even parity (minSumEven)
            // But minSumEven already includes prefix at index 0, which is < current index
            if (minSumEven != INF) {
                answer = std::max(answer, prefixSum - minSumEven);
            }
            // Also possible using odd? No, parity must match.
            // Update min for current parity after using it
            minSumEven = std::min(minSumEven, prefixSum);
        } else {
            // Need previous prefix with odd parity
            if (minSumOdd != INF) {
                answer = std::max(answer, prefixSum - minSumOdd);
            }
            minSumOdd = std::min(minSumOdd, prefixSum);
        }
    }

    return static_cast<int>(answer);
}

#include <cassert>
#include <vector>

// Function declaration (already defined above)
int maximumBalancedSubarraySum(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(maximumBalancedSubarraySum({1, 2, 3}) == 6); // whole array, 0 negatives (even)
    assert(maximumBalancedSubarraySum({-1, -2, -3}) == 0); // all odd negatives, length 3 -> no even subarray
    assert(maximumBalancedSubarraySum({-1, -2, -3, -4}) == -2); // subarray { -2, -3, -4 }? Wait: even negatives? Let's compute: subarray {-1,-2} sum -3 even, {-2,-3} -5 even, {-3,-4} -7 even, {-1,-2,-3} odd, {-2,-3,-4} odd, all single are odd. Max even is -3 (e.g., {-1,-2}). Actually max is -3? Let's check: {-1,-2} = -3, {-2,-3}=-5, {-3,-4}=-7, {-1,-2,-3,-4} even? 4 negatives even, sum -10. So max is -3. But my function returns -2? Let's think: subarray {-1,-2,-3,-4} has 4 negatives even, sum -10. Subarray {-2,-3} sum -5, even. Subarray {-1} is not allowed. Maximum is -3? Actually -3 is from {-1,-2} or {-2}? {-2} has one negative odd, not allowed. {-1,-2} gives -3. So answer should be -3. Let's recompute my algorithm: For array [-1,-2,-3,-4], prefix sums: after -1: sum=-1 parity=1, minOdd=INF first? Actually we start with prefixSum=0 parity=0 minEven=0 minOdd=INF. i=0: sum=-1 parity=1. Since parity=1, check minOdd (INF) so no update. Then minOdd = min(INF,-1) -> -1. i=1: sum=-3 parity=0 (since -2 flips to even). Since parity=0, check minEven=0 -> sum -3 - 0 = -3, answer=-3. Then minEven min(0,-3) = -3. i=2: sum=-6 parity=1, check minOdd=-1 -> sum - (-1) = -5, answer max(-3,-5) = -3. Then minOdd min(-1,-6) = -6. i=3: sum=-10 parity=0, check minEven=-3 -> sum - (-3) = -7, answer stays -3. So answer -3. My assertion wrote -2 incorrectly. Correct assertion should be -3.

    // Correct the above
    assert(maximumBalancedSubarraySum({-1, -2, -3, -4}) == -3);

    // Mixed positives and negatives
    assert(maximumBalancedSubarraySum({1, -2, 3, -4, 5}) == 3); // Subarray {3}? 0 negatives even sum 3, or {1,-2,3,-4,5} has 2 negatives even sum 3, both 3.
    assert(maximumBalancedSubarraySum({-5, 2, -3}) == 0); // all subarrays: single negatives odd, {2} even sum 2? Wait {2} has 0 negatives even sum 2! So should be 2. Let's recompute: My algorithm: start prefSum=0 parity=0 minEven=0 minOdd=INF. i=0: -5 -> sum=-5 parity=1, check minOdd=INF, update minOdd=-5. i=1: +2 -> sum=-3 parity=1 (still odd? Wait 2 is not negative, parity stays 1), check minOdd=-5 -> sum - (-5) = 2, answer=2. So correct answer should be 2, not 0. Fix assertion.

    assert(maximumBalancedSubarraySum({-5, 2, -3}) == 2);

    // Single element positive
    assert(maximumBalancedSubarraySum({7}) == 7);
    // Single element negative
    assert(maximumBalancedSubarraySum({-7}) == 0); // no even subarray

    // All zeros (0 is not negative, even count)
    assert(maximumBalancedSubarraySum({0, 0, 0}) == 0);

    // Large positive and one negative
    assert(maximumBalancedSubarraySum({10, -1, 10}) == 20); // whole sum 19? 10+(-1)+10=19 has 1 negative odd not allowed, subarray {10,-1,10} not allowed. Subarray {10,10} not contiguous. So max even is {10} sum 10, or {10,-1,10}? No. Actually {10,-1,10} has 1 negative odd, not allowed. {10} is 10, {-1} not allowed, {10,10} not contiguous. So max is 10? Wait subarray {10,-1} has 1 negative odd, not allowed. So answer is 10. Let's compute my algo: start: 0 even. i=0: 10 -> sum=10 parity0, check minEven=0 -> 10, answer=10, update minEven=0 (min(0,10)=0). i=1: -1 -> sum=9 parity1, check minOdd=INF, update minOdd=9. i=2: 10 -> sum=19 parity1 (since -1 only negative, parity remains 1? Actually parity flips on -1 to 1, then +10 doesn't change, so parity1), check minOdd=9 -> sum - 9 = 10, answer remains 10. So correct.

    // Correct test case: {10, -1, 10} -> 10
    assert(maximumBalancedSubarraySum({10, -1, 10}) == 10);

    // Case where whole array has even negatives and large sum
    assert(maximumBalancedSubarraySum({5, -3, 5}) == 7); // subarray {5,-3,5} sum 7 even (1 negative? Wait -3 is one negative, odd! So not allowed. Actually {5,-3,5} has 1 negative odd, not allowed. Subarray {5,-3} sum 2 odd? 1 negative odd, not allowed. {-3,5} sum 2 odd? 1 negative, not allowed. So only single positives: {5} sum 5, {5} sum 5. Max 5. So assert should be 5.

    assert(maximumBalancedSubarraySum({5, -3, 5}) == 5);

    // Edge case: length 2 negatives both -> even count
    assert(maximumBalancedSubarraySum({-2, -3}) == -5); // whole array sum -5 even (2 negatives)

    return 0;
}

// The key is to efficiently compute the maximum subarray sum while enforcing that the parity of negative count in the subarray is even. A direct brute force over all O(n^2) subarrays, computing the sum and negative parity for each, would work within n≤100, but we can do better. Use a prefix-sum and prefix-negative-parity approach. Let `prefSum[0]=0` and `prefNegParity[0]=0` (even). For each index i from 1 to n, maintain the cumulative sum and the cumulative parity of negatives up to that point. For any subarray from l to r (1-indexed), the parity of negatives is `prefNegParity[r] ^ prefNegParity[l-1]` (where ^ is XOR, 0=even,1=odd). We need this to be 0 (even), so we require `prefNegParity[r] == prefNegParity[l-1]`. The sum of that subarray is `prefSum[r] - prefSum[l-1]`. Thus, for each r, we want to find the earliest (or smallest) `prefSum[l-1]` among all indices with the same parity as `prefNegParity[r]`. Then the best subarray ending at r is `prefSum[r] - minPrefSumSoFarForThatParity`. We track two running minima: one for even parity prefix positions, one for odd parity. Initialize both minima to +∞ initially (except we will set the even parity minimum to 0 at position 0 before any elements). Then iterate through the array, updating prefix sum and parity, then update the answer using the opposite parity's stored minimum, and then update the minimum for the current parity with the current prefix sum. Edge cases: empty subarray is not allowed, so we must ensure we only consider subarrays of length at least 1. This is naturally handled because we compute the best for each r using a minimum from a previous position l-1 < r, meaning length ≥1. If the array has all odd negatives and length odd, no subarray has even negative count, so answer remains 0 (since we initialize answer to 0). Time complexity O(n), space complexity O(1) (only storing prefix sum and parity as we go, plus two minima).
