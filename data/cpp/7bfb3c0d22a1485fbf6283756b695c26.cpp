/*
Write a C++ function `pairWithTargetSum` that takes a non-empty vector of integers and a target integer, and returns a vector containing the indices of the first pair of elements (from the left) whose sum equals the target. If multiple pairs exist, return the one with the smallest first index; if there is still a tie, return the one with the smallest second index. If no such pair exists, return {-1, -1}. The input vector may contain negative numbers, zeros, duplicates, and is not guaranteed to be sorted. The function must be `const` correct and not modify the input vector.
*/

#include <vector>

// Find the first pair of indices (i, j) with i < j such that nums[i] + nums[j] == target.
// Returns {i, j} if found, otherwise {-1, -1}.
std::vector<int> pairWithTargetSum(const std::vector<int>& nums, int target) {
    int n = static_cast<int>(nums.size());
    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (nums[i] + nums[j] == target) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

#include <cassert>
#include <vector>

// Declaration of the solution function (would be provided by the user's code)
std::vector<int> pairWithTargetSum(const std::vector<int>& nums, int target);

int main() {
    // Basic case
    assert(pairWithTargetSum({2, 7, 11, 15}, 9) == std::vector<int>({0, 1}));
    // Negative numbers and zeros
    assert(pairWithTargetSum({-3, 4, 0, 7}, 4) == std::vector<int>({0, 2}));
    // Duplicate values
    assert(pairWithTargetSum({3, 3, 3}, 6) == std::vector<int>({0, 1}));
    // No valid pair
    assert(pairWithTargetSum({1, 2, 3}, 10) == std::vector<int>({-1, -1}));
    // Single element (non-empty but cannot form a pair)
    assert(pairWithTargetSum({5}, 5) == std::vector<int>({-1, -1}));
    // Tie-breaking: first index smallest, then second index smallest
    // Multiple pairs: (0,3) sum=4, (1,2) sum=4 → choose (0,3) because first index 0 < 1
    assert(pairWithTargetSum({1, 2, 2, 3}, 4) == std::vector<int>({0, 3}));
    // Tie-breaking: same first index, choose smallest second index
    // Pairs: (0,2) sum=5, (0,3) sum=5 → choose (0,2)
    assert(pairWithTargetSum({1, 9, 4, 4}, 5) == std::vector<int>({0, 2}));
    // Large vector with no match at first but later
    assert(pairWithTargetSum({10, 20, 30, 40, 50}, 70) == std::vector<int>({2, 3}));
    // Both values negative
    assert(pairWithTargetSum({-5, -7, -3}, -10) == std::vector<int>({0, 1}));
    // Zero target with zeros and positives
    assert(pairWithTargetSum({0, 0, 5, -5}, 0) == std::vector<int>({0, 1}));
    
    return 0;
}

// The problem is a classic pair-sum search. The straightforward approach is to use two nested loops: the outer loop iterates over each index `i` from 0 to `n-2`, and the inner loop iterates over each index `j` from `i+1` to `n-1`. For each pair `(i, j)`, check if `nums[i] + nums[j] == target`. Since we iterate `i` in increasing order and, for a fixed `i`, we iterate `j` in increasing order, the first pair we encounter that satisfies the condition is guaranteed to have the smallest first index and, among those, the smallest second index. This matches the required tie-breaking rule precisely. Edge cases include: an empty vector (problem says non-empty, but we can still handle gracefully by returning `{-1,-1}`), a vector with only one element (impossible to form a pair, return `{-1,-1}`), duplicate values (they are allowed, and we still check indices individually), negative numbers (addition works normally), and no valid pair (return `{-1,-1}`). Time complexity is \(O(n^2)\) due to the nested loops, and space complexity is \(O(1)\) aside from the returned vector.
