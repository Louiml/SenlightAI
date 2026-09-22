Write a C++ function `int countDoublePairs(const std::vector<int>& nums)` that, given a non-empty list of positive integers (each between 1 and 10000), returns the total number of ordered pairs `(i, j)` such that `nums[i] == nums[j] * 2`. For each element in the list, check if its double exists anywhere in the list (including itself, though a number equal to its double cannot happen since all numbers are positive, except for 0 which is excluded). Count all such occurrences for every element. For example, for input `{1, 3, 2, 6, 4, 2}`, the doubles are: 1→2 (appears once), 3→6 (appears once), 2→4 (appears once), so total is 3. For input `{1, 2, 4}`, counts are 1→2 (1), 2→4 (1), 4→8 (0) → total 2. Do not modify the input; return the count as an integer. Assume the input has at least one element.
#include <cassert>
#include <vector>
#include "solution.h" // assume the function is declared here

int main() {
    // Basic case from the description
    std::vector<int> v1 = {1, 3, 2, 6, 4, 2};
    assert(countDoublePairs(v1) == 3);

    // Simple chain
    std::vector<int> v2 = {1, 2, 4};
    assert(countDoublePairs(v2) == 2);

    // No doubles present
    std::vector<int> v3 = {1, 3, 5};
    assert(countDoublePairs(v3) == 0);

    // All same numbers (1 appears many times, double 2 never appears)
    std::vector<int> v4 = {7, 7, 7};
    assert(countDoublePairs(v4) == 0);

    // Contains a pair where both directions exist? e.g., 2 and 4
    std::vector<int> v5 = {2, 4, 8, 16};
    // 2->4 (1), 4->8 (1), 8->16 (1), 16->32 (0) => 3
    assert(countDoublePairs(v5) == 3);

    // Single element
    std::vector<int> v6 = {5};
    assert(countDoublePairs(v6) == 0);

    // Larger test with duplicates
    std::vector<int> v7 = {1, 1, 2, 2, 4, 4};
    // For each 1 (2 of them): find 2 -> appears twice => 4 total
    // For each 2 (2 of them): find 4 -> appears twice => 4 total
    // For each 4 (2 of them): find 8 -> zero => 0
    // Total = 8
    assert(countDoublePairs(v7) == 8);

    return 0;
}
#include <vector>

// Count the number of ordered pairs (i, j) such that nums[i] == nums[j] * 2.
// For each element, search the entire vector for its double and sum counts.
int countDoublePairs(const std::vector<int>& nums) {
    int total = 0;
    const int n = static_cast<int>(nums.size());
    // Outer loop: pick each element as the "double" candidate.
    for (int i = 0; i < n; ++i) {
        int target = nums[i] * 2;
        // Inner loop: count how many times the double appears.
        for (int j = 0; j < n; ++j) {
            if (nums[j] == target) {
                ++total;
            }
        }
    }
    return total;
}
// The solution is straightforward: for each element `x` in the vector, compute its double `2*x`, then scan the entire vector to count occurrences of that double. Sum these counts over all elements. This brute-force approach works because the constraints are small (the original problem reads a list until 0, with typical sizes under a few hundred). Edge cases: if the double value is not present, the inner loop contributes zero; if the element itself is half of another element, it will be counted when the smaller element is processed; duplicates in the vector are handled naturally because the inner loop counts every occurrence of the double, and the outer loop processes each element (including duplicates) separately. No special case for self-doubling is needed because all numbers are positive, so `x != 2*x` unless `x == 0`, which is excluded. Time complexity is `O(n^2)` because for each of the `n` elements we do a full scan of `n` elements. Space complexity is `O(1)` beyond the input vector. If the problem were extended to large inputs, a hash map could reduce time to `O(n)`, but the task specification (based on the snippet) expects the straightforward double loop.
