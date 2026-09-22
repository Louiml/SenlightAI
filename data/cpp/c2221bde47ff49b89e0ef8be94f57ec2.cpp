// Write a C++ function named `houseRobber` that takes a `std::vector<int>` representing the amount of money stashed in each house along a street, where adjacent houses have security systems that alert the police if two adjacent houses are robbed on the same night. The function must return the maximum amount of money that can be robbed tonight without alerting the police. You cannot rob two adjacent houses, but you may rob any non-adjacent selection, including all houses if there is only one. The input vector is guaranteed non-empty and may contain any non-negative integers. The function must be `const`-correct (accept a const reference) and must not modify the input. Implement an iterative dynamic programming solution with constant extra space.
// The problem is the classic "House Robber" dynamic programming problem. The core idea is to maintain two state variables: `prev2` (the maximum loot up to the house two positions back) and `prev1` (the maximum loot up to the previous house). For each current house with value `n`, the best we can do is either skip it (keep `prev1`) or rob it (take `prev2 + n`); we take the maximum. Then we shift the states: `prev2` becomes the old `prev1`, and `prev1` becomes the new best. This works because the choice for house `i` only depends on the best solutions up to `i-1` and `i-2`. Initializing both `prev2` and `prev1` to `0` correctly handles the empty prefix and ensures that for the first house, the max is just its value. Edge cases: a single element yields the value itself (since we start with zeros, the first iteration gives max(0+n,0)=n). All zeros returns zero. The algorithm runs in O(n) time and O(1) auxiliary space, which is optimal.
#include <vector>
#include <algorithm>

/**
 * @brief Returns the maximum amount of money that can be robbed without robbing adjacent houses.
 * 
 * @param nums A non-empty vector of non-negative integers representing money in each house.
 * @return int The maximum achievable loot.
 */
int houseRobber(const std::vector<int>& nums) {
    int prev2 = 0; // best up to i-2
    int prev1 = 0; // best up to i-1
    for (int n : nums) {
        int curr = std::max(prev2 + n, prev1);
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
#include <cassert>
#include <vector>
#include "houseRobber.h" // Assume the solution is in this header or replace with direct include

int main() {
    assert(houseRobber({1, 2, 3, 1}) == 4);
    assert(houseRobber({2, 7, 9, 3, 1}) == 12);
    assert(houseRobber({5}) == 5);
    assert(houseRobber({0}) == 0);
    assert(houseRobber({0, 0, 0}) == 0);
    assert(houseRobber({1, 1, 1, 1}) == 2);
    assert(houseRobber({10, 1, 1, 10}) == 20);
    assert(houseRobber({100, 1, 1, 100}) == 200);
    assert(houseRobber({1, 3, 1, 3, 100}) == 103);
    assert(houseRobber({2, 1, 1, 2}) == 4);
    return 0;
}
