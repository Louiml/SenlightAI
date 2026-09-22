// You are given two `vector<vector<int>>` inputs: `flights` and `days`. `flights` is an `n x n` adjacency matrix where `flights[i][j] == 1` means there is a direct flight from city `i` to city `j` on any day (you can also stay in the same city, i.e., you are allowed to "fly" from city `i` to itself, but the matrix may not have `1` on the diagonal; staying is always allowed). `days` is an `n x K` matrix where `days[city][week]` gives the number of vacation days you can spend in that city during that week (0-indexed weeks). You start in city `0` at the beginning of week 0, **before** any vacation days are counted. Each week, you may either stay in your current city or take a flight to another city (subject to `flights` matrix) and then spend that week's vacation days in your new city. You have `K` weeks total. Write a C++ function that returns the **maximum total vacation days** you can accumulate over `K` weeks. You are allowed to end in any city. Constraints: `1 <= n <= 10`, `1 <= K <= 100`, `0 <= days[i][j] <= 1000`, and `flights` contains only `0` or `1`. The function signature: `int maxVacationDays(const vector<vector<int>>& flights, const vector<vector<int>>& days)`.
This is a classic dynamic programming problem over weeks and cities. Let `dp[week][city]` represent the maximum total vacation days achievable at the end of week `week` (1-indexed weeks, where week `1` is the first vacation week) while being in `city`. Base case: `dp[0][0] = 0` for week 0 (before any vacation), and `dp[0][city] = -infinity` for all other cities (you cannot be there initially). For each week from 1 to K, for each destination city `j`, you can come from any previous city `i` that either equals `j` (stay) or has `flights[i][j] == 1`. The recurrence is `dp[week][j] = max(dp[week-1][i] + days[j][week-1])` over all such `i`. Since you can always stay, you must include `i == j` even if `flights[j][j] == 0` (self-loop is implicitly allowed). Implementation uses a 2D array of size `(K+1) x n`, initializing with a very negative value for impossible states. The answer is the maximum value in the last row `dp[K][*]`. Edge cases: when `n == 1`, you can only stay, so the answer is the sum of all days in city 0. Also, handle when the initial city cannot be reached? But you start in city 0, so it is always reachable. Time complexity is `O(K * n^2)` due to iterating over all previous cities for each destination each week, and space complexity is `O(K * n)` (can be optimized to O(n) storing only previous week). Constraint `n <= 10` makes this trivial.
#include <vector>
#include <algorithm>
#include <cstring>

// Returns the maximum vacation days over K weeks, starting in city 0.
int maxVacationDays(const std::vector<std::vector<int>>& flights,
                    const std::vector<std::vector<int>>& days) {
    int n = flights.size();
    int K = days[0].size();

    // dp[week][city]: max days after 'week' weeks, being in 'city'.
    // week ranges from 0 to K. week 0 means before any vacation.
    int dp[K + 1][n];
    const int NEG_INF = -0x3f3f3f3f;

    // Initialize all states to impossible.
    for (int w = 0; w <= K; ++w)
        for (int c = 0; c < n; ++c)
            dp[w][c] = NEG_INF;

    // Base case: at week 0, we are in city 0 with 0 vacation days.
    dp[0][0] = 0;

    // Process each week.
    for (int w = 1; w <= K; ++w) {
        for (int j = 0; j < n; ++j) {
            // Option 1: stay in city j (always allowed, even if flights[j][j] is 0).
            int best = dp[w - 1][j];

            // Option 2: fly from some city i != j, if flights[i][j] == 1.
            for (int i = 0; i < n; ++i) {
                if (i != j && flights[i][j] == 1) {
                    best = std::max(best, dp[w - 1][i]);
                }
            }

            // Add vacation days for current week in city j.
            if (best != NEG_INF) {
                dp[w][j] = best + days[j][w - 1];
            }
        }
    }

    // Answer: maximum over all possible ending cities after K weeks.
    int ans = 0;
    for (int c = 0; c < n; ++c) {
        ans = std::max(ans, dp[K][c]);
    }
    return ans;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Single city, 3 weeks.
    {
        std::vector<std::vector<int>> flights = {{0}};
        std::vector<std::vector<int>> days = {{1, 2, 3}};
        assert(maxVacationDays(flights, days) == 6); // 1+2+3
    }

    // Test 2: Two cities, no flights (must stay in 0 always).
    {
        std::vector<std::vector<int>> flights = {{0, 0}, {0, 0}};
        std::vector<std::vector<int>> days = {{5, 7}, {10, 20}};
        assert(maxVacationDays(flights, days) == 12); // stay in 0: 5+7
    }

    // Test 3: Two cities, one flight from 0 to 1, only 1 week.
    {
        std::vector<std::vector<int>> flights = {{0, 1}, {0, 0}};
        std::vector<std::vector<int>> days = {{1, 100}};
        // Either stay in 0 (1 day) or fly to 1 (100 days).
        assert(maxVacationDays(flights, days) == 100);
    }

    // Test 4: Example from typical problem (LeetCode 568).
    {
        std::vector<std::vector<int>> flights = {{0, 1, 1}, {1, 0, 1}, {1, 1, 0}};
        std::vector<std::vector<int>> days = {{1, 3, 1}, {6, 0, 3}, {3, 3, 3}};
        // Best: week1: 0, week2: 1, week3: 2 => 1+3+3=7? Actually track:
        // week1: 0->0 (1) or 0->1 (6) or 0->2 (3)
        // week2: from 1 can go to 0(3),1(0),2(3) -> best 3+3=6? Let's compute manually:
        // Path: 0 (week1:1) -> 1 (week2:0) -> 2 (week3:3) total 4
        // Path: 0 (week1:1) -> 2 (week2:3) -> 1 (week3:3) total 7
        // Path: 0 (week1:6) -> 0 (week2:3) -> 0 (week3:1) total 10? Actually 6+3+1=10
        // Path: 0(6) -> 1(0) -> 0(1) = 7
        // Path: 0(6) -> 2(3) -> 0(1) = 10
        // So answer should be 10.
        assert(maxVacationDays(flights, days) == 10);
    }

    // Test 5: K weeks, but flights allow returning to 0.
    {
        std::vector<std::vector<int>> flights = {{0, 1}, {1, 0}};
        std::vector<std::vector<int>> days = {{5, 5}, {10, 10}};
        // week1: stay 0=5, fly to 1=10
        // week2: from 0: stay=5, fly=10; from 1: stay=10, fly=5
        // Best: 10+10=20 (stay in 1 both weeks) -> also 5+5=10, 10+5=15, 5+10=15.
        assert(maxVacationDays(flights, days) == 20);
    }

    // Test 6: Large K with trivial flights.
    {
        std::vector<std::vector<int>> flights = {{0}};
        std::vector<std::vector<int>> days(1, std::vector<int>(10, 7));
        assert(maxVacationDays(flights, days) == 70);
    }

    return 0;
}
