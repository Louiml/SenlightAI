// Write a C++ function that takes a vector of integers (only containing 0s and 1s) and returns the maximum number of consecutive 1s in the vector. The input vector may be empty, in which case the result should be 0. The function must handle cases where the array contains only 0s, only 1s, or a mix, and must correctly count consecutive runs of 1s across the entire array. Use a single pass through the array to count consecutive 1s, resetting the count when encountering a 0, and track the maximum count seen so far. The function should be named `longestOnes` and take a `const std::vector<int>&` parameter, returning an `int`.

The algorithm uses a linear scan of the array with two variables: `currentCount` that tracks the length of the current run of consecutive 1s, and `maxCount` that stores the maximum run length encountered so far. For each element, if it is 1, increment `currentCount`; otherwise, the run has ended, so update `maxCount` with `currentCount` (if larger) and reset `currentCount` to 0. After the loop, perform a final update of `maxCount` because a run might extend to the end of the array. Edge cases include an empty array (returns 0), an array of all zeros (currentCount never exceeds 0, so maxCount remains 0), and an array of all ones (currentCount equals array length, final update catches it). Complexity is O(n) time and O(1) auxiliary space.

#include <vector>
#include <algorithm>

// Return the maximum number of consecutive 1s in the input vector.
int longestOnes(const std::vector<int>& nums) {
    int currentCount = 0;
    int maxCount = 0;
    
    for (int value : nums) {
        if (value == 1) {
            ++currentCount;
        } else {
            maxCount = std::max(maxCount, currentCount);
            currentCount = 0;
        }
    }
    
    // Handle the case where the last run extends to the end of the vector.
    maxCount = std::max(maxCount, currentCount);
    return maxCount;
}

#include <cassert>
#include <vector>

int longestOnes(const std::vector<int>& nums);

int main() {
    assert(longestOnes({}) == 0);
    assert(longestOnes({0}) == 0);
    assert(longestOnes({1}) == 1);
    assert(longestOnes({0, 0, 0}) == 0);
    assert(longestOnes({1, 1, 1}) == 3);
    assert(longestOnes({1, 0, 1, 1, 0, 1}) == 2);
    assert(longestOnes({0, 1, 1, 1, 0, 1, 1}) == 3);
    assert(longestOnes({1, 1, 0, 1, 0, 0, 1, 1, 1, 1}) == 4);
    assert(longestOnes({1, 0, 1, 0, 1}) == 1);
    assert(longestOnes({1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1}) == 4);
}
