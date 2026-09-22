/*
Given a vector of integers that may contain duplicates and be unsorted, write a C++ function `int longestConsecutiveSubsequence(const std::vector<int>& nums)` that returns the length of the longest sequence of consecutive integers that can be formed from the elements of the vector. For example, given `{100, 4, 200, 1, 3, 2}`, the longest consecutive sequence is `{1, 2, 3, 4}`, so the function should return `4`. The function must handle an empty vector (returning `0`) and vectors with all identical elements (returning `1`). The algorithm must run in linear time on average and use linear auxiliary space. You may assume the input vector is not modified.
*/

#include <vector>
#include <unordered_set>
#include <algorithm>

// Return the length of the longest consecutive sequence in the input vector.
int longestConsecutiveSubsequence(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    std::unordered_set<int> values(nums.begin(), nums.end());
    int maxLength = 1;

    for (int num : values) {
        // Only start counting from the beginning of a sequence.
        if (values.find(num - 1) == values.end()) {
            int current = num;
            int length = 1;

            while (values.find(current + 1) != values.end()) {
                ++current;
                ++length;
            }

            maxLength = std::max(maxLength, length);
        }
    }

    return maxLength;
}

#include <cassert>
#include <vector>

// The solution function is declared above.
int main() {
    // Basic example from the prompt.
    std::vector<int> v1 = {100, 4, 200, 1, 3, 2};
    assert(longestConsecutiveSubsequence(v1) == 4);

    // Empty vector.
    std::vector<int> v2;
    assert(longestConsecutiveSubsequence(v2) == 0);

    // All identical elements.
    std::vector<int> v3 = {7, 7, 7};
    assert(longestConsecutiveSubsequence(v3) == 1);

    // Single element.
    std::vector<int> v4 = {5};
    assert(longestConsecutiveSubsequence(v4) == 1);

    // Negative numbers and duplicates.
    std::vector<int> v5 = {-3, -2, -1, 0, 1, 1, -2, 5};
    assert(longestConsecutiveSubsequence(v5) == 5); // -3,-2,-1,0,1

    // Non-consecutive large gaps.
    std::vector<int> v6 = {10, 20, 30, 40};
    assert(longestConsecutiveSubsequence(v6) == 1);

    // Mixed with a long run in the middle.
    std::vector<int> v7 = {0, 3, 7, 2, 5, 8, 4, 6, 0, 1};
    assert(longestConsecutiveSubsequence(v7) == 9); // 0..8

    return 0;
}

// The key idea is to identify the start of each possible consecutive sequence. If we place all elements into a hash set (or `std::unordered_set` for average O(1) lookups), then for any element `x` where `x-1` is not present, `x` must be the first element of a consecutive run. Starting from such an `x`, we can count how many consecutive numbers exist by repeatedly checking if `x+1`, `x+2`, ... are present. This ensures that each element is visited at most twice: once when checking if it is a start, and once when being part of a run counted from its start. Edge cases include an empty vector (return 0), a vector with one or more identical elements (the longest run length is 1), and negative numbers (handled naturally by the set). The time complexity is O(n) on average (due to hash set operations) and O(n) space for the set. The solution avoids sorting, which would be O(n log n).
