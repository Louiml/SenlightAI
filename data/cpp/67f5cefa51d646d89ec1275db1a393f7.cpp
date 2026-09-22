Write a C++ function that takes a vector of integers and returns the length of the longest contiguous subarray (non-empty) whose sum equals zero. If no such subarray exists, the function must return 0. The input vector is not modified, and the function must be efficient for large inputs (e.g., up to 10^5 elements). Handle edge cases such as an empty vector, all zeros, only positive numbers, and multiple zero-sum subarrays of differing lengths. The function signature is: `int longestZeroSumSubarray(const std::vector<int>& arr);`

#include <cassert>
#include <vector>

int main() {
    // Empty array
    assert(longestZeroSumSubarray({}) == 0);
    // Single zero
    assert(longestZeroSumSubarray({0}) == 1);
    // Single non-zero
    assert(longestZeroSumSubarray({5}) == 0);
    // All zeros
    assert(longestZeroSumSubarray({0, 0, 0}) == 3);
    // Mixed with a zero-sum subarray in the middle
    assert(longestZeroSumSubarray({1, 2, -3, 4}) == 3);
    // Longest is whole array
    assert(longestZeroSumSubarray({1, -1, 2, -2}) == 4);
    // No zero-sum subarray
    assert(longestZeroSumSubarray({1, 2, 3}) == 0);
    // Negative numbers only
    assert(longestZeroSumSubarray({-1, -2, -3}) == 0);
    // Multiple candidates, pick longest
    assert(longestZeroSumSubarray({1, -1, 5, -5, 0}) == 5);
    // Classic case with prefix sum repeated
    assert(longestZeroSumSubarray({1, 2, -2, -1, 3}) == 4); // subarray [2, -2, -1, 3] sum=2? Actually check: 1+2-2-1=0 length 4? Wait 1+2-2-1=0 yes length 4
    return 0;
}

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the length of the longest contiguous subarray with sum zero.
// Returns 0 if no such subarray exists.
int longestZeroSumSubarray(const std::vector<int>& arr) {
    std::unordered_map<int, int> prefixSumToIndex;
    int maxLen = 0;
    int sum = 0;

    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        sum += arr[i];

        if (sum == 0) {
            maxLen = std::max(maxLen, i + 1);
        } else {
            auto it = prefixSumToIndex.find(sum);
            if (it != prefixSumToIndex.end()) {
                maxLen = std::max(maxLen, i - it->second);
            } else {
                prefixSumToIndex[sum] = i;
            }
        }
    }

    return maxLen;
}

// The brute-force O(n²) approach (as in the snippet) checks every starting and ending pair, which fails for large inputs. A better algorithm uses a hash map to store the first occurrence index of each prefix sum. As we iterate through the array, we maintain a running sum. If the current sum is 0, the subarray from index 0 to the current index has sum 0, so its length is `i + 1`. If the current sum has been seen before at some earlier index `j` (where the prefix sum was the same), then the subarray from `j + 1` to `i` has sum 0, giving length `i - j`. We only store the first occurrence of each prefix sum to maximize the length for future matches. We track the maximum length across all such occurrences. Edge cases: empty vector returns 0; a single zero element returns 1 (since sum 0 → length 1); arrays with no zero-sum subarray return 0. Time complexity is O(n) and space complexity is O(n) for the map.
