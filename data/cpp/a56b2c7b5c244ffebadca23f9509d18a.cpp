Write a C++ function `int firstMissingPositive(std::vector<int>& nums)` that accepts a vector of integers (which may contain negatives, zeros, duplicates, and values larger than the vector size) and returns the smallest positive integer (starting from 1) that is not present in the vector. The function must solve the problem in-place with O(1) extra space (excluding the input) and O(n) time on average, using an array-based cyclic bucket sort approach similar to the provided `bucket_sort` and `firstMissingPositive` methods in the snippet. The input vector is passed by reference, and after the function returns, the vector may be reordered; no additional data structures (like sets or hash maps) are allowed.

// The core idea is to place each positive integer `x` in the range `[1, n]` (where `n` is the vector's size) at its correct index `x-1`, using cyclic swapping. We iterate through all positions. For each position `i`, while the current value `nums[i]` is a positive integer between 1 and `n`, and it is not already at its correct position (i.e., `nums[i] != nums[nums[i]-1]`), we swap `nums[i]` with `nums[nums[i]-1]`. This places each number in the range into its correct bucket. After the reordering, we scan the vector from index 0 upward and return the first index `i` where `nums[i] != i+1`; the missing positive is `i+1`. If all positions 1..n are occupied correctly, the missing positive is `n+1`. Edge cases include empty vectors (return 1), all negatives/zeros (return 1), duplicates (break the loop when the target position already holds the same value to avoid infinite loops), and values out of range (break immediately). Since each swap places at least one number into its correct final position and we only move forward, the total number of swaps across the entire loop is at most `n`, so the time complexity is O(n). Space complexity is O(1) since we only use a few integer variables.

#include <vector>
#include <algorithm>

int firstMissingPositive(std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());

    // Cyclic bucket sort: place each positive integer in [1, n] at its correct index.
    for (int i = 0; i < n; ++i) {
        while (nums[i] > 0 && nums[i] <= n && nums[i] != nums[nums[i] - 1]) {
            std::swap(nums[i], nums[nums[i] - 1]);
        }
    }

    // Find the first index where the value does not match i+1.
    for (int i = 0; i < n; ++i) {
        if (nums[i] != i + 1) {
            return i + 1;
        }
    }

    // All numbers 1..n are present; the missing one is n+1.
    return n + 1;
}

#include <cassert>
#include <vector>

// The solution function is declared above (included via separate compilation or header).

int main() {
    // Basic cases.
    std::vector<int> v1 = {3, 4, -1, 1};
    assert(firstMissingPositive(v1) == 2);

    std::vector<int> v2 = {1, 2, 0};
    assert(firstMissingPositive(v2) == 3);

    std::vector<int> v3 = {7, 8, 9, 11, 12};
    assert(firstMissingPositive(v3) == 1);

    // Edge cases.
    std::vector<int> v4 = {};
    assert(firstMissingPositive(v4) == 1);

    std::vector<int> v5 = {0};
    assert(firstMissingPositive(v5) == 1);

    std::vector<int> v6 = {-5, -1, 0};
    assert(firstMissingPositive(v6) == 1);

    // Duplicates and large values.
    std::vector<int> v7 = {1, 1, 1, 2, 2, 3};
    assert(firstMissingPositive(v7) == 4);

    std::vector<int> v8 = {2, 2, 2, 2};
    assert(firstMissingPositive(v8) == 1);

    // All from 1 to n present.
    std::vector<int> v9 = {3, 1, 2};
    assert(firstMissingPositive(v9) == 4);

    // Single positive value missing.
    std::vector<int> v10 = {1};
    assert(firstMissingPositive(v10) == 2);

    return 0;
}
