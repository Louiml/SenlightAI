/*
Given the number of trips `n` and the departure times `w[1..n]` (in minutes since midnight, sorted in non-decreasing order), write a C++ function that computes, for each trip `i` from 1 to `n`, the minimum total cost to travel on the first `i` trips, assuming the following ticket options are available: a single-trip ticket costs 20; a 90-minute pass costs 50 (valid for any number of trips whose departure times are within 90 minutes of each other, starting from the first trip it covers); and a 1440-minute (daily) pass costs 120 (valid for any number of trips within 1440 minutes of each other). The passes are not issued from the moment of purchase but rather each pass covers a contiguous block of trips; when you buy a pass, it applies to the next trip and all subsequent trips until the time limit expires. The cost for each prefix of trips must be minimized, and your function should return a vector of length `n` where the `i`-th element is the *difference* between the minimum total cost for the first `i` trips and the minimum total cost for the first `i-1` trips (with the first element being the minimum cost for just the first trip). The input times are guaranteed sorted and non-decreasing, and `n` is at least 1. Use 1-based indexing conceptually for the trips, but implement with 0-based vectors internally.
*/

#include <vector>
#include <algorithm>

// Returns the incremental minimum cost for each prefix, given sorted departure times.
std::vector<int> incrementalTravelCost(const std::vector<int>& w) {
    int n = (int)w.size();
    std::vector<int> dp(n);
    int p = 0;   // first trip that might be covered by a 90-minute pass ending at current i
    int p2 = 0;  // first trip that might be covered by a 1440-minute pass ending at current i

    for (int i = 0; i < n; ++i) {
        if (i == 0) {
            dp[i] = 20;
        } else {
            dp[i] = dp[i-1] + 20;
        }

        // Advance pointers while the pass starting at p/p2 cannot cover trip i.
        while (w[p] + 90 <= w[i]) ++p;
        while (w[p2] + 1440 <= w[i]) ++p2;

        // 90-minute pass: covers trips p..i
        int candidate90 = (p == 0 ? 0 : dp[p-1]) + 50;
        dp[i] = std::min(dp[i], candidate90);

        // 1440-minute pass: covers trips p2..i
        int candidate1440 = (p2 == 0 ? 0 : dp[p2-1]) + 120;
        dp[i] = std::min(dp[i], candidate1440);
    }

    std::vector<int> result(n);
    for (int i = 0; i < n; ++i) {
        int prev = (i == 0) ? 0 : dp[i-1];
        result[i] = dp[i] - prev;
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above (included before tests).
int main() {
    // Single trip
    std::vector<int> t1 = {100};
    std::vector<int> r1 = incrementalTravelCost(t1);
    assert(r1.size() == 1 && r1[0] == 20);

    // Two trips 30 minutes apart: buy a 90-minute pass
    std::vector<int> t2 = {0, 30};
    std::vector<int> r2 = incrementalTravelCost(t2);
    assert(r2.size() == 2 && r2[0] == 20 && r2[1] == 30); // 20 then +30 total 50

    // Three trips within 90 minutes: one pass cheaper than singles
    std::vector<int> t3 = {0, 45, 89};
    std::vector<int> r3 = incrementalTravelCost(t3);
    // dp: 20, 50, 50 -> increments: 20,30,0
    assert(r3.size() == 3 && r3[0] == 20 && r3[1] == 30 && r3[2] == 0);

    // Four trips spread over a day: likely singles then daily pass
    std::vector<int> t4 = {0, 100, 200, 1500};
    std::vector<int> r4 = incrementalTravelCost(t4);
    // dp: 20, 50, 70, 120 (pass for last two? Actually last trip at 1500 with p2=0? 1440 covers 0..1500? no 1500-0=1500 >1440, so p2 advances to 1; p2=1, dp[0]+120=140 vs dp[2]+20=90 -> choose 90, so dp3=90? Let's compute correctly: for i=1 (100): dp=50. i=2 (200): singles 70, p=0 (since 0+90<=200 false? 90<=200 yes so p=1, p+90=190<=200 so p=2, p=2, dp[1]+50=100, so dp=70. i=3 (1500): singles 90, p advances until w[p]+90>1500? p from 2, w[2]+90=290<=1500, p=3, dp[2]+50=120, candidate 120; p2: w[0]+1440=1440<=1500, p2=1, dp[0]+120=140, candidate 140; min=90. So increments: 20,30,20,20 => sum 90. assert r4[0]==20, r4[1]==30, r4[2]==20, r4[3]==20.
    assert(r4.size() == 4);
    assert(r4[0] == 20 && r4[1] == 30 && r4[2] == 20 && r4[3] == 20);

    // Trips exactly at boundaries
    std::vector<int> t5 = {0, 90, 1440}; // 0 and 90: 90-minute pass; then 1440 alone: single
    std::vector<int> r5 = incrementalTravelCost(t5);
    // dp: 20, 50 (pass covers both), 70 (add single) -> increments: 20,30,20
    assert(r5.size() == 3 && r5[0] == 20 && r5[1] == 30 && r5[2] == 20);

    // Many trips within 24h: daily pass best
    std::vector<int> t6 = {0, 10, 20, 30, 40, 50, 60};
    std::vector<int> r6 = incrementalTravelCost(t6);
    // 7 singles = 140, 90-min pass for all (since within 60) = 50, so dp all 50 after first? dp0=20, dp1=50, dp2=50, ... increments: 20,30,0,0,0,0,0
    assert(r6.size() == 7);
    assert(r6[0] == 20 && r6[1] == 30);
    for (int i = 2; i < 7; ++i) assert(r6[i] == 0);

    // Unsorted not allowed but we trust input sorted; test with duplicate times
    std::vector<int> t7 = {100, 100, 100};
    std::vector<int> r7 = incrementalTravelCost(t7);
    // All within 90 min, so pass of 50 for all: dp0=20, dp1=50, dp2=50 -> increments: 20,30,0
    assert(r7.size() == 3 && r7[0] == 20 && r7[1] == 30 && r7[2] == 0);

    return 0;
}

// We need to compute the minimum cost to cover trips up to each index. This is a classic dynamic programming problem. Let `dp[i]` be the minimum cost to cover trips `0..i` (0-based). For each trip `i`, the optimal strategy for the last ticket is one of three options: (1) pay a single ticket for trip `i`, so cost is `dp[i-1] + 20`; (2) buy a 90-minute pass that starts at some trip `j <= i` such that `w[i] - w[j] <= 90` and the pass covers all trips from `j` to `i`; the cost is `dp[j-1] + 50` (with `dp[-1] = 0`). To minimize, we take the smallest such `j` for which the condition holds, because a pass starting earlier covers more trips for the same price. So we maintain a pointer `p` that points to the first trip where `w[p] + 90 > w[i]` (i.e., the earliest trip that the pass must include to cover trip `i`). Then the candidate is `dp[p-1] + 50`. Similarly, for a 1440-minute pass, maintain pointer `p2` such that `w[p2] + 1440 > w[i]`, candidate `dp[p2-1] + 120`. Take the minimum of the three. The output for each `i` is `dp[i] - dp[i-1]` (with `dp[-1] = 0`). Complexity is O(n) time and O(n) space for `dp` and the returned vector. Edge cases: when `i=0`, `dp[-1]` is 0; pointers start at 0 and are incremented only while the condition fails. The times are non-decreasing, so two pointers suffice.
