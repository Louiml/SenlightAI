// Given an array of integers of length \( n \) representing heights, write a C++ function `int longestValidWindow(const std::vector<int>& heights)` that returns the maximum length of a contiguous subarray such that the first element of the subarray is strictly greater than every other element in that subarray. In other words, within that subarray, the leftmost element must be the unique maximum and no other element may equal or exceed it. If no such subarray of length at least 2 exists, return 0; otherwise, return the largest possible length. For example, for heights `[3, 1, 2]`, the valid subarrays are `[3]` (length 1, ignored) and `[3,1]` (length 2) but `[3,1,2]` is invalid because 2 is not less than the first element 3? Actually 2 < 3 so it would be valid, but the subarray starting at index 0 and ending at 2 has first element 3 which is greater than both 1 and 2, so it is valid and length 3. So answer is 3. For `[2, 3, 1]`, valid subarrays are `[2]`, `[3]`, `[3,1]` (length 2), and `[2,3]` is invalid because first element 2 is not greater than 3. Answer is 2.

#include <cassert>
#include <vector>

// Function declaration (from solution)
int longestValidWindow(const std::vector<int>& heights);

int main() {
    // Basic cases
    assert(longestValidWindow({3, 1, 2}) == 3);          // [3,1,2] valid
    assert(longestValidWindow({2, 3, 1}) == 2);          // [3,1] is longest
    assert(longestValidWindow({1, 2, 3}) == 1);          // only single-elements valid -> return 0
    assert(longestValidWindow({5, 4, 3, 2, 1}) == 5);    // entire array valid
    assert(longestValidWindow({1, 1, 1}) == 0);          // duplicates invalid for length>=2

    // Duplicates breaking longer windows
    assert(longestValidWindow({4, 5, 4, 3}) == 2);       // [5,4] valid, but [4,5,4] invalid
    assert(longestValidWindow({7, 6, 5, 6}) == 3);       // [7,6,5] valid, not [7,6,5,6]
    assert(longestValidWindow({10, 9, 8, 7}) == 4);      // strict decreasing full array
    assert(longestValidWindow({1}) == 0);                // single element
    assert(longestValidWindow({2, 1, 2}) == 2);          // [2,1] or [2] from second? Actually [2,1] valid length 2, not [2,1,2]

    // All zeros
    assert(longestValidWindow({0, 0, 0}) == 0);

    // Mixed
    assert(longestValidWindow({3, 3, 3, 2}) == 2);       // [3,2] from last 3? The last 3 is greater than 2, so length 2
    return 0;
}

#include <vector>
#include <algorithm>

// Returns the maximum length of a contiguous subarray where the leftmost element
// is strictly greater than all other elements in that subarray.
// If no such subarray of length at least 2 exists, returns 0.
int longestValidWindow(const std::vector<int>& heights) {
    const int n = static_cast<int>(heights.size());
    if (n == 0) return 0;

    std::vector<int> nextGE(n, n); // next index with height >= heights[i]
    std::vector<int> stack;
    stack.reserve(n);

    // Monotonic decreasing stack (strictly decreasing heights) to find next greater-or-equal
    for (int i = n - 1; i >= 0; --i) {
        while (!stack.empty() && heights[stack.back()] < heights[i]) {
            stack.pop_back();
        }
        if (!stack.empty()) {
            nextGE[i] = stack.back();
        }
        stack.push_back(i);
    }

    int best = 0;
    for (int i = 0; i < n; ++i) {
        best = std::max(best, nextGE[i] - i);
    }
    return (best >= 2) ? best : 0;
}

// The problem is to find for every possible ending index `i`, the longest subarray ending at `i` whose first element is strictly greater than all other elements. For a subarray `[L, R]`, the condition is that `heights[L] > max(heights[L+1...R])`. Equivalently, for each index `i`, we need the smallest `L` such that for all `k` in `(L, i]`, `heights[L] > heights[k]`. This is hard to do naively in O(n^2). The provided snippet uses two monotonic deques to track maximum and minimum candidates, but a simpler and clearer solution: For each index `i`, we can find the nearest previous index where a value is greater than or equal to `heights[i]`? Actually we need a different viewpoint: The condition for a subarray `[L, R]` is that the maximum of the entire subarray is at position `L` and unique. So for each `R`, we want the smallest `L` such that `heights[L]` is the strict maximum in `[L, R]`. We can precompute for each index `i` the previous greater-or-equal element (PGE) and next greater-or-equal element (NGE). Then a subarray `[L, R]` is valid if `L` is the unique maximum in `[L, R]`, which means that the previous greater-or-equal element of `L` must be less than `L` (obviously) and the next greater-or-equal element of `L` must be greater than `R`. So for each `L`, the longest valid subarray starting at `L` extends up to `R = NGE[L] - 1` (or `n-1` if none), and we need to ensure that no element before `L` affects it, but we are starting at `L` anyway. So for each `L`, the maximum length ending at any `R` such that `R < NGE[L]` is `NGE[L] - L`. We can compute `NGE` for each index using a monotonic decreasing stack (strictly greater? Actually for strict maximum unique, we need the next index with value >= current, because if equal occurs, the subarray is invalid). So compute `nextGreaterOrEqual` for each index. Then answer is max over all `L` of `(nextGreaterOrEqual[L] - L - 1)`? Wait: If `NGE[L]` is the first index `> L` with `heights[NGE[L]] >= heights[L]`, then any subarray starting at `L` and ending before `NGE[L]` has `heights[L]` as the unique maximum. The maximum possible `R` is `NGE[L] - 1`, so the length is `(NGE[L] - 1) - L + 1 = NGE[L] - L`. If there is no such index, then `NGE[L] = n`, so length is `n - L`. But we also need to consider that the subarray could start later? Actually the condition is about the first element, so each valid subarray has a unique leftmost maximum. So for a given `L`, the longest valid subarray starting at `L` is from `L` to `NGE[L]-1`. So answer is max over `L` of `NGE[L] - L`. But we must ensure length at least 1? The task says return 0 if no subarray of length at least 2, but we can just compute max over all `L` of `NGE[L] - L` and if max is 1 then return 0 because length 1 is not allowed? Actually if the array is strictly decreasing, then for every `L`, `NGE[L]=n` so length `n-L` which is at least 1. For `L=n-1` length 1, but we have longer. So we just take max over all `L` of `NGE[L]-L`, and if that max is 1, return 0. Otherwise return max. Edge case: empty array? Not specified, assume non-empty. Duplicates: Equal values break the condition because first element must be strictly greater than every other, so if equal appears, the subarray is invalid. That's why we use `>=` for NGE. Use monotonic decreasing stack to compute `nextGreaterOrEqual` for each position: iterate from right to left, maintain a stack of indices with strictly decreasing heights? Actually we need the next index to the right where height is >= current. So we pop while stack top has height < current (strictly less), then the top (if any) is the next greater-or-equal. Then push current. Complexity O(n) time, O(n) space. Answer is max of `nextGE[i] - i`. Then if answer <=1, return 0, else return answer. Test with examples.
