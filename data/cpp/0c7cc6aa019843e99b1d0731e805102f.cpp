/*
You are given a series of upgrades that take effect at specific starting positions along a linear path from position 0 to position 100000. Initially, at position 0, the daily progress rate is `start` units per day. When you reach a position `a` where an upgrade is defined, the daily rate permanently becomes `s` for all subsequent positions (including `a` itself). There are no other changes to the rate. You need to answer `m` queries. Each query provides a target cumulative progress value `p` and a limit position `x`. For a query, consider two possible goals: (1) reaching cumulative progress `p` without ever passing position `x` (i.e., you are only allowed to progress to positions in the range `[0, x]`), and (2) first reaching cumulative progress `p` while traveling all the way past position `x` (i.e., you are allowed to continue beyond `x`; note the cumulative progress at position `x` is known, and you need to reach `p` in the expanded path). For each query, output the earliest position (the smallest position index) at which the cumulative progress after that position equals or exceeds `p`, under the appropriate scenario: if the progress at position `x` is already at least `p`, use scenario (1) within `[0,x]`; otherwise use scenario (2) in the infinite path (bounded by 100000). There is at most one upgrade per position, but positions may be omitted; if no upgrade exists at a position, the rate remains the previous rate. The total number of upgrades `n`, number of queries `m`, initial rate `start`, and all input values are integers. All positions `a` are between 1 and 100000 inclusive. The cumulative progress values are non-negative and can be large; however, the final cumulative progress at position 100000 is guaranteed to be at least the maximum `p` value (so a solution always exists). Write a C++ function that, given `n`, `m`, `start`, the list of upgrade positions and rates, and the list of queries, returns a vector of integers (the answers to each query in order).
*/
#include <vector>
#include <algorithm>
#include <unordered_map>

// Given a linear path of positions 0..100000, an initial daily rate at position 0,
// and a list of upgrades (position -> new rate) that take effect when reaching that position,
// answer each query (p, x) with the earliest position where cumulative progress >= p,
// respecting the rule: if progress at x already reaches p, stop within [0,x]; else continue past x.
// Assumes all rates are non-negative and progress array is non-decreasing.
std::vector<int> answerQueries(
    int n, int m, int start,
    const std::vector<std::pair<int,int>>& upgrades,
    const std::vector<std::pair<int,int>>& queries) {
    
    const int MAX_POS = 100000;
    // current rate at each position (starting from position 1)
    std::unordered_map<int,int> rateChange;
    for (const auto& up : upgrades) {
        rateChange[up.first] = up.second; // upgrade at position up.first
    }
    
    std::vector<long long> progress(MAX_POS + 1, 0);
    long long currentRate = start;
    for (int i = 1; i <= MAX_POS; ++i) {
        if (rateChange.find(i) != rateChange.end()) {
            currentRate = rateChange[i];
        }
        progress[i] = progress[i-1] + currentRate;
    }
    
    std::vector<int> answers;
    answers.reserve(m);
    for (const auto& q : queries) {
        int p = q.first;
        int x = q.second;
        if (x > MAX_POS) x = MAX_POS; // safety
        if (progress[x] >= p) {
            // find first index in [0, x] with progress >= p
            int pos = std::lower_bound(progress.begin(), progress.begin() + x + 1, (long long)p) - progress.begin();
            answers.push_back(pos);
        } else {
            // find first index in [x, MAX_POS] with progress >= p (since monotonic, this is >= x)
            int pos = std::lower_bound(progress.begin() + x, progress.end(), (long long)p) - progress.begin();
            answers.push_back(pos);
        }
    }
    return answers;
}
#include <cassert>
#include <vector>

// Assume the solution function is defined as above.

int main() {
    // Test 1: Simple constant rate
    // n=0 upgrades, start=5, query p=12, x=2 -> progress[2]=10 <12, so search from >=2: progress[3]=15 => answer 3
    std::vector<std::pair<int,int>> up1;
    std::vector<std::pair<int,int>> q1 = {{12,2}};
    auto res1 = answerQueries(0,1,5,up1,q1);
    assert(res1[0] == 3);
    
    // Test 2: Upgrade at position 2 to 10, start=1. progress: [0]=0,1:1,2:1+10=11,3:21,...
    // Query p=5,x=1 -> progress[1]=1<5, so search from index1: progress[2]=11>=5 => answer 2
    std::vector<std::pair<int,int>> up2 = {{2,10}};
    std::vector<std::pair<int,int>> q2 = {{5,1}};
    auto res2 = answerQueries(1,1,1,up2,q2);
    assert(res2[0] == 2);
    
    // Test 3: Query where progress at x already enough
    // start=3, upgrades none, p=6,x=3 -> progress[3]=9>=6, search [0,3]: progress[2]=6 => answer 2
    std::vector<std::pair<int,int>> up3;
    std::vector<std::pair<int,int>> q3 = {{6,3}};
    auto res3 = answerQueries(0,1,3,up3,q3);
    assert(res3[0] == 2);
    
    // Test 4: Multiple upgrades, ensure monotonic
    // start=2, upgrade at pos5=0 (rate drops to 0), p=6,x=4: progress[4]=8>=6, search [0,4]: progress[3]=6 => answer 3
    std::vector<std::pair<int,int>> up4 = {{5,0}};
    std::vector<std::pair<int,int>> q4 = {{6,4}};
    auto res4 = answerQueries(1,1,2,up4,q4);
    assert(res4[0] == 3);
    
    // Test 5: Large p requiring full path
    // start=1, upgrades none, p=100001,x=0 -> progress[0]=0<100001, search from index0: progress[100000]=100000 <100001, but guaranteed solution? Actually 100000 <100001, so no solution. To avoid, use p=100000 => answer 100000
    std::vector<std::pair<int,int>> up5;
    std::vector<std::pair<int,int>> q5 = {{100000,0}};
    auto res5 = answerQueries(0,1,1,up5,q5);
    assert(res5[0] == 100000);
    
    // Test 6: Query with x at end
    // start=10, no upgrades, p=5,x=100000 => progress[100000]=1e6 >=5, search [0,100000]: progress[1]=10>=5 => answer 1
    std::vector<std::pair<int,int>> up6;
    std::vector<std::pair<int,int>> q6 = {{5,100000}};
    auto res6 = answerQueries(0,1,10,up6,q6);
    assert(res6[0] == 1);
    
    // Test 7: Multiple queries
    // start=2, upgrade at pos3=5, queries: (5,2) -> progress[2]=4<5, search from 2: progress[3]=9>=5 => ans 3; (7,3) -> progress[3]=9>=7, search [0,3]: progress[3]=9>=7, progress[2]=4<7 => ans 3; (10,4) -> progress[4]=14>=10, search [0,4]: progress[3]=9<10, progress[4]=14 => ans 4
    std::vector<std::pair<int,int>> up7 = {{3,5}};
    std::vector<std::pair<int,int>> q7 = {{5,2},{7,3},{10,4}};
    auto res7 = answerQueries(1,3,2,up7,q7);
    assert(res7[0] == 3);
    assert(res7[1] == 3);
    assert(res7[2] == 4);
    
    // Test 8: Rate becomes zero after upgrade
    // start=5, upgrade at pos1=0, p=4,x=0 -> progress[0]=0<4, search from 0: progress[1]=0, progress[2]=0,... never reaches, but problem guarantees solution; so set p=0 -> answer 0
    std::vector<std::pair<int,int>> up8 = {{1,0}};
    std::vector<std::pair<int,int>> q8 = {{0,0}};
    auto res8 = answerQueries(1,1,5,up8,q8);
    assert(res8[0] == 0);
    
    // Test 9: Exact match at boundary
    // start=4, no upgrades, p=8,x=2 -> progress[2]=8>=8, search [0,2]: progress[2]=8 => answer 2
    std::vector<std::pair<int,int>> up9;
    std::vector<std::pair<int,int>> q9 = {{8,2}};
    auto res9 = answerQueries(0,1,4,up9,q9);
    assert(res9[0] == 2);
    
    // Test 10: Upgrade at position 0? Not typical; we ignore (position starts from 1).
    // But ensure no crash with empty upgrades and m=0.
    std::vector<std::pair<int,int>> up10;
    std::vector<std::pair<int,int>> q10;
    auto res10 = answerQueries(0,0,1,up10,q10);
    assert(res10.empty());
    
    return 0;
}
// The problem essentially asks for precomputing an array `cumulative[i]` representing the total progress after being at position `i` (i.e., after traversing exactly `i` steps from position 0, where position 0 has progress 0? In the original snippet, `total[0] = start` and `total[i] = total[i-1] + start` with `start` being the current rate; note that `total[i]` is the cumulative progress after completing step `i` (i.e., after moving from position `i-1` to `i`). So position `i` (for i>=1) has cumulative progress equal to the sum of rates from step 1 to i. For a query with limit `x`, we need to find the earliest position `pos` such that `total[pos] >= p` either under the constraint `pos <= x` (if `total[x] >= p`) or without constraint (if `total[x] < p`). Because `total` is non-decreasing (rates are non-negative? Actually rates could be zero? The problem does not specify positivity; but since `start` and upgrades `s` could be zero or negative? In practice, assume rates are non-negative, likely positive integers. However, the snippet uses `lower_bound` which requires non-decreasing; if negative rates existed, the prefix sums would not be monotonic, but the original code assumes monotonic. So we assume all rates are non-negative. With non-decreasing `total`, we can binary search. Precompute `total` for all positions from 0 to 100000. For a query `(p,x)`: if `total[x] >= p`, answer is the first index `i` in `[0,x]` with `total[i] >= p` (use lower_bound on range `[total.begin(), total.begin()+x+1]`). Else answer is the first index `i` in `[x, 100000]` with `total[i] >= p` (note: since we have already passed `x`, the earliest position that satisfies is the first index >= x where `total[i]` reaches `p`; because `total` is non-decreasing, this is lower_bound from `total.begin()+x`). The original snippet uses `p+total[x]` in the second branch, which is a bit odd; actually `lower_bound(total.begin()+x, total.end(), p+total[x])` would find the first index where `total[i] >= p+total[x]`, which is not correct unless we shift. Wait, the snippet's logic: if `total[x] < p`, then it outputs `lower_bound(total.begin()+x, total.end(), p+total[x]) - total.begin()`. That seems incorrect because `total[x]` is already less than `p`, but then adding `p+total[x]` to the search value gives a much larger threshold. Let's analyze: The snippet's intent might be different: maybe `total[i]` is cumulative progress *after* completing step i, but they want to find the position where cumulative progress reaches `p` after having already reached `x`? Actually reading the original code: they output `(total[x]>=p ? lower_bound(begin,begin+x+1,p) : lower_bound(begin+x,end,p+total[x]))`. If `total[x] < p`, then they search for `p+total[x]` instead of `p`. That seems like a mistake unless the meaning of `total` is different—perhaps `total[i]` is the progress *at* position i before moving? But the code sets `total[0]=start` and `total[i]=total[i-1]+start`, so `total[i]` is the progress after i steps. For a query `(p,x)`, if `total[x] < p`, you need to continue beyond x; the condition to reach `p` is `total[i] >= p` for i >= x. So binary searching for `p` is correct. Searching for `p+total[x]` would be wrong. But in the problem description I wrote, I redefined it correctly: if `total[x] < p`, we search for `p` in the suffix. So my task specification will use the correct logic, not the snippet's possibly buggy version. I'll clarify the semantics accordingly. The algorithm: precompute `total` of size 100001 (indices 0..100000). `total[0]` is initial rate? Actually better to set `total[0] = start`? The snippet does that, but then what does `total[0]` represent? At position 0, you haven't moved, so cumulative progress is 0. But the snippet starts with `total[0]=start`, which implies after 0 steps you already have `start` progress? That is inconsistent. Possibly they define position `i` meaning after completing `i` days, and `total[0]` is progress after 0 days (which should be 0). But they set `total[0]=start` maybe because they count the first day's progress at position 0? I think the intended model is: you start at position 0 with rate `start`, and each unit step you gain `start` progress. So after completing step 1 (to position 1), you have `start` progress. So `total[1] = start`. Hence `total[0]` should be 0. But the snippet sets `total[0]=start`, which shifts everything by one. To avoid ambiguity, I will define: Let `progress[i]` be the total progress after moving from position 0 to position i (i steps). So `progress[0] = 0`. For i from 1 to 100000, if there is an upgrade at position i, the rate for that step is the new rate; else it's the previous rate. So `progress[i] = progress[i-1] + current_rate` where `current_rate` is the rate after applying any upgrade at position i. Then for a query `(p,x)`, we need to find the smallest `i` such that `progress[i] >= p`. The constraint: if `progress[x] >= p`, then we can stop at or before x; else we must go beyond x. So the answer is the lower_bound of `p` in the prefix `[0,x]` if `progress[x] >= p`, otherwise lower_bound of `p` in the full array (since array is monotonic, the first occurrence is at least x). This is simpler and correct. I will implement that. Edge cases: multiple upgrades at same position? The problem says at most one per position, but we can handle by overwriting. If rates can be zero, `progress` is still non-decreasing (if zero, constant). If rates are negative, not monotonic, but we assume non-negative. Time complexity: O(100000 + n + m log 100000) which is O(100000 + m log 100000), space O(100000). For the solution, I will precompute an array of long long to avoid overflow (since p and sums can be large, but given constraints, int may be fine, but use long long for safety). The function signature: `std::vector<int> answerQueries(int n, int m, int start, const std::vector<std::pair<int,int>>& upgrades, const std::vector<std::pair<int,int>>& queries)`. Return vector of int. I'll provide the implementation.
