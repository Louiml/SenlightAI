Write a C++ function `int longestIncreasingSubsequence(const std::vector<int>& nums)` that returns the length of the longest strictly increasing subsequence (LIS) in the given array of integers. The array may be empty, may contain negative numbers, duplicates, or unsorted values. A subsequence is obtained by deleting some elements (possibly none) without changing the order of the remaining elements, and it must be strictly increasing (each element must be greater than the previous one). Your solution must be efficient for arrays up to \(10^5\) elements, so avoid exponential recursion. The function should be `const`-correct (take a const reference) and handle edge cases like a single element, all equal elements, or a fully decreasing array.

#include <cassert>
#include <vector>

// The function is declared above; include its definition here.

int main() {
    assert(longestIncreasingSubsequence({}) == 0);
    assert(longestIncreasingSubsequence({5}) == 1);
    assert(longestIncreasingSubsequence({3, 1, 2}) == 2); // [1,2] or [3]
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4); // [2,3,7,101]
    assert(longestIncreasingSubsequence({7, 7, 7, 7}) == 1); // strictly increasing
    assert(longestIncreasingSubsequence({0, 1, 0, 3, 2, 3}) == 4); // [0,1,2,3]
    assert(longestIncreasingSubsequence({5, 4, 3, 2, 1}) == 1);
    assert(longestIncreasingSubsequence({1, 3, 6, 7, 9, 4, 10, 5, 6}) == 6); // [1,3,6,7,9,10]
    assert(longestIncreasingSubsequence({2, 6, 3, 4, 1, 2, 9, 5, 8}) == 5); // [2,3,4,5,8]
    assert(longestIncreasingSubsequence({-5, -2, -1, -7, 0, 1}) == 5); // [-5,-2,-1,0,1]
}

#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    std::vector<int> tails; // tails[k] = smallest tail of any inc. subsequence of length k+1
    for (int x : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x); // extend the longest subsequence
        } else {
            *it = x; // replace with smaller tail to allow future growth
        }
    }
    return static_cast<int>(tails.size());
}

// The core idea is dynamic programming. We process the array from right to left (or left to right) and track the best possible LIS length for each index and each possible "previous" index (the index of the last chosen element, or -1 if none chosen). The recurrence is: for each position `i`, either skip `nums[i]` (keeping the previous index unchanged) or take `nums[i]` if it is greater than the previous chosen number (or if no previous number exists), then add 1 and move to `i+1` with `prev = i`. We take the maximum of taking or skipping. A straightforward top-down memoization would use \(O(n^2)\) time and space, which is too slow for \(n=10^5\). Instead, we use the well-known **greedy + binary search** algorithm: maintain an array `tails` where `tails[k]` is the smallest possible tail value of any increasing subsequence of length `k+1`. For each number in the input, we find the first position in `tails` where the value is greater than or equal to the current number (using `lower_bound`), and replace it. If no such position exists (current number is larger than all tails), we append it, increasing the LIS length. This ensures the tails array is always sorted, and the final length of `tails` is the LIS length. Edge cases: empty array returns 0; all decreasing array yields length 1; duplicates are handled correctly because we use `lower_bound` (strictly increasing, so equal values replace the existing tail). Time complexity is \(O(n \log n)\) and space \(O(n)\) for the tails vector.
