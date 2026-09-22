Write a standalone C++ function named `maximumRobbedValue` that takes a constant reference to a vector of non-negative integers representing the amount of money in each house along a street. The function must return the maximum amount of money that can be robbed without robbing two adjacent houses. The problem is a classic dynamic programming "House Robber" problem: you may select any subset of houses, but you cannot pick two consecutive indices. The function must handle an empty vector (return 0) and a vector with a single element (return that element). The input vector may contain zero values, and duplicates are allowed. You must implement an iterative solution (not recursion) for efficiency and avoid stack overflow on large inputs.

// The problem is solved using dynamic programming with a state that tracks the best result up to the current house. Let `prev2` represent the maximum amount that can be robbed from houses up to index `i-2`, and `prev1` represent the maximum up to index `i-1`. For each house at index `i`, the optimal choice is either to rob it (taking `nums[i] + prev2`) or skip it (keeping `prev1`). Thus the new best is `max(prev1, nums[i] + prev2)`. We update `prev2 = prev1` and `prev1 = currentBest` and proceed. Edge cases: empty vector returns 0; single element returns that element; zeros in the vector are harmless because they never improve the sum. The iterative solution runs in O(n) time and uses O(1) extra space, making it robust for large inputs. The original recursive snippet has exponential time and is not suitable; this task requires the iterative version.

#include <vector>
#include <algorithm>

// Returns the maximum sum of non-adjacent elements from the input vector.
// Complexity: O(n) time, O(1) auxiliary space.
int maximumRobbedValue(const std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }
    if (nums.size() == 1) {
        return nums[0];
    }

    int prev2 = nums[0];           // best up to index 0
    int prev1 = std::max(nums[0], nums[1]); // best up to index 1

    for (std::size_t i = 2; i < nums.size(); ++i) {
        int current = std::max(prev1, nums[i] + prev2);
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}

#include <cassert>
#include <vector>

int maximumRobbedValue(const std::vector<int>& nums); // declaration from solution

int main() {
    // Basic cases
    assert(maximumRobbedValue({}) == 0);
    assert(maximumRobbedValue({5}) == 5);
    assert(maximumRobbedValue({1, 2, 3, 1}) == 4);
    assert(maximumRobbedValue({2, 7, 9, 3, 1}) == 12);

    // All zeros
    assert(maximumRobbedValue({0, 0, 0, 0}) == 0);

    // Increasing and alternating patterns
    assert(maximumRobbedValue({1, 2, 3, 4, 5}) == 9); // 1+3+5
    assert(maximumRobbedValue({5, 1, 1, 5}) == 10); // 5+5

    // Large single value
    assert(maximumRobbedValue({100, 1, 1, 100}) == 200);
    assert(maximumRobbedValue({10, 14, 13, 20, 8}) == 34); // 14+20

    // Duplicate values
    assert(maximumRobbedValue({4, 4, 4, 4}) == 8);
    assert(maximumRobbedValue({3, 3, 3}) == 6);

    return 0;
}
