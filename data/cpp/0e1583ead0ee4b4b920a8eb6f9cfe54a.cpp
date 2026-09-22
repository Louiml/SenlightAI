Given a non-empty vector of integers, write a C++ function that returns the minimum number of moves required to make all elements equal, where a single move increments `n-1` elements by 1 (i.e., all but one chosen element). The input vector may contain negative numbers, duplicates, and large values. The function must handle vectors of any size, including when all elements are already equal (answer 0). The result should be an integer and must fit within a 32-bit signed integer.
#include <cassert>

int main() {
    std::vector<int> v1 = {1, 2, 3};
    assert(minMovesToEqualize(v1) == 3);

    std::vector<int> v2 = {1, 1, 1};
    assert(minMovesToEqualize(v2) == 0);

    std::vector<int> v3 = {5, 5, 5, 5};
    assert(minMovesToEqualize(v3) == 0);

    std::vector<int> v4 = {-3, -1, -2};
    assert(minMovesToEqualize(v4) == 3); // min=-3, diffs: 0+2+1=3

    std::vector<int> v5 = {0, 10, 20};
    assert(minMovesToEqualize(v5) == 30);

    std::vector<int> v6 = {100, 100, 0};
    assert(minMovesToEqualize(v6) == 200); // min=0, diffs: 100+100+0=200

    std::vector<int> v7 = {7}; // single element
    assert(minMovesToEqualize(v7) == 0);

    std::vector<int> v8 = {-5, -5, -5};
    assert(minMovesToEqualize(v8) == 0);

    std::vector<int> v9 = {2, 2, 2, 2, 2};
    assert(minMovesToEqualize(v9) == 0);

    std::vector<int> v10 = {3, 0, 0, 0};
    assert(minMovesToEqualize(v10) == 3); // min=0, diffs: 3+0+0+0=3

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstddef>

// Computes the minimum number of moves to make all elements equal,
// where each move increments all but one element by 1.
int minMovesToEqualize(std::vector<int>& nums) {
    if (nums.empty()) {
        return 0;
    }

    // Find the minimum element in the vector.
    const int minVal = *std::min_element(nums.begin(), nums.end());

    // Total moves = sum of (each element - minimum).
    // Since each move reduces the max by 1 (relative difference), sum of differences works.
    long long totalMoves = 0;
    for (const int& value : nums) {
        totalMoves += static_cast<long long>(value) - minVal;
    }

    // The final answer is guaranteed to fit in int for valid inputs.
    return static_cast<int>(totalMoves);
}
// The key insight is that incrementing `n-1` elements by 1 is equivalent to decrementing the one unchosen element by 1 (relative to the others) because we care about differences, not absolute values. Thus, the problem reduces to bringing all elements down to the minimum element: each move can decrease exactly one element by 1 (the one not incremented), so the total number of moves equals the sum of differences between each element and the minimum. For example, `[1,2,3]` → min=1, differences: 0+1+2=3 moves. The algorithm first finds the minimum element, then accumulates `nums[i] - mn` for all `i`. Edge cases: empty vector is invalid per spec (non-empty), but if provided, returning 0 or handling gracefully is a design choice—we assume non-empty. Duplicate values naturally contribute 0. Time complexity is O(n) with a single pass to find min and another to sum, or combined if we compute min first (still O(n)). Space complexity is O(1) auxilary.
