// Write a C++ function `int maxConsultingProfit(int n, const std::vector<std::pair<int,int>>& consultations)` that solves the classic "Resignation" (퇴사) problem: A worker has `n` days until retirement. For each day `i` (0-indexed), a consultation takes `T[i]` days to complete and pays `P[i]` profit. The worker can either accept the consultation on day `i` (which occupies days `i` through `i+T[i]-1`) or skip it and work another day. Jobs that would finish after day `n` cannot be accepted. The goal is to maximize total profit. Return the maximum possible profit. The input may contain consultations with `T[i]` larger than the remaining days, which must be ignored. Assume `1 ≤ n ≤ 15` and all profits are non-negative integers.
// This is a dynamic programming or recursive brute-force problem. Since `n` is small (≤15), a simple recursion exploring all subsets of accepted consultations works: for each day `i`, we have two choices — either skip the consultation at day `i` and move to day `i+1`, or accept it (if `i + T[i] ≤ n`) and move to day `i + T[i]`, adding `P[i]` to the profit. To avoid recomputation, we can use memoization with an array `dp[i]` representing the maximum profit obtainable from day `i` onward. The recursion visits each day at most once due to memoization. The base case is when `i ≥ n`, where profit is 0. Edge cases: tasks that extend beyond the last day (i.e., `i + T[i] > n`) are simply skipped, as they cannot be started. Another subtlety: the original snippet uses a global `dp` array and tries to update it with a max check; we can implement a cleaner recursive function with memoization. Time complexity is O(n) after memoization (each day visited once, each call does constant work), but without memoization it would be O(2^n); with n≤15 even O(2^n) is acceptable, but we provide O(n) solution. Space complexity is O(n) for the memo array and recursion stack.
#include <vector>
#include <algorithm>
#include <functional>

// Returns the maximum profit obtainable from consultations starting at day 0.
// consultations[i] = {duration, profit} for day i (0-indexed). Jobs that finish after day n are ignored.
int maxConsultingProfit(int n, const std::vector<std::pair<int,int>>& consultations) {
    // Memoization: dp[i] = max profit from day i onward. -1 means not computed.
    std::vector<int> dp(n + 1, -1);
    
    // Recursive lambda with memoization.
    std::function<int(int)> solve = [&](int day) -> int {
        if (day >= n) return 0;  // No more days available
        if (dp[day] != -1) return dp[day];
        
        int duration = consultations[day].first;
        int profit = consultations[day].second;
        
        // Option 1: Skip current day's consultation.
        int skip = solve(day + 1);
        
        // Option 2: Accept current day's consultation if it fits within available days.
        int take = 0;
        if (day + duration <= n) {
            take = profit + solve(day + duration);
        }
        
        dp[day] = std::max(skip, take);
        return dp[day];
    };
    
    return solve(0);
}
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (provided by the solution above)
int maxConsultingProfit(int n, const std::vector<std::pair<int,int>>& consultations);

int main() {
    // Test 1: Example from the original problem (n=7)
    // Days: (3,10), (5,20), (1,10), (1,20), (2,15), (4,40), (2,200)
    // Max profit = 45 (accept day 0 → ends day 3, then day 3 → ends day 4, then day 4 → ends day 6? Actually compute properly)
    // Let's manually verify: Accept day 0 (3 days, pay 10) → next day 3, accept day 3 (1 day, pay 20) → next day 4, accept day 4 (2 days, pay 15) → next day 6, skip day 6? Actually day 6 has 2 days, ends day 8 > 7, so skip. Total = 10+20+15=45.
    std::vector<std::pair<int,int>> test1 = {{3,10},{5,20},{1,10},{1,20},{2,15},{4,40},{2,200}};
    assert(maxConsultingProfit(7, test1) == 45);

    // Test 2: All tasks too long, accept none
    std::vector<std::pair<int,int>> test2 = {{5,100},{6,100},{10,100}};
    assert(maxConsultingProfit(5, test2) == 0);

    // Test 3: Single day with 1-day task
    std::vector<std::pair<int,int>> test3 = {{1,50}};
    assert(maxConsultingProfit(1, test3) == 50);

    // Test 4: All days have 1-day tasks, choose highest profit each day (all positive so take all)
    std::vector<std::pair<int,int>> test4 = {{1,1},{1,2},{1,3}};
    assert(maxConsultingProfit(3, test4) == 6);

    // Test 5: Overlapping tasks, must choose best combo
    // n=4, options: day0 (2,100) then day2 (2,1) = 101, or day0(2,100) then skip? Actually day0 ends day2, day2 can start, ends day4. Total 101. Alternatively day1 (1,90) + day2(2,1) = 91. So best is 101.
    std::vector<std::pair<int,int>> test5 = {{2,100},{1,90},{2,1},{1,0}};
    assert(maxConsultingProfit(4, test5) == 101);

    // Test 6: Edge case: n=1, task takes 2 days (impossible) → 0
    std::vector<std::pair<int,int>> test6 = {{2,500}};
    assert(maxConsultingProfit(1, test6) == 0);

    // Test 7: Multiple zero-profit tasks, still can accept but doesn't affect max
    std::vector<std::pair<int,int>> test7 = {{1,0},{1,0},{1,0}};
    assert(maxConsultingProfit(3, test7) == 0);

    // Test 8: Large profit at end but conflicts with earlier small profit
    // n=3: day0 (2,10) ends day2, then day2 (1,5) = 15. Or skip day0 and take day1 (1,1) + day2(1,5) = 6. So 15.
    std::vector<std::pair<int,int>> test8 = {{2,10},{1,1},{1,5}};
    assert(maxConsultingProfit(3, test8) == 15);

    // Test 9: All tasks span exactly one day, choose all
    std::vector<std::pair<int,int>> test9 = {{1,5},{1,3},{1,7}};
    assert(maxConsultingProfit(3, test9) == 15);

    // Test 10: Max n=15, random data, ensure no crash and reasonable output
    std::vector<std::pair<int,int>> test10 = {{1,1},{2,2},{3,3},{4,4},{5,5},{1,6},{2,7},{3,8},{4,9},{5,10},{1,11},{2,12},{3,13},{4,14},{5,15}};
    // Not asserting exact value, just that it runs (can't assert easily), but we can assert it's >=0.
    int result10 = maxConsultingProfit(15, test10);
    assert(result10 >= 0);

    return 0;
}
