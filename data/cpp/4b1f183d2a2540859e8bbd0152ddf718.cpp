Write a standalone C++ function `checkContinuousSubarraySum(const std::vector<int>& nums, int k)` that returns `true` if the given integer vector contains a contiguous subarray of length at least two whose sum is a multiple of `k`, where `k` is a non-zero integer (you may assume `k != 0`). The function must handle arrays containing zeros, negative numbers, and large magnitudes. A subarray sum of `0` is considered a multiple of any `k` (including negative `k`). The function should be efficient for vectors up to length 10^5, and you must not use any external libraries beyond the standard C++ ones. The function should not modify the input vector, and should be declared with proper `const` correctness.

The core idea is to track prefix sums modulo `k` and check for repeated remainders. If the same remainder appears at two different indices `i` and `j` with `j - i >= 2`, then the subarray between them has sum exactly `(prefix[j] - prefix[i])` which is divisible by `k`. We use an unordered map to store the earliest index where each remainder was first seen. Initialize the map with remainder `0` at index `-1` (to handle subarrays starting at index 0 when length ≥ 2). Then iterate through the array, accumulating a running sum, compute `sum % k` (adjusting for negative remainders by adding `k` and taking modulo again to ensure non‑negative), and check if this remainder already exists in the map. If it exists and the distance from the stored index is at least 2, return `true`. If not, store the current index for that remainder if not already present. Edge cases: `k` can be negative; handle modulo carefully by normalizing. Also, zeros: if `k` is non-zero, `0 % k` is `0`, so two consecutive zeros will cause the same remainder `0` at indices `i-1` and `i` (distance 1) but we need distance ≥2, so we need original index logic; however, because we store earliest index, a subarray of two zeros will give remainders `0` at indices `i-1` and `i` with distance 1 (not good), but when we reach index `i+1` (third zero), the remainder is still `0` and stored index is `i-1`, distance 2 → good. So it works. Time complexity O(n), space O(min(n, k)) due to map, but in practice O(n).

#include <vector>
#include <unordered_map>

// Returns true if there exists a contiguous subarray of length at least 2
// whose sum is a multiple of k (k != 0). Uses prefix sum remainder tracking.
bool checkContinuousSubarraySum(const std::vector<int>& nums, int k) {
    // Map from prefix sum remainder to the earliest index where that remainder occurred.
    std::unordered_map<int, int> remainderToIndex;
    // Initialize with remainder 0 at index -1 to handle subarrays starting at index 0.
    remainderToIndex[0] = -1;

    int prefixSum = 0;
    const int n = static_cast<int>(nums.size());

    for (int i = 0; i < n; ++i) {
        prefixSum += nums[i];
        // Normalize remainder to be non-negative, even for negative k.
        int remainder = prefixSum % k;
        if (remainder < 0) {
            remainder += k;
        }

        if (remainderToIndex.find(remainder) != remainderToIndex.end()) {
            // If the same remainder appeared before and the gap is at least 2,
            // then the subarray sum between those indices is a multiple of k.
            if (i - remainderToIndex[remainder] >= 2) {
                return true;
            }
        } else {
            // Only store the first occurrence of this remainder.
            remainderToIndex[remainder] = i;
        }
    }
    return false;
}

#include <cassert>
#include <vector>

// The solution function is declared above.
bool checkContinuousSubarraySum(const std::vector<int>& nums, int k);

int main() {
    // Basic example from the snippet.
    assert(checkContinuousSubarraySum({23, 2, 4, 6, 7}, 6) == true);
    // Subarray [2,4] sums to 6 (multiple of 6).
    assert(checkContinuousSubarraySum({1, 2, 3}, 6) == true); // [1,2,3] sum=6
    // No subarray of length >=2 sums to a multiple.
    assert(checkContinuousSubarraySum({1, 2}, 4) == false);
    // Single element is not enough (length must be >=2).
    assert(checkContinuousSubarraySum({6}, 6) == false);
    // Two consecutive zeros: sum 0 is a multiple of any k.
    assert(checkContinuousSubarraySum({0, 0}, 5) == true);
    // Negative numbers and negative k.
    assert(checkContinuousSubarraySum({-1, -2, -3}, -3) == true); // [-1,-2] sum=-3
    // Larger array with a match not at start.
    assert(checkContinuousSubarraySum({5, 0, 0, 7}, 5) == true); // [0,0] sum=0
    // No match.
    assert(checkContinuousSubarraySum({1, 1, 1}, 5) == false);
    // k can be large, remainders wrap correctly.
    assert(checkContinuousSubarraySum({1000000000, 1000000000}, 1000000000) == false); // sum=2e9, not multiple
    assert(checkContinuousSubarraySum({1000000000, 1000000000, 1000000000}, 1000000000) == true); // first two sum=2e9, second+third=2e9
    return 0;
}
