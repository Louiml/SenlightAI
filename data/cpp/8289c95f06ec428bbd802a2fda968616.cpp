// Write a C++ function `int oddEvenJumps(std::vector<int>& arr)` that, given a non-empty array of integers `arr` of length `n`, returns the number of valid starting indices from which a sequence of jumps can reach the last index. A jump sequence is defined as follows: starting at index `i` with jump type "odd" (the first jump is always odd), you must move to the smallest index `j > i` such that `arr[j] >= arr[i]` (for odd jumps) or `arr[j] <= arr[i]` (for even jumps). If multiple such `j` exist, choose the one with the smallest value of `arr[j]`; if ties, choose the smallest index `j`. If no such `j` exists, the jump fails. After each jump, the type alternates (odd→even, even→odd). The process continues until you either reach the last index (success) or cannot jump (failure). Count all starting indices from which you can eventually reach `arr[n-1]`. The array may contain negative numbers, duplicates, and values up to 10^9. The length `n` satisfies 1 ≤ n ≤ 20,000.
// The core idea is to precompute, for each index and each jump type (0 = even, 1 = odd), the next index to jump to. We process the array from right to left, maintaining a `map<int, int>` that stores the most recent (rightmost) index for each value seen so far. For an odd jump from index `i`, we need the smallest value greater than or equal to `arr[i]`; using `lower_bound(arr[i])` on the map gives the iterator to that value (or end). The associated value is the rightmost index with that value, which is optimal because we need the smallest index among equal values—since we process right-to-left and overwrite the map entry with current `i`, the stored index is the smallest index for that value. For an even jump, we need the largest value less than or equal to `arr[i]`; using `upper_bound(arr[i])` gives the first element strictly greater, then `prev` gives the largest ≤. If the iterator is `begin()`, no such value exists. This precomputation runs in O(n log n) time due to the map operations. After building the `g[i][k]` table (where `g[i][0]` is the next index for an even jump from `i`, and `g[i][1]` for odd), we use memoized DFS to determine reachability from each index. `f[i][k]` stores whether starting at index `i` with jump type `k` can reach the end: 0 = unknown, 1 = success, -1 = failure (though the reference stores only success as 1 and failure as 0). The base case is `i == n-1` returning 1. The recursion alternates the type `k ^ 1`. If the next index is -1, return 0. Otherwise, recursively solve and store. Finally, sum `dfs(i, 1)` for all `i` (since the first jump is odd). Edge cases include: the last index always counts (starting there trivially succeeds); duplicates handled by the rightmost-index rule; negative values are naturally handled by the map's ordering. Time complexity is O(n log n) for preprocessing and O(n) for DFS with memoization (each state visited once), total O(n log n). Space is O(n) for the DP tables and map.
#include <vector>
#include <map>
#include <functional>
#include <cstring>

// Count starting indices from which the odd/even jump sequence reaches the last index.
int oddEvenJumps(std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n == 0) return 0;

    std::map<int, int> value_to_index; // maps value -> rightmost index seen so far
    std::vector<std::vector<int>> next(n, std::vector<int>(2, -1)); // next[i][0]=even, [1]=odd

    // Build jump table from right to left
    for (int i = n - 1; i >= 0; --i) {
        // Odd jump: smallest value >= arr[i]
        auto it_ge = value_to_index.lower_bound(arr[i]);
        if (it_ge != value_to_index.end()) {
            next[i][1] = it_ge->second;
        }

        // Even jump: largest value <= arr[i]
        auto it_gt = value_to_index.upper_bound(arr[i]);
        if (it_gt != value_to_index.begin()) {
            --it_gt;
            next[i][0] = it_gt->second;
        }

        // Store current index as the rightmost for this value
        value_to_index[arr[i]] = i;
    }

    // Memoization: dp[i][k] = 1 if reachable, 0 if not, -1 if unknown
    std::vector<std::vector<int>> dp(n, std::vector<int>(2, -1));

    std::function<int(int, int)> dfs = [&](int idx, int kind) -> int {
        if (idx == n - 1) return 1;
        if (next[idx][kind] == -1) return 0;
        if (dp[idx][kind] != -1) return dp[idx][kind];
        dp[idx][kind] = dfs(next[idx][kind], kind ^ 1);
        return dp[idx][kind];
    };

    int answer = 0;
    for (int i = 0; i < n; ++i) {
        if (dfs(i, 1) == 1) ++answer;
    }
    return answer;
}
#include <cassert>
#include <vector>

// The solution function is declared above; include it here.

int main() {
    {
        std::vector<int> arr = {10, 13, 12, 14, 15};
        assert(oddEvenJumps(arr) == 2);
    }
    {
        std::vector<int> arr = {2, 3, 1, 1, 4};
        assert(oddEvenJumps(arr) == 3);
    }
    {
        std::vector<int> arr = {5, 1, 3, 4, 2};
        assert(oddEvenJumps(arr) == 3);
    }
    {
        std::vector<int> arr = {1};
        assert(oddEvenJumps(arr) == 1);
    }
    {
        std::vector<int> arr = {1, 2, 3, 4, 5};
        assert(oddEvenJumps(arr) == 5);
    }
    {
        std::vector<int> arr = {5, 4, 3, 2, 1};
        assert(oddEvenJumps(arr) == 1);
    }
    {
        std::vector<int> arr = {1, 1, 1, 1};
        assert(oddEvenJumps(arr) == 4);
    }
    {
        std::vector<int> arr = {1, 3, 2, 4, 5, 2, 6};
        assert(oddEvenJumps(arr) == 5);
    }
    {
        std::vector<int> arr = {-1, -2, -3, -4};
        assert(oddEvenJumps(arr) == 1);
    }
    {
        std::vector<int> arr = {3, 2, 1, 4, 5};
        assert(oddEvenJumps(arr) == 4);
    }
    return 0;
}
