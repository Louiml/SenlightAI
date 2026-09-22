Write a C++ function that takes a vector of non-negative integers and a target sum `k`, and returns a boolean indicating whether any non-empty subset of the vector (not necessarily contiguous) sums exactly to `k`. The function should be robust to empty vectors (return `false` unless `k==0`, in which case return `true` because the empty subset sums to 0), and should handle zeros and duplicates correctly. The subset may include any elements, each used at most once.

#include <cassert>
#include <vector>

// The solution function is declared above (included in the same translation unit).
int main() {
    // Basic cases
    assert(hasSubsetSum({7, 2, 3}, 4) == false);  // no subset sums to 4
    assert(hasSubsetSum({7, 2, 3}, 5) == true);   // 2+3=5
    assert(hasSubsetSum({7, 2, 3}, 7) == true);   // 7
    assert(hasSubsetSum({7, 2, 3}, 12) == true);  // 7+2+3=12

    // Edge cases
    assert(hasSubsetSum({}, 0) == true);          // empty subset for k=0
    assert(hasSubsetSum({}, 5) == false);         // empty vector, nonzero target
    assert(hasSubsetSum({1, 2, 3}, 0) == true);   // empty subset
    assert(hasSubsetSum({1, 2, 3}, -1) == false); // negative target

    // Zeros and duplicates
    assert(hasSubsetSum({0, 5}, 0) == true);      // empty subset or zero element
    assert(hasSubsetSum({0, 5}, 5) == true);      // just 5
    assert(hasSubsetSum({2, 2, 2}, 4) == true);   // two 2s
    assert(hasSubsetSum({2, 2, 2}, 6) == true);   // three 2s
    assert(hasSubsetSum({2, 2, 2}, 5) == false);  // cannot make odd target

    // Large target that cannot be formed
    assert(hasSubsetSum({1, 2, 4}, 8) == false);  // 1+2+4=7 < 8
    assert(hasSubsetSum({1, 2, 4}, 7) == true);   // 1+2+4=7

    // Single element
    assert(hasSubsetSum({3}, 3) == true);
    assert(hasSubsetSum({3}, 0) == true);
    assert(hasSubsetSum({3}, 1) == false);
}

#include <vector>
#include <cstdint>

// Returns true if a non-empty subset of nums (each element used at most once) sums exactly to k.
// For k==0, returns true because the empty subset sums to 0.
bool hasSubsetSum(const std::vector<int>& nums, int k) {
    if (k < 0) return false;
    if (nums.empty()) return (k == 0);
    if (k == 0) return true;

    const int n = static_cast<int>(nums.size());
    // dp[i][target] = -1 (uncomputed), 0 (false), 1 (true)
    std::vector<std::vector<int8_t>> dp(n, std::vector<int8_t>(k + 1, -1));

    // Recursive lambda with memoization
    std::function<bool(int, int)> f = [&](int i, int target) -> bool {
        if (target == 0) return true;
        if (i == 0) return (nums[0] == target);
        if (dp[i][target] != -1) return (dp[i][target] == 1);

        bool result = false;
        // Skip current element
        result = result || f(i - 1, target);
        // Take current element if it fits
        if (target >= nums[i]) {
            result = result || f(i - 1, target - nums[i]);
        }
        dp[i][target] = result ? 1 : 0;
        return result;
    };

    // Call with last index and full target
    return f(n - 1, k);
}

// The problem is a classic subset sum decision problem. We solve it using dynamic programming with memoization to avoid exponential recursion. Define a recursive function `f(i, target)` that returns true if we can form `target` using elements from index `0` up to `i` (inclusive). The base cases are: if `target==0`, we return true (empty subset works); if `i==0`, we return whether `nums[0]` equals `target` (since we can only pick or skip the first element). For other cases, we try two options: either skip the current element (call `f(i-1, target)`), or take it if it does not exceed `target` (call `f(i-1, target - nums[i])`). The result is the logical OR of both. We use a 2D memoization table `dp` of size `n x (k+1)` initialized to `-1`, storing 0/1 results to avoid recomputation. Edge cases: if `k==0`, return true immediately (empty subset). If the vector is empty, return `(k==0)`. If `k` is negative or all elements are larger than `k`, the recursion will naturally return false. Time complexity is O(n*k) because each state `(i, target)` is computed once. Space complexity is O(n*k) for the memoization table, plus O(n) recursion stack depth.
