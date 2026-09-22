/*
Write a C++ function `int absoluteDifferenceSum(const std::vector<int>& first, const std::vector<int>& second)` that takes two lists of integers (each list may contain duplicates and have different lengths) and returns the sum of the absolute differences between the frequency of each distinct integer in the first list and its frequency in the second list. For example, if `first = {1, 2, 2, 3}` and `second = {2, 3, 3, 4}`, then frequencies are: 1→1 vs 0 → |1-0|=1; 2→2 vs 1 → |2-1|=1; 3→1 vs 2 → |1-2|=1; 4→0 vs 1 → |0-1|=1; total = 4. The function should handle empty lists (returning 0), negative integers, and integers that appear in only one list.
*/
#include <vector>
#include <unordered_map>
#include <cstdlib> // for std::abs

// Returns the sum of absolute differences in frequency of each distinct integer
// between the two input lists.
int absoluteDifferenceSum(const std::vector<int>& first, const std::vector<int>& second) {
    std::unordered_map<int, int> freq;
    for (const auto& x : first) {
        ++freq[x];
    }
    for (const auto& x : second) {
        --freq[x];
    }
    int result = 0;
    for (const auto& p : freq) {
        result += std::abs(p.second);
    }
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Example from task
    std::vector<int> a = {1, 2, 2, 3};
    std::vector<int> b = {2, 3, 3, 4};
    assert(absoluteDifferenceSum(a, b) == 4);

    // Identical lists
    assert(absoluteDifferenceSum({1, 2}, {1, 2}) == 0);

    // One empty list
    assert(absoluteDifferenceSum({}, {5, 5}) == 2);
    assert(absoluteDifferenceSum({5, 5}, {}) == 2);
    assert(absoluteDifferenceSum({}, {}) == 0);

    // Negative numbers
    assert(absoluteDifferenceSum({-1, -1, 2}, {-1, 2, 3}) == 2); // -1:0, 2:0, 3:1

    // Completely disjoint sets
    assert(absoluteDifferenceSum({1, 1}, {2, 3}) == 4);

    // Duplicates in both but different counts
    assert(absoluteDifferenceSum({7, 7, 7}, {7, 7}) == 1);

    return 0;
}
// The solution counts the frequency of each integer in the first list, then subtracts the frequency from the second list for each element in the second list. This can be done using an `unordered_map<int, int>`: for each element in `first`, increment the map entry; for each element in `second`, decrement the map entry. After processing both lists, iterate over the map and sum the absolute value of each stored count (since positive means more occurrences in `first`, negative means more in `second`). This directly yields the sum of absolute differences in frequencies. Edge cases: empty lists produce an empty map and a sum of 0; if an integer appears in only one list, its map entry will be non-zero and its absolute value contributes correctly; negative integers are handled naturally because the map uses integer keys. Time complexity is O(n + m) where n and m are the sizes of the two lists, and space complexity is O(k) where k is the number of distinct integers across both lists.
