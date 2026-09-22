/*
Write a C++ function `long long maxPogoScore(vector<pair<long long, long long>>& cows)` that takes a list of cows, each represented by a pair `(position, points)`, where positions are distinct integers and points are non-negative integers. The function must return the maximum total points that can be collected by a sequence of jumps starting from any cow and then repeatedly jumping to another cow such that the jump distances are strictly non-increasing and each subsequent jump goes in the same direction (either always to the right or always to the left). In other words, if cows are sorted by position, you can jump from cow `i` to cow `j` (where `i > j` for rightward movement) only if the distance `pos[i]-pos[j]` is less than or equal to the distance of the previous jump (for the first jump, any distance is allowed). You may choose to collect points from cows in either left-to-right or right-to-left order, and you must collect the points of every cow you land on, starting with your first cow. The input list may be unsorted and contain up to 1000 cows. Return the maximum total points achievable.
*/

#include <vector>
#include <algorithm>
#include <utility>
using namespace std;

using LL = long long;
using pi = pair<LL, LL>;

// Compute the maximum pogo points achievable given a list of cows.
LL maxPogoScore(vector<pi>& cows) {
    int n = cows.size();
    if (n == 0) return 0;
    
    sort(cows.begin(), cows.end()); // sort by position ascending
    vector<vector<LL>> dp(n, vector<LL>(n, 0));
    LL ans = 0;
    
    // Pass 1: rightward jumps (jump to higher index)
    for (int i = 0; i < n; ++i) {
        dp[i][i] = cows[i].second;
        ans = max(ans, dp[i][i]);
        for (int j = 0; j < i; ++j) {
            // current jump length = cows[i].first - cows[j].first
            for (int k = j; k >= 0; --k) {
                // previous jump length = cows[j].first - cows[k].first
                if (cows[i].first - cows[j].first < cows[j].first - cows[k].first) break;
                dp[i][j] = max(dp[i][j], dp[j][k] + cows[i].second);
            }
            ans = max(ans, dp[i][j]);
        }
    }
    
    // Reset dp and reverse order for leftward pass
    fill(dp.begin(), dp.end(), vector<LL>(n, 0));
    reverse(cows.begin(), cows.end());
    
    // Pass 2: leftward jumps (equivalent to rightward on reversed array)
    for (int i = 0; i < n; ++i) {
        dp[i][i] = cows[i].second;
        ans = max(ans, dp[i][i]);
        for (int j = 0; j < i; ++j) {
            for (int k = j; k >= 0; --k) {
                if (cows[i].first - cows[j].first < cows[j].first - cows[k].first) break;
                dp[i][j] = max(dp[i][j], dp[j][k] + cows[i].second);
            }
            ans = max(ans, dp[i][j]);
        }
    }
    
    return ans;
}

#include <cassert>
#include <vector>
#include <utility>
using namespace std;
using LL = long long;
using pi = pair<LL, LL>;

// Declaration (solution function is provided above)
LL maxPogoScore(vector<pi>& cows);

int main() {
    // Test 1: single cow
    vector<pi> c1 = {{5, 10}};
    assert(maxPogoScore(c1) == 10);
    
    // Test 2: two cows increasing position and points
    vector<pi> c2 = {{1, 3}, {4, 5}};
    assert(maxPogoScore(c2) == 8); // jump from 1 to 4 (distance 3) total 8
    
    // Test 3: unsorted input
    vector<pi> c3 = {{10, 2}, {0, 7}, {5, 1}};
    // sorted: (0,7), (5,1), (10,2) -> best: start at 0, jump to 5? distance 5, then jump to 10? distance 5, non-increasing? 5 <= 5 okay, total 7+1+2=10. Or start at 0 alone =7. Also from 10 leftward: 10 to 5 (dist5), 5 to0 (dist5) total 2+1+7=10.
    assert(maxPogoScore(c3) == 10);
    
    // Test 4: decreasing points
    vector<pi> c4 = {{0, 100}, {1, 1}, {2, 99}};
    // sorted: (0,100),(1,1),(2,99) -> best: start at 0, jump to 2? dist2, then no more. total 199. Or 0 alone 100. Or 2 to 0? reversed same.
    assert(maxPogoScore(c4) == 199);
    
    // Test 5: all zeros
    vector<pi> c5 = {{0,0},{1,0},{2,0}};
    assert(maxPogoScore(c5) == 0);
    
    // Test 6: strict decreasing jump lengths needed
    // positions: 0,3,5,8. points: 1,10,100,1000
    // sorted: (0,1),(3,10),(5,100),(8,1000)
    // start at 8 -> 5 (dist3) -> 0 (dist5? 5-0=5) fails because prev=3, next must <=3, so can't. Best: 8->5 (dist3) total 1100, or 8 alone 1000, or 5->3 (dist2) then ->0? dist3 >2 fails, so 5->3 total 110. So best 1100.
    vector<pi> c6 = {{0,1},{3,10},{5,100},{8,1000}};
    assert(maxPogoScore(c6) == 1100);
    
    // Test 7: leftward better than rightward
    // positions: 0,2,5,9. points: 100,1,1,100
    // Rightward: start at 0->2 (dist2) total101, then 2->5? dist3>2 no, or 0->5? dist5 total101, 0 alone100. So best101.
    // Leftward: start at 9->5 (dist4) total101, then 5->2? dist3<=4 yes total102, then 2->0? dist2<=3 yes total103. So answer 103.
    vector<pi> c7 = {{0,100},{2,1},{5,1},{9,100}};
    assert(maxPogoScore(c7) == 103);
    
    // Test 8: large values, ensure no overflow within long long
    vector<pi> c8 = {{0, 1000000}, {1, 2000000}, {2, 3000000}};
    assert(maxPogoScore(c8) == 6000000); // all three collected
    
    return 0;
}

// The problem is a variant of the longest increasing subsequence with a constraint on the decreasing jump lengths, applied in both directions. Sort the cows by position ascending. Define `dp[i][j]` as the maximum points collected on a path ending with a jump from cow `j` to cow `i` (where `i > j` in sorted order), and where the last jump length is `pos[i]-pos[j]`. Base case: `dp[i][i] = points[i]` for starting at cow `i` (no previous jump). For each pair `(i,j)` with `i>j`, we consider all possible previous cows `k` such that `k <= j` and the previous jump length `pos[j]-pos[k]` is at least the current jump length `pos[i]-pos[j]`, i.e., `pos[i]-pos[j] <= pos[j]-pos[k]`. Then `dp[i][j] = max(dp[i][j], dp[j][k] + points[i])`. Since positions are sorted, for a fixed `j`, as `k` decreases, `pos[j]-pos[k]` increases, so we can break the inner loop as soon as the condition fails. After computing all `dp[i][j]` for rightward movement, we take the maximum over all `dp` values. Then repeat the same process on the reversed sorted array to handle leftward movement (or equivalently, run the same DP on the reversed order). The answer is the maximum over both passes. Edge cases: single cow (answer is its points), all points zero, points may be large (use `long long`). Time complexity: O(n^3) in the worst case due to three nested loops, but the break condition often reduces it; with n ≤ 1000, this is acceptable (about 10^9 operations worst-case, but typical break reduces it; still fine for competitive tasks). Space complexity: O(n^2) for the DP table.
