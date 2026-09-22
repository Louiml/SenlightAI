Write a C++ function named `longestIncreasingSubsequence` that takes a `std::vector<int>` (which may be empty, contain duplicates, or contain negative numbers) and returns the length of the longest strictly increasing subsequence (LIS). A subsequence is obtained by deleting zero or more elements without changing the order of the remaining elements; “strictly increasing” means each element must be greater than the previous one. The function should be `const`-correct and must not modify the input. For an empty vector, the function should return 0. The solution must be efficient for large inputs (up to 10^5 elements) and must not rely on brute-force enumeration.
// The standard efficient approach to LIS uses a patience-sorting-like greedy algorithm with binary search. We maintain a vector `tails` where `tails[k]` stores the smallest possible tail value of an increasing subsequence of length `k+1`. For each element `x` in the input, we find the first position in `tails` such that `tails[pos] >= x` using `std::lower_bound`. If such a position exists, we replace it with `x` (since for the same length, a smaller tail is better). Otherwise, `x` is larger than all current tails, so we append it. The length of `tails` after processing all elements is the LIS length. 
//
// Edge cases: 
// - If the input is empty, we return 0.
// - Duplicates: strictly increasing means equal values cannot extend subsequence, so `lower_bound` (which finds first `>=`) correctly prevents using equal values to extend. For example, `[2,2]` returns 1.
// - Negative numbers are handled naturally since comparisons are value-based.
// Time complexity: O(n log n) due to binary search for each element. Space complexity: O(n) for `tails` vector.
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestIncreasingSubsequence(const std::vector<int>& nums) {
    if (nums.empty()) return 0;
    
    std::vector<int> tails;
    for (int x : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) {
            tails.push_back(x);
        } else {
            *it = x;
        }
    }
    return static_cast<int>(tails.size());
}
#include <cassert>
#include <vector>

int longestIncreasingSubsequence(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(longestIncreasingSubsequence({}) == 0);
    assert(longestIncreasingSubsequence({5}) == 1);
    assert(longestIncreasingSubsequence({1, 2, 3}) == 3);
    assert(longestIncreasingSubsequence({3, 2, 1}) == 1);
    
    // Strictly increasing with duplicates
    assert(longestIncreasingSubsequence({2, 2, 2}) == 1);
    assert(longestIncreasingSubsequence({1, 1, 2, 2, 3}) == 3);
    
    // Negative numbers and mixed
    assert(longestIncreasingSubsequence({-5, -1, -10, 0}) == 3);  // -5, -1, 0 or -10, 0? Actually -5, -1, 0 is length 3.
    assert(longestIncreasingSubsequence({0, -1, 2, -2, 3}) == 3); // 0,2,3 or -1,2,3
    
    // Classic LIS example
    assert(longestIncreasingSubsequence({10, 9, 2, 5, 3, 7, 101, 18}) == 4); // 2,3,7,101 or 2,5,7,101 etc.
    
    // Larger test
    std::vector<int> large(1000, 1);
    assert(longestIncreasingSubsequence(large) == 1);
    
    // Random increasing
    std::vector<int> inc;
    for (int i = 0; i < 10000; i++) inc.push_back(i);
    assert(longestIncreasingSubsequence(inc) == 10000);
    
    return 0;
}
