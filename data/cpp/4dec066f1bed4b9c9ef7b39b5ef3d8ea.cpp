/*
Write a C++ function that, given a non-empty vector of integers representing the types of fruit on a row of trees (where each integer is a fruit type), returns the maximum number of fruits you can collect if you start at any tree and move right, but you can only carry fruits of at most two distinct types at any time. You must collect exactly one fruit from each tree you pass, and once you pick a fruit of a third type, you cannot continue further from that point onward (you must stop before that tree). The function should handle vectors of any length, including very large ones, and fruit type values that are arbitrary non-negative integers (not necessarily contiguous). The result is always at least 1 because the vector is non-empty. Do not modify the input vector.
*/

#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the maximum number of fruits collectable from a row of trees,
// where at most two distinct fruit types can be collected consecutively.
int maxFruitsInTwoBaskets(const std::vector<int>& fruits) {
    if (fruits.empty()) return 0;
    std::unordered_map<int, int> freq;
    int start = 0;
    int max_len = 0;
    const int n = static_cast<int>(fruits.size());

    for (int end = 0; end < n; ++end) {
        ++freq[fruits[end]];

        while (freq.size() > 2) {
            auto it = freq.find(fruits[start]);
            if (--(it->second) == 0) {
                freq.erase(it);
            }
            ++start;
        }

        max_len = std::max(max_len, end - start + 1);
    }
    return max_len;
}

#include <cassert>
#include <vector>

// Function declaration (implementation would be included above)
int maxFruitsInTwoBaskets(const std::vector<int>& fruits);

int main() {
    // Single element
    assert(maxFruitsInTwoBaskets({5}) == 1);

    // All same type
    assert(maxFruitsInTwoBaskets({3, 3, 3, 3}) == 4);

    // Two types alternating -> whole array
    assert(maxFruitsInTwoBaskets({1, 2, 1, 2, 1}) == 5);

    // Three types: longest subarray with at most 2 distinct types is 3
    assert(maxFruitsInTwoBaskets({1, 2, 3, 2, 2}) == 4); // subarray [1,2] length 2, or [2,3,2,2] length 4

    // More complex: [3,3,3,1,2,1,1,2,3,3,4]
    // Longest subarray with at most 2 distinct: [1,2,1,1,2] length 5, or [3,3,3,1] length 4, or [2,3,3] length 3
    assert(maxFruitsInTwoBaskets({3,3,3,1,2,1,1,2,3,3,4}) == 5);

    // Large vector with only two types repeated
    std::vector<int> large(100000, 0);
    for (int i = 0; i < 100000; i += 2) large[i] = 0;
    for (int i = 1; i < 100000; i += 2) large[i] = 7;
    assert(maxFruitsInTwoBaskets(large) == 100000);

    // All distinct types -> max length is 2
    assert(maxFruitsInTwoBaskets({1,2,3,4,5}) == 2);

    // Mixed with repeated triple type at the end
    assert(maxFruitsInTwoBaskets({1,1,2,2,3,3,3}) == 5); // subarray [1,1,2,2] length 4, or [2,2,3,3,3] length 5

    return 0;
}

// The problem is a classic sliding window (two-pointer) problem. We maintain a window `[start, end]` that contains at most two distinct fruit types. As we move the `end` pointer right, we add the current fruit type to a frequency map. If the map has at most two keys, the current window is valid, and we update the maximum length as `end - start + 1`. If the map has more than two keys, we shrink the window from the left by moving `start` right, decrementing the frequency of the fruit at `start`, and erasing that key when its frequency reaches zero, until the map again has at most two keys. After shrinking, we do **not** need to update `max_len` because the window is now smaller or equal to a previously valid one. Edge cases: vector length 1 (return 1), all fruits same type (return n), exactly two types alternating (return n), and many repeats of a few types (the window will still correctly find the longest subarray with at most two distinct values). Time complexity is O(n) because each element is added and removed at most once; space complexity is O(1) in the number of distinct fruit types (at most 3 at any time, but theoretically O(k) if we treat k as number of distinct types, but since we cap at 3 in the map, it's effectively constant).
