// Write a C++ function named `houseRobber` that accepts a vector of non-negative integers representing the amount of money in each house along a street. Adjacent houses have a security system that will alert the police if two adjacent houses are robbed on the same night. The function must return the maximum amount of money that can be robbed tonight without alerting the police. The input vector may be empty (return 0) or contain any number of elements, and the function should handle large inputs efficiently. The solution must be a free function, not a class method, and should use `const` references where appropriate.
// This is the classic "House Robber" dynamic programming problem. The optimal strategy is to consider each house sequentially and maintain two values: `prev1` (the maximum robbery amount considering up to the previous house, where the previous house was not necessarily robbed) and `prev2` (the maximum robbery amount considering up to the second-to-last house). For each new house with value `num`, we compute the current maximum as `max(prev1, prev2 + num)` because we either skip this house (keep `prev1`) or rob it (add `num` to `prev2`, since we cannot rob adjacent houses). After the update, `prev2` becomes the old `prev1`, and `prev1` becomes the new current maximum. The answer for an empty vector is 0, and for a single-element vector it simplifies naturally since the loop will correctly compute `max(0, 0+num)` = `num`. This approach runs in O(n) time and uses O(1) auxiliary space, making it optimal for large inputs. Edge cases include empty input, single element, all zeros, and strictly increasing or decreasing values.
#include <vector>
#include <algorithm>

// Return the maximum robbable amount without robbing adjacent houses.
int houseRobber(const std::vector<int>& nums) {
    int prev1 = 0;  // max robbery amount up to previous house
    int prev2 = 0;  // max robbery amount up to two houses back

    for (int num : nums) {
        int current = std::max(prev1, prev2 + num);
        prev2 = prev1;
        prev1 = current;
    }

    return prev1;
}
#include <cassert>
#include <vector>

int houseRobber(const std::vector<int>& nums);

int main() {
    assert(houseRobber({}) == 0);
    assert(houseRobber({5}) == 5);
    assert(houseRobber({1, 2, 3, 1}) == 4);                      // Rob houses 1 and 3
    assert(houseRobber({2, 7, 9, 3, 1}) == 12);                  // Rob houses 2 and 5
    assert(houseRobber({2, 1, 1, 2}) == 4);                      // Rob houses 1 and 4
    assert(houseRobber({0, 0, 0, 0}) == 0);
    assert(houseRobber({10, 1, 1, 10}) == 20);                   // Rob first and last
    assert(houseRobber({1, 3, 1, 3, 100}) == 103);               // Rob house 2 and 5
    assert(houseRobber({5, 4, 3, 2, 1}) == 9);                   // Rob houses 1, 3, 5
    assert(houseRobber({1, 1, 1, 1, 1}) == 3);                   // Rob three non-adjacent houses
    return 0;
}
