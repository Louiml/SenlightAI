// Write a C++ function named `longestStrictlyIncreasingSubsequence` that accepts a `const std::vector<int>&` and returns the length of the longest strictly increasing subsequence (LIS). A strictly increasing subsequence is a sequence of indices `i1 < i2 < ... < ik` such that `nums[i1] < nums[i2] < ... < nums[ik]`. The function must handle empty vectors (returning 0) and vectors with all equal elements (returning 1 if non-empty). You may not modify the input vector; use `const` references. The solution must be efficient enough for vectors up to 10^5 elements, so avoid exponential or polynomial-with-high-degree approaches. Provide only the function implementation; no `main` or test harness.
The optimal approach uses a greedy + binary search algorithm, often called the patience sorting method. We maintain a vector `tails` where `tails[i]` is the smallest possible tail value of any increasing subsequence of length `i+1`. Iterate through each number in the input: use `std::lower_bound` on `tails` to find the first element that is `>=` the current number. If found, replace that element with the current number, which keeps the tails as small as possible while preserving lengths. If not found (i.e., the current number is larger than all tails), append it to extend the longest subsequence. After processing all elements, the size of `tails` is the length of the LIS. This is correct because maintaining minimal tails ensures that we can always extend a subsequence optimally. Edge cases: empty vector returns 0; all equal values produce a tails vector of size 1 because `lower_bound` will find the first element and replace it, never appending since the value is never greater than all existing. Time complexity is O(n log n) due to binary search for each element, and space complexity is O(n) in the worst case for the `tails` vector. The greedy replacement does not affect the correctness of the length, only the tail values.
#include <vector>
#include <algorithm>

// Returns the length of the longest strictly increasing subsequence.
int longestStrictlyIncreasingSubsequence(const std::vector<int>& nums) {
    std::vector<int> tails; // tails[i] = smallest tail of any increasing subsequence of length i+1
    
    for (int num : nums) {
        auto it = std::lower_bound(tails.begin(), tails.end(), num);
        if (it == tails.end()) {
            tails.push_back(num); // extend the longest subsequence
        } else {
            *it = num; // replace to keep tails minimal
        }
    }
    
    return static_cast<int>(tails.size());
}
#include <cassert>
#include <vector>

// The solution function declaration (included here for completeness)
int longestStrictlyIncreasingSubsequence(const std::vector<int>& nums);

int main() {
    // Basic examples
    assert(longestStrictlyIncreasingSubsequence({10,9,2,5,3,7,101,18}) == 4);
    assert(longestStrictlyIncreasingSubsequence({0,1,0,3,2,3}) == 4);
    
    // Edge cases
    assert(longestStrictlyIncreasingSubsequence({}) == 0);
    assert(longestStrictlyIncreasingSubsequence({5}) == 1);
    assert(longestStrictlyIncreasingSubsequence({7,7,7}) == 1);
    
    // Strictly increasing and decreasing sequences
    assert(longestStrictlyIncreasingSubsequence({1,2,3,4,5}) == 5);
    assert(longestStrictlyIncreasingSubsequence({5,4,3,2,1}) == 1);
    
    // Duplicates and mixed
    assert(longestStrictlyIncreasingSubsequence({1,3,3,2,4}) == 3); // 1,2,4 or 1,3,4
    assert(longestStrictlyIncreasingSubsequence({-2,-1,0}) == 3);
    assert(longestStrictlyIncreasingSubsequence({2,1,2,1,2}) == 2); // either 1,2
    
    // Large values with negative numbers
    assert(longestStrictlyIncreasingSubsequence({-100, 0, 100, 200}) == 4);
    
    return 0;
}
