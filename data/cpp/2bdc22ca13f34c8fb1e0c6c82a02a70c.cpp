Write a standalone C++ function `minimumRepeatedSubarrayLength` that takes a vector of integers as input and returns the length of the shortest contiguous subarray that contains at least one pair of equal elements at its two ends. If no such subarray exists (i.e., all elements are distinct), return -1. The subarray is defined by indices `i` and `j` (with `i < j`) such that `arr[i] == arr[j]`, and its length is `j - i + 1`. The function must consider every possible pair of equal elements in the array and return the minimum length among all such pairs. If the array has length 1, return -1 because no pair exists. The array may contain duplicate values, negative numbers, and the elements can be arbitrarily large. The function must handle large inputs efficiently.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minimumRepeatedSubarrayLength({1, 2, 3}) == -1);
    assert(minimumRepeatedSubarrayLength({5}) == -1);
    assert(minimumRepeatedSubarrayLength({1, 1}) == 2);
    assert(minimumRepeatedSubarrayLength({1, 2, 1}) == 3);
    assert(minimumRepeatedSubarrayLength({1, 2, 3, 2, 1}) == 3); // subarray [2,3,2] length 3
    assert(minimumRepeatedSubarrayLength({1, 2, 3, 4, 2, 5}) == 4); // [2,3,4,2]
    assert(minimumRepeatedSubarrayLength({1, 1, 1}) == 2); // [1,1] at start
    assert(minimumRepeatedSubarrayLength({1, 2, 3, 4, 5, 1}) == 6);
    assert(minimumRepeatedSubarrayLength({-1, -2, -1, 0}) == 3);
    
    // Larger test
    std::vector<int> large(100000, 0);
    assert(minimumRepeatedSubarrayLength(large) == 2);
    
    return 0;
}
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum length of a contiguous subarray whose first and last
// elements are equal. Returns -1 if no such subarray exists.
int minimumRepeatedSubarrayLength(const std::vector<int>& arr) {
    if (arr.size() < 2) {
        return -1;
    }

    std::unordered_map<int, int> last_seen; // value -> last index
    int min_length = arr.size() + 1; // sentinel larger than any possible answer

    for (int i = 0; i < static_cast<int>(arr.size()); ++i) {
        int val = arr[i];
        auto it = last_seen.find(val);
        if (it != last_seen.end()) {
            int length = i - it->second + 1;
            min_length = std::min(min_length, length);
        }
        last_seen[val] = i;
    }

    return (min_length == arr.size() + 1) ? -1 : min_length;
}
// The core idea is to track, for each distinct value, the most recent index where that value appeared. We iterate through the array from left to right. For each element, if we have seen the same value before, then the distance from the previous occurrence to the current index (inclusive) is a candidate subarray length. We keep the minimum such length across all occurrences. The key simplification: to get the shortest possible subarray for a given value, we only need to compare each occurrence with its immediate previous occurrence, because any earlier occurrence would yield a longer subarray. This is because if `arr[i] == arr[j]` with `i < j`, then also `arr[i] == arr[k]` for any `k` between them if there's another occurrence; but the closest previous occurrence gives the smallest window for that value. Therefore, we maintain an unordered_map from value to the most recent index where it appeared. When we see a value again, compute `current_index - previous_index + 1` and update the global minimum. After processing, if no update happened, return -1. Time complexity is O(n) on average (with unordered_map) and O(n) worst-case with ordered map. Space complexity is O(n) in the worst case for storing up to n distinct values. Edge cases: empty array? The task assumes non-empty; if provided empty, handle gracefully by returning -1 (though specification implies at least length 1). Single element returns -1. Duplicates far apart still produce correct length. Negative and large integers work fine.
