Write a C++ function `long long houseRobberCircular(const std::vector<int>& nums)` that solves the "House Robber II" problem: given a circular array of non-negative integers representing the money in each house, return the maximum amount of money you can rob tonight without robbing two adjacent houses. Since the array is circular, the first and last houses are considered adjacent. The function should handle empty and single-element arrays appropriately. Use a helper that computes the maximum for a linear range, and take the maximum of two possible subproblems: robbing from house 0 to n−2 (exclude last) and robbing from house 1 to n−1 (exclude first). The returned value must be of type `long long` to avoid overflow, though inputs fit in `int`. Preserve `const` correctness on the input parameter.

#include <cassert>
#include <vector>

int main() {
    // Empty array
    assert(houseRobberCircular({}) == 0);
    // Single element
    assert(houseRobberCircular({5}) == 5);
    // Two elements (circular adjacency forces picking max)
    assert(houseRobberCircular({2, 3}) == 3);
    // Three elements: cannot pick both adjacent, and are all adjacent in circle
    assert(houseRobberCircular({1, 2, 3}) == 3); // pick 1 and 3? no, 1 and 3 are adjacent in circle, so pick max single = 3
    // Classic case: nums = {2,3,2} -> max is 3 (pick middle) or 2+2? no, adjacent, so 3
    assert(houseRobberCircular({2, 3, 2}) == 3);
    // Larger example: {1,2,3,1} -> max is 4 (pick 1 and 3? they are not adjacent in circle? indices 0 and 2 are not adjacent, but last and first are adjacent, so indeces 0 and 2 are not adjacent, so 1+3=4)
    assert(houseRobberCircular({1, 2, 3, 1}) == 4);
    // Large values
    assert(houseRobberCircular({1000000, 1, 2, 1000000}) == 1000002); // pick 0 and 2 (1000000+2) or 1 and 3 (1+1000000) => max 1000002
    // All zeros
    assert(houseRobberCircular({0, 0, 0, 0}) == 0);
    // Mixed case
    assert(houseRobberCircular({5, 3, 4, 11, 2}) == 16); // pick 0 (5), 3 (11) => 16? wait 0 and 3 are not adjacent in circle? 3 is adjacent to 2 and 4, so yes, 5+11=16
    return 0;
}

#include <vector>
#include <algorithm>

// Compute maximum robbery amount for a linear strip from start (inclusive) to end (exclusive).
long long robLinear(const std::vector<int>& nums, int start, int end) {
    long long prev2 = 0; // max up to two houses before current
    long long prev1 = 0; // max up to one house before current
    for (int i = start; i < end; ++i) {
        long long current = std::max(prev1, prev2 + static_cast<long long>(nums[i]));
        prev2 = prev1;
        prev1 = current;
    }
    return prev1;
}

// Return maximum amount robable from a circular array without robbing adjacent houses.
long long houseRobberCircular(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 0) return 0;
    if (n == 1) return static_cast<long long>(nums[0]);

    // Scenario 1: include first house, exclude last (indices 0 .. n-2)
    long long takeFirst = robLinear(nums, 0, n - 1);
    // Scenario 2: exclude first house, include last (indices 1 .. n-1)
    long long takeLast = robLinear(nums, 1, n);

    return std::max(takeFirst, takeLast);
}

// The core idea is to reduce the circular problem to two linear "House Robber I" problems. Because the first and last houses are adjacent in a circle, you cannot rob both. Therefore, you can consider two mutually exclusive scenarios: (1) exclude the last house and solve the linear chain from index 0 to n−2; (2) exclude the first house and solve the linear chain from index 1 to n−1. The answer is the maximum of these two results. For the linear chain, use dynamic programming with two state variables: `prev2` (the maximum up to two houses before) and `prev1` (the maximum up to one house before). Initialize with the first house’s value and 0 respectively, then iterate through the range. At each house, compute `current = max(house_value + prev2, prev1)`, then shift states. Edge cases: empty array returns 0; single-element array returns that element (because there is no adjacency issue). Time complexity is O(n) because each house is visited twice total (once per subproblem), and space is O(1) because only a constant number of variables are used.
