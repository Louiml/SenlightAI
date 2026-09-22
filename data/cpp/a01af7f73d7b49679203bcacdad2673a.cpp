// Given a vector of integers `nums` and an integer `target`, write a C++ function `int findTargetSumWays(const std::vector<int>& nums, int target)` that returns the number of ways to assign a `+` or `-` sign to each integer so that the total sum equals `target`. You must solve this using recursion with memoization (top-down dynamic programming). The order of assignments matters: each element must be used exactly once, and you may not reorder the elements. Assume the vector can be empty (in which case the only possible sum is 0), and the target can be any integer (positive, negative, or zero). The result may be large, but it fits within a 32-bit signed integer for the given constraints.
// The problem is a classic subset-sum variation where each element can be either added or subtracted. The naive recursive solution explores both choices for each element, leading to `O(2^n)` time, which is too slow for large inputs. Memoization dramatically improves this by caching results for each `(index, current_sum)` pair. Since sums can be negative, a simple 2D array is not convenient; instead, use a `std::map` keyed by a pair of integers, or a vector of `unordered_map<int,int>` where the index is the current position and the key is the current sum. 
//
// Algorithm:
// - Define a recursive helper `int solve(const vector<int>& nums, int idx, int current_sum, int target, map<pair<int,int>, int>& memo)`.
// - Base case: if `idx == nums.size()`, return `(current_sum == target) ? 1 : 0`.
// - If `(idx, current_sum)` is already in memo, return the cached value.
// - Otherwise, compute `add = solve(nums, idx+1, current_sum + nums[idx], target, memo)` and `sub = solve(nums, idx+1, current_sum - nums[idx], target, memo)`. Store the sum in memo and return it.
// - The main function calls the helper starting with `idx=0`, `current_sum=0`.
//
// Edge cases:
// - Empty input: returns 1 if target is 0 (since the only way is to do nothing), else 0.
// - Target values that are unreachable: the recursion explores all possibilities and will return 0 when no combination matches.
// - Negative numbers in `nums` are allowed; they just add or subtract as usual.
// - The memo key must include both index and running sum because the same sum at different indices leads to different future possibilities.
//
// Time complexity: Each state `(idx, sum)` is computed once. There are `n+1` possible indices and at most `2*sum_abs + 1` distinct sums, where `sum_abs = sum(|nums[i]|)`. So worst-case time is `O(n * sum_abs)`. Space complexity is the same for the memo table.
#include <vector>
#include <map>
#include <utility>

// Returns the number of ways to assign '+' or '-' to each element
// so that the sum equals target.
int findTargetSumWays(const std::vector<int>& nums, int target) {
    // Memoization table: key = (index, current_sum), value = number of ways.
    std::map<std::pair<int, int>, int> memo;
    
    // Recursive helper function (lambda style to capture nums and target).
    // We use a function-object or a separate function for clarity.
    // Here we implement a recursive lambda using std::function for simplicity.
    // But to avoid overhead, we'll use a helper function with references.
    // Since the problem is self-contained, we can write a lambda and 
    // call it recursively using std::function.
    std::function<int(int, int)> dfs = [&](int idx, int current_sum) -> int {
        if (idx == static_cast<int>(nums.size())) {
            return (current_sum == target) ? 1 : 0;
        }
        auto key = std::make_pair(idx, current_sum);
        auto it = memo.find(key);
        if (it != memo.end()) {
            return it->second;
        }
        int add = dfs(idx + 1, current_sum + nums[idx]);
        int sub = dfs(idx + 1, current_sum - nums[idx]);
        memo[key] = add + sub;
        return memo[key];
    };
    
    return dfs(0, 0);
}
Note: The code above uses `std::function` which requires `#include <functional>`. For a pure standalone solution, include that header. Below is the full standalone version with the necessary includes. The above snippet is incomplete regarding the include, so the final solution code is provided below.

#include <vector>
#include <map>
#include <utility>
#include <functional>

// Returns the number of ways to assign '+' or '-' to each element
// so that the sum equals target.
int findTargetSumWays(const std::vector<int>& nums, int target) {
    std::map<std::pair<int, int>, int> memo;
    std::function<int(int, int)> dfs = [&](int idx, int current_sum) -> int {
        if (idx == static_cast<int>(nums.size())) {
            return (current_sum == target) ? 1 : 0;
        }
        auto key = std::make_pair(idx, current_sum);
        auto it = memo.find(key);
        if (it != memo.end()) {
            return it->second;
        }
        int add = dfs(idx + 1, current_sum + nums[idx]);
        int sub = dfs(idx + 1, current_sum - nums[idx]);
        memo[key] = add + sub;
        return memo[key];
    };
    return dfs(0, 0);
}
#include <cassert>
#include <vector>

// The solution function is declared here (from above).
int findTargetSumWays(const std::vector<int>& nums, int target);

int main() {
    // Example 1: [1,1,1,1,1] target=3 -> 5 ways
    assert(findTargetSumWays({1,1,1,1,1}, 3) == 5);
    // Example 2: [1] target=1 -> 1 way (+1)
    assert(findTargetSumWays({1}, 1) == 1);
    // Example 3: [1] target=0 -> 0 ways
    assert(findTargetSumWays({1}, 0) == 0);
    // Example 4: Empty vector target=0 -> 1 way (no signs)
    assert(findTargetSumWays({}, 0) == 1);
    // Example 5: Empty vector target=1 -> 0 ways
    assert(findTargetSumWays({}, 1) == 0);
    // Example 6: [1,2,3] target=0 -> 2 ways (+1+2-3 and -1-2+3)
    assert(findTargetSumWays({1,2,3}, 0) == 2);
    // Example 7: [0,0,0] target=0 -> 8 ways (each element can be +0 or -0)
    assert(findTargetSumWays({0,0,0}, 0) == 8);
    // Example 8: [2,2] target=4 -> 1 way (++), target=0 -> 2 ways (+- and -+)
    assert(findTargetSumWays({2,2}, 4) == 1);
    assert(findTargetSumWays({2,2}, 0) == 2);
    // Example 9: [1,2,3,4] target=6 -> 2 ways (1-2-3+4? No, check: +1+2+3-4=2, +1+2-3+4=4, ... Let's trust code)
    // Instead, simple check: [1,2,3] target=2 -> 2 ways (+1-2+3=2, -1+2+3=4? Actually +1+2-3=0, -1+2-3=-2, +1-2-3=-4, -1-2+3=0. Let's just use a known case)
    // Better: [1,1] target=2 -> 1 way (++), target=0 -> 2 ways
    assert(findTargetSumWays({1,1}, 2) == 1);
    assert(findTargetSumWays({1,1}, 0) == 2);
    return 0;
}
Note: The above test code uses `std::vector` initializer lists which require C++11 or later. The solution function is declared separately, but in a real test you would include the solution header or copy the function definition above the test. For completeness, the test code is meant to be placed after the solution definition.
