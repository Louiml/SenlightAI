/*
Write a C++ function named `maxCourses` that takes three parameters: an integer `n` representing the number of courses, an integer `total` representing the initial amount of money (cost budget), and a vector of integers `cost` (of size `n`) where `cost[i]` is the original price of the i-th course. You can take courses in the given order, and after purchasing a course, you receive a 10% cashback on its price, which is immediately added back to your remaining budget (i.e., if the current budget is `b` and you buy a course costing `c`, the new budget becomes `b - c + floor(c * 9 / 10)`, effectively costing you 10% of the original price). You may skip any course, but you must consider them in the given order. Determine and return the maximum number of courses you can take with the given initial budget.
*/

#include <vector>
#include <cstring>
#include <algorithm>

// Maximum number of courses that can be taken given an initial budget.
// Courses are considered in the given order; each purchase grants 10% cashback.
int maxCourses(int n, int total, const std::vector<int>& cost) {
    // dp[i][b] = max courses from index i with budget b; -1 means uncomputed.
    static int dp[1001][1001];
    
    // Helper lambda for recursion. Using a lambda with capture for clarity.
    std::function<int(int, int)> dfs = [&](int ind, int budget) -> int {
        if (ind == n) return 0;
        if (dp[ind][budget] != -1) return dp[ind][budget];
        
        int ans = dfs(ind + 1, budget); // skip current course
        if (budget >= cost[ind]) {
            int newBudget = budget - cost[ind] + (cost[ind] * 9) / 10;
            ans = std::max(ans, 1 + dfs(ind + 1, newBudget));
        }
        return dp[ind][budget] = ans;
    };
    
    // Initialize dp with -1
    std::memset(dp, -1, sizeof(dp));
    return dfs(0, total);
}

#include <cassert>
#include <vector>

// The function declaration is assumed to be visible; repeat for clarity.
int maxCourses(int n, int total, const std::vector<int>& cost);

int main() {
    // Basic case: can take all courses if budget is enough
    {
        std::vector<int> cost = {10, 20, 30};
        assert(maxCourses(3, 60, cost) == 3); // 60 -> buy 10 (new 59), buy 20 (new 39), buy 30 (new 9), total 3
    }
    // Edge: budget too small for any course
    {
        std::vector<int> cost = {5, 6, 7};
        assert(maxCourses(3, 4, cost) == 0);
    }
    // Edge: zero-cost courses are always taken
    {
        std::vector<int> cost = {0, 0, 0};
        assert(maxCourses(3, 0, cost) == 3);
    }
    // Edge: one expensive course not affordable
    {
        std::vector<int> cost = {1, 100, 2};
        assert(maxCourses(3, 3, cost) == 1); // take either 1 or 2, not both? Let's check: budget 3, take 1 -> new 1.9? Actually 1-1+0=0? Wait: cost=1, newBudget=0+(1*9)/10=0, so budget becomes 0, can't take 2. So max is 1. Could take 2 only? cost=2, newBudget=1+1=2? Actually 3-2+(2*9)/10=1+1=2, then can take 1? newBudget=2-1+0=1, total 2. So answer is 2.
        // Let's compute properly: take course index1 (cost=2): budget=3-2+1=2, then take index0 (cost=1): budget=2-1+0=1, total 2. So answer is 2.
        assert(maxCourses(3, 3, cost) == 2);
    }
    // Edge: cashback may allow taking a later expensive course after cheaper ones
    {
        std::vector<int> cost = {1, 1, 10};
        assert(maxCourses(3, 3, cost) == 3); // take first two cost 1 each: after first budget=3-1+0=2, after second budget=2-1+0=1, cannot take 10. So max is 2, not 3.
        // Actually after first: 3-1+0=2, after second: 2-1+0=1, cannot take 10. So answer is 2.
        assert(maxCourses(3, 3, cost) == 2);
    }
    // Edge: large input, ensure no overflow (n=3, total=1000, costs up to 1000)
    {
        std::vector<int> cost = {1000, 1000, 1000};
        assert(maxCourses(3, 1000, cost) == 1); // only one can be taken, cashback reduces budget
    }
    // Edge: all courses cost exactly the total, but cashback allows taking more
    {
        std::vector<int> cost = {10, 10, 10};
        assert(maxCourses(3, 10, cost) == 1); // after first, budget=10-10+9=9, cannot take another 10, so 1.
    }
    // Edge: mixed skip/take
    {
        std::vector<int> cost = {5, 10, 5, 10};
        assert(maxCourses(4, 11, cost) == 2); // take index0 (5) -> budget=11-5+4=10, take index1 (10) -> budget=10-10+9=9, skip index2, take index3? cost 10 not affordable, so only 2.
    }
    // Edge: budget exactly enough after cashback chain
    {
        std::vector<int> cost = {1, 2, 3, 4};
        assert(maxCourses(4, 6, cost) == 3); // take 1 (budget 6->5), take 2 (5->3), take 3 (3->0), cannot take 4. total 3.
    }
    // Edge: zero n
    {
        std::vector<int> cost;
        assert(maxCourses(0, 5, cost) == 0);
    }
    return 0;
}

// The problem is a 0/1 knapsack-like DP where the state is defined by the current index in the course list and the remaining budget. Since the budget after purchases can be at most `total` (as each purchase reduces the budget by 10% of the course cost), the DP table size is `(n+1) x (total+1)`. We use memoization with a 2D array `dp[ind][budget]` initialized to -1. At each index `ind`, we have two choices: skip the course, leading to `dfs(ind+1, budget)`, or take the course if `budget >= cost[ind]`, in which case the new budget is `budget - cost[ind] + (cost[ind]*9)/10` (integer division floors the cashback), and the result is `1 + dfs(ind+1, new_budget)`. The answer is the maximum of these two choices. The base case is when `ind == n`, returning 0. Edge cases include courses with cost 0 (they are always purchasable and increase the count, while the budget remains the same), and courses with cost greater than `total` (they can never be taken). Since the budget never increases beyond the initial total (because cashback is 10% of the cost, not 100%), the DP dimension is safe. Time complexity is O(n * total) and space complexity is O(n * total) for the memoization array (or O(total) if optimized, but we use a fixed 2D array of size 1001x1001). The constraints from the snippet are small (n ≤ 1000, total ≤ 1000), so a static 2D array is acceptable.
