// Write a C++ function named `longestRunOfOnes` that takes a `const std::vector<int>&` and returns the maximum number of consecutive `1`s appearing in the array. The input array may be empty, contain only `0`s, only `1`s, or a mix. The function must handle large arrays efficiently and must not modify the input. Return `0` if no `1`s are present.
The solution uses a single linear pass through the array. Maintain two integer variables: `currentRun` (count of consecutive `1`s seen so far) and `bestRun` (maximum run length recorded). For each element: if it is `1`, increment `currentRun` and update `bestRun` with `max(bestRun, currentRun)`; if it is `0`, reset `currentRun` to `0`. Edge cases: an empty array returns `0`; an array with only zeros returns `0`; an array of all ones returns the array length. The algorithm is optimal because every element must be examined at least once. Time complexity is O(n) where n is the number of elements. Space complexity is O(1) auxiliary (only two integer counters, no additional data structures).
#include <vector>
#include <algorithm>

// Returns the length of the longest consecutive run of 1s in the vector.
int longestRunOfOnes(const std::vector<int>& nums) {
    int currentRun = 0;
    int bestRun = 0;

    for (int value : nums) {
        if (value == 1) {
            ++currentRun;
            bestRun = std::max(bestRun, currentRun);
        } else {
            currentRun = 0;
        }
    }

    return bestRun;
}
#include <cassert>
#include <vector>

int main() {
    // Empty vector
    assert(longestRunOfOnes({}) == 0);
    // No ones
    assert(longestRunOfOnes({0, 0, 0}) == 0);
    // Single one
    assert(longestRunOfOnes({0, 1, 0}) == 1);
    // All ones
    assert(longestRunOfOnes({1, 1, 1, 1}) == 4);
    // Mixed with longest in middle
    assert(longestRunOfOnes({1, 1, 0, 1, 1, 1, 0, 1}) == 3);
    // Mixed with longest at start
    assert(longestRunOfOnes({1, 1, 1, 0, 1}) == 3);
    // Mixed with longest at end
    assert(longestRunOfOnes({0, 1, 0, 1, 1, 1}) == 3);
    // Multiple runs of equal length
    assert(longestRunOfOnes({1, 0, 1, 0, 1}) == 1);
    // Long alternating pattern
    assert(longestRunOfOnes({1, 0, 1, 0, 1, 0, 1, 0, 1}) == 1);
    // Large length
    std::vector<int> big(100000, 1);
    assert(longestRunOfOnes(big) == 100000);
    return 0;
}
