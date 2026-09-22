/*
Write a C++ function named `maxRobbedAmount` that takes a vector of non-negative integers representing the money in each house along a street, and returns the maximum amount of money a robber can take without robbing two adjacent houses. The function must handle empty input (returning 0) and inputs with up to 100 houses, each with a value from 0 to 400. The solution must use constant auxiliary space, meaning you cannot allocate arrays or vectors proportional to the number of houses.
*/
#include <vector>
#include <algorithm>

// Returns the maximum amount of money that can be robbed without taking adjacent houses.
int maxRobbedAmount(const std::vector<int>& nums) {
    int prev_max = 0;  // Max amount up to the house before the previous one
    int curr_max = 0;  // Max amount up to the previous house
    
    for (int money : nums) {
        int new_max = std::max(curr_max, prev_max + money);
        prev_max = curr_max;
        curr_max = new_max;
    }
    return curr_max;
}
#include <cassert>
#include <vector>

int main() {
    // Empty input
    assert(maxRobbedAmount({}) == 0);
    // Single house
    assert(maxRobbedAmount({5}) == 5);
    // Two houses: must choose the larger one
    assert(maxRobbedAmount({2, 3}) == 3);
    // Example 1 from problem
    assert(maxRobbedAmount({1, 2, 3, 1}) == 4);
    // Example 2 from problem
    assert(maxRobbedAmount({2, 7, 9, 3, 1}) == 12);
    // All zeros
    assert(maxRobbedAmount({0, 0, 0}) == 0);
    // Alternating large values
    assert(maxRobbedAmount({10, 1, 1, 10}) == 20); // rob house 1 and 4
    // Long sequence with maximum values (400)
    std::vector<int> many(100, 400);
    assert(maxRobbedAmount(many) == 20000); // 50 houses * 400
    return 0;
}
// This is the classic "House Robber" dynamic programming problem. Let `dp[i]` represent the maximum amount that can be robbed from the first `i+1` houses. For each house, you have two choices: either skip it (so the total remains `dp[i-1]`), or rob it (so the total is `dp[i-2] + nums[i]`). The recurrence is `dp[i] = max(dp[i-1], dp[i-2] + nums[i])`. Since each step only depends on the previous two values, we can reduce space to O(1) by maintaining two variables: `prev_max` (the maximum up to the previous house) and `curr_max` (the maximum up to the current house). We iterate once, updating both in constant time. Edge cases: when the vector is empty, return 0; when it has one element, return that element; when values are zero or repeated, the max operation naturally handles them. Time complexity is O(n) because we scan the array once, and space complexity is O(1) since we only store two integers.
