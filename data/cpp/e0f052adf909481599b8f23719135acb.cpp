// Write a C++ function that takes a vector of integers `nums`, a vector of integer queries `queries`, and an integer `x` as inputs. For each query value `q` in `queries`, the function must return the index of the first element in `nums` where the value equals `x`, considering only the occurrences of `x` in order. Specifically, query `q` asks for the index of the `q`-th occurrence of `x` in `nums` (1-indexed counting of occurrences). If the `q`-th occurrence does not exist, return `-1` for that query. The function should return a vector of integers where each element corresponds to the result for the respective query, maintaining the same order as in `queries`. For example, if `nums = {1, 2, 3, 2, 4, 2}`, `queries = {1, 3, 5}`, and `x = 2`, the output should be `{1, 3, -1}` because the first occurrence of 2 is at index 1, the third occurrence is at index 5, and there is no fifth occurrence. Your implementation must handle cases where `nums` is empty, `queries` is empty, or `x` does not appear in `nums`. Ensure the function is efficient for large inputs (up to 10^5 elements and 10^5 queries). Do not modify the input vectors.
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Basic case with multiple occurrences
    std::vector<int> nums1 = {1, 2, 3, 2, 4, 2};
    std::vector<int> q1 = {1, 2, 3, 4, 5};
    std::vector<int> r1 = occurrencesOfElement(nums1, q1, 2);
    assert((r1 == std::vector<int>{1, 3, 5, -1, -1}));

    // x not present
    std::vector<int> nums2 = {5, 6, 7};
    std::vector<int> q2 = {1, 2};
    std::vector<int> r2 = occurrencesOfElement(nums2, q2, 10);
    assert((r2 == std::vector<int>{-1, -1}));

    // Empty nums
    std::vector<int> nums3 = {};
    std::vector<int> q3 = {1, 3};
    std::vector<int> r3 = occurrencesOfElement(nums3, q3, 1);
    assert((r3 == std::vector<int>{-1, -1}));

    // Empty queries
    std::vector<int> nums4 = {1, 1, 2};
    std::vector<int> q4 = {};
    std::vector<int> r4 = occurrencesOfElement(nums4, q4, 1);
    assert(r4.empty());

    // First occurrence at index 0, and x appears only once
    std::vector<int> nums5 = {7, 8, 9};
    std::vector<int> q5 = {1, 2};
    std::vector<int> r5 = occurrencesOfElement(nums5, q5, 7);
    assert((r5 == std::vector<int>{0, -1}));

    // Negative numbers and large indices
    std::vector<int> nums6 = {-1, -2, -1, -3, -1};
    std::vector<int> q6 = {1, 2, 3};
    std::vector<int> r6 = occurrencesOfElement(nums6, q6, -1);
    assert((r6 == std::vector<int>{0, 2, 4}));

    // Queries with zero or negative values should always return -1
    std::vector<int> nums7 = {1, 1};
    std::vector<int> q7 = {0, -1, 3};
    std::vector<int> r7 = occurrencesOfElement(nums7, q7, 1);
    assert((r7 == std::vector<int>{-1, -1, -1}));

    // Single element, x present
    std::vector<int> nums8 = {42};
    std::vector<int> q8 = {1};
    std::vector<int> r8 = occurrencesOfElement(nums8, q8, 42);
    assert((r8 == std::vector<int>{0}));

    // All elements equal x, many queries
    std::vector<int> nums9(100, 5);
    std::vector<int> q9 = {1, 50, 100, 101};
    std::vector<int> r9 = occurrencesOfElement(nums9, q9, 5);
    assert((r9 == std::vector<int>{0, 49, 99, -1}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <vector>
#include <cstddef>

// Returns the index of the q-th occurrence of x in nums, or -1 if it doesn't exist.
// Queries are 1-indexed: q=1 means first occurrence, q=2 means second, etc.
std::vector<int> occurrencesOfElement(const std::vector<int>& nums, const std::vector<int>& queries, int x) {
    // Collect the indices where nums[i] == x, in increasing order.
    std::vector<int> positions;
    positions.reserve(nums.size());
    for (std::size_t i = 0; i < nums.size(); ++i) {
        if (nums[i] == x) {
            positions.push_back(static_cast<int>(i));
        }
    }

    // Answer each query by direct lookup in positions.
    std::vector<int> result;
    result.reserve(queries.size());
    for (int q : queries) {
        if (q >= 1 && static_cast<std::size_t>(q) <= positions.size()) {
            result.push_back(positions[static_cast<std::size_t>(q - 1)]);
        } else {
            result.push_back(-1);
        }
    }
    return result;
}
// The core idea is to precompute the prefix counts of `x` as we scan `nums` from left to right, storing the cumulative count in a separate vector `prefix_counts` of the same length. For each index `i`, `prefix_counts[i]` represents how many times `x` has appeared up to and including position `i`. Then, for a query `q`, we need to find the smallest index `i` such that `prefix_counts[i] == q`. Since `prefix_counts` is non-decreasing (because counts only increase or stay the same), we can use binary search to locate the first index where the count equals `q`. Specifically, use a standard binary search for the lower bound of `q` in `prefix_counts`. If that position has exactly `q` occurrences, then that index is the answer; otherwise, if the value at that position is not equal to `q` (meaning there are fewer than `q` occurrences), return `-1`. Alternatively, we can store the indices of occurrences directly (e.g., a list `positions` where `positions[k]` is the index of the `(k+1)`-th occurrence), then each query `q` is answered by checking if `q - 1 < positions.size()` and returning `positions[q-1]`, else `-1`. The prefix-count approach with binary search is also valid, but the direct positions list is simpler and more efficient (O(1) per query after O(n) precomputation). Edge cases include: empty `nums` (no positions, all queries return `-1`), empty `queries` (return empty vector), and `x` not present (all queries return `-1`). Time complexity is O(n + m) where n is length of `nums` and m is length of `queries` if we use the positions list, or O(n + m log n) if using binary search on prefix counts. Space complexity is O(n) for the positions list (or prefix counts). We will use the positions-list approach for clarity and optimal performance.
