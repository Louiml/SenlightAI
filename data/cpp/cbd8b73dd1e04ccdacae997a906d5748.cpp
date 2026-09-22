Write a C++ function named `findLongestHarmoniousSubsequence` that takes a vector of integers (`std::vector<int>`) and returns the length of the longest harmonious subsequence. A harmonious subsequence is a subsequence where the difference between the maximum and minimum values in the subsequence is exactly 1. For example, in `[1,3,2,2,5,2,3,7]`, the longest harmonious subsequence is `[3,2,2,2,3]` (length 5). The input vector may be empty, contain duplicate values, and can have negative numbers. The function should return 0 if no such subsequence exists. The vector must not be modified, and the function should be const-correct and use appropriate standard library containers.

// The key insight is that for a subsequence to be harmonious, it must consist of only two consecutive integer values, say `x` and `x+1`, because any larger range would violate the condition of exactly 1 difference between max and min. Therefore, we can count the frequency of each distinct number using a hash map (`std::unordered_map`). Then, for each distinct value `x` present in the map, check if `x+1` is also present. If yes, the length of a harmonious subsequence using `x` and `x+1` is simply `freq[x] + freq[x+1]`. We take the maximum over all such pairs. This avoids iterating over the entire array repeatedly; instead, we iterate over the distinct keys in the map, which is at most the number of unique elements. Edge cases: an empty vector returns 0; if no consecutive pair exists (e.g., all values equal or gaps >1), return 0; negative numbers are handled naturally by the map; duplicate values are counted correctly. Time complexity is O(n) to build the map plus O(m) to scan distinct keys, where m ≤ n, so overall O(n). Space complexity is O(n) for the map.

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the length of the longest harmonious subsequence.
// A harmonious subsequence has max - min == 1.
int findLongestHarmoniousSubsequence(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    // Count frequencies of each distinct number.
    std::unordered_map<int, int> freq;
    for (int num : nums) {
        ++freq[num];
    }

    int longest = 0;
    // For each distinct value x, check if x+1 exists.
    for (const auto& [value, count] : freq) {
        auto nextIt = freq.find(value + 1);
        if (nextIt != freq.end()) {
            longest = std::max(longest, count + nextIt->second);
        }
    }

    return longest;
}

#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(findLongestHarmoniousSubsequence({1, 3, 2, 2, 5, 2, 3, 7}) == 5);
    assert(findLongestHarmoniousSubsequence({1, 2, 3, 4}) == 2);
    assert(findLongestHarmoniousSubsequence({1, 1, 1, 1}) == 0);
    assert(findLongestHarmoniousSubsequence({}) == 0);
    // Negative and duplicates
    assert(findLongestHarmoniousSubsequence({-1, -2, -2, -3, 0, 1}) == 3);
    assert(findLongestHarmoniousSubsequence({0, 0, 1, 1, 1, 2, 2}) == 5);
    // Large gap
    assert(findLongestHarmoniousSubsequence({1, 3, 5, 7}) == 0);
    // Single element
    assert(findLongestHarmoniousSubsequence({5}) == 0);
    // Only two consecutive values repeated
    assert(findLongestHarmoniousSubsequence({10, 11, 10, 11, 10}) == 5);

    return 0;
}
