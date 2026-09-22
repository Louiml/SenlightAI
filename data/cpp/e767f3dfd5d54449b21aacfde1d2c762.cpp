Write a C++ function that takes a vector of integers and returns the length of the longest strictly increasing subsequence (LIS). The subsequence must maintain the original order of elements but does not need to be contiguous. If the input is empty, return 0. The function should handle duplicate values correctly (strictly increasing means equal values cannot be part of the same subsequence). For example, for input `{3, 10, 2, 1, 20}`, the LIS length is 3 (`3,10,20` or `2,10,20` or `1,10,20`). The solution must be efficient for arrays of size up to 10^5, meaning an O(n log n) approach is preferred over the classic O(n^2) dynamic programming.
The classic O(n^2) dynamic programming solution initializes each position with length 1 and for each i from 1 to n-1 checks all j < i, updating dp[i] = max(dp[i], dp[j]+1) if arr[i] > arr[j]. This works but is too slow for 10^5 elements. The optimal solution uses patience sorting: maintain a vector `tails` where `tails[k]` stores the smallest possible tail value for any increasing subsequence of length `k+1`. For each element `x`, we use binary search to find the first position in `tails` where the value is >= x. If found, we replace that position with x (because a smaller tail for that length is better). If not found (x is larger than all tails), we push x to the end, increasing the max length. The final size of `tails` is the LIS length. Edge cases: empty input returns 0; single element returns 1; duplicates are handled correctly because we replace the first tail >= x, so equal values don't extend a subsequence (strictly increasing requires >). Time complexity is O(n log n) due to binary search per element; space complexity is O(n) for the tails vector.
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
// Uses patience sorting with binary search for O(n log n) time.
int longestIncreasingSubsequenceLength(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    
    // tails[k] = smallest possible tail of an increasing subsequence of length k+1
    std::vector<int> tails;
    
    for (const int x : nums) {
        // Find first position where tails[pos] >= x (for strict increasing, we replace >=)
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        
        if (it == tails.end()) {
            tails.push_back(x); // x extends the longest subsequence found so far
        } else {
            *it = x; // replace to keep the smallest tail for that length
        }
    }
    
    return static_cast<int>(tails.size());
}
#include <cassert>
#include <vector>

// Declaration of the function under test (included from solution)
int longestIncreasingSubsequenceLength(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(longestIncreasingSubsequenceLength({}) == 0);
    assert(longestIncreasingSubsequenceLength({5}) == 1);
    assert(longestIncreasingSubsequenceLength({7, 7, 7}) == 1);  // duplicates don't extend
    assert(longestIncreasingSubsequenceLength({1, 2, 3, 4, 5}) == 5);
    assert(longestIncreasingSubsequenceLength({5, 4, 3, 2, 1}) == 1);
    
    // Classic LIS examples
    assert(longestIncreasingSubsequenceLength({3, 10, 2, 1, 20}) == 3);
    assert(longestIncreasingSubsequenceLength({50, 3, 10, 7, 40, 80}) == 4); // {3,7,40,80}
    assert(longestIncreasingSubsequenceLength({10, 9, 2, 5, 3, 7, 101, 18}) == 4); // {2,3,7,101} or {2,5,7,101}
    
    // Larger test with interleaved values
    std::vector<int> large = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
    assert(longestIncreasingSubsequenceLength(large) == 6); // {0,2,6,9,13,15} or similar
    
    // Performance check with 100k ascending numbers (should be fast)
    std::vector<int> bigAscending(100000);
    for (int i = 0; i < 100000; ++i) bigAscending[i] = i;
    assert(longestIncreasingSubsequenceLength(bigAscending) == 100000);
    
    return 0;
}
