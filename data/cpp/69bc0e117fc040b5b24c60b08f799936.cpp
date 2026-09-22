Write a C++ function that takes a vector of integers `nums` containing `n` distinct numbers in the range `[0, n]` (one number is missing) and returns the missing number. The function must be named `missingNumber` and should handle inputs where the vector size is at least 1. The input vector is modified inside the original snippet, but your function must not modify the input; instead, use a frequency marker approach without altering the original data.
The problem is a classic missing-number puzzle. Since the vector contains `n` distinct integers from `0` to `n` inclusive, exactly one number in that range is missing. The simplest correct approach is to mark which numbers are present using a boolean or integer marker array of size `n`. Iterate through the input vector, and for each value `v`, set `marker[v] = 1` (or `true`). Then scan the marker from index 0 upward until you find an index `i` where `marker[i] == 0`; that index is the missing number. Edge cases: if the input is `[0]`, the missing number is `1`; if the input is `[1]`, the missing number is `0`; if the input is `[0,1]`, the missing number is `2`. The algorithm runs in O(n) time and uses O(n) auxiliary space for the marker array. A more space-efficient alternative (using XOR or sum formulas) exists, but this marker approach is clear and directly parallels the given snippet.
#include <vector>

// Returns the missing number in a vector containing all distinct integers from 0..n except one.
// The vector must have size n and contain n distinct values from the range [0, n].
int missingNumber(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    // Marker array: index i is 1 if i is present in nums.
    std::vector<int> marker(n + 1, 0); // size n+1 to safely index value n (which may be present)
    for (int value : nums) {
        marker[value] = 1;
    }
    // Find the first index with marker == 0.
    for (int i = 0; i <= n; ++i) {
        if (marker[i] == 0) {
            return i;
        }
    }
    return -1; // Should never reach here for valid input.
}
#include <cassert>
#include <vector>

// Forward declaration of the function under test.
int missingNumber(const std::vector<int>& nums);

int main() {
    // Basic cases
    assert(missingNumber({0, 1, 3}) == 2);
    assert(missingNumber({0, 1, 2, 4}) == 3);
    assert(missingNumber({1}) == 0);
    assert(missingNumber({0}) == 1);
    // Larger case with missing 5
    assert(missingNumber({0, 1, 2, 3, 4, 6, 7}) == 5);
    // Missing 0
    assert(missingNumber({1, 2, 3}) == 0);
    // Missing n (the largest)
    assert(missingNumber({0, 1, 2, 3}) == 4);
    // Single-element vector missing 1
    assert(missingNumber({0}) == 1);
    // Check const correctness: input should not be modified
    std::vector<int> data = {3, 0, 1};
    assert(missingNumber(data) == 2);
    assert(data.size() == 3 && data[0] == 3 && data[1] == 0 && data[2] == 1);
    return 0;
}
