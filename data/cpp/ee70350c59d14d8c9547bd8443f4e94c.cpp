// You are given three integers: `n` (number of problems in a contest), `k` (number of topics), and `m` (total time available in minutes). You are also given an array `a` of length `k` where `a[i]` is the time (in minutes) required to solve a problem on topic `i`. You can solve problems in any order, but for each topic, you must solve all its problems consecutively (i.e., you cannot interleave topics). The scoring is: each solved problem gives 1 point, and each topic that is completely solved (all `n` problems of that topic? No—clarify: there are `n` problems total across all topics? Actually re-read the snippet: `defi(n,k,m)` and `defvi(a,k)`, then `Sum(a)` is the total time for one round of all `k` topics, and the loop `rep(i, n+1)` suggests `n` is the number of complete rounds you attempt, each round costs `C` time and gives `k+1` points (`k` problems plus a bonus point for completing a full set). The snippet allows `i` complete rounds, then fills remaining time with individual problems from `n-i` rounds' worth of problems (each round provides one problem per topic). Write a function `maxPoints(int n, int k, int m, const std::vector<int>& a)` that returns the maximum points achievable given the constraints. The rules: For each full round of `k` problems (one from each topic), you get `k+1` points (bonus for finishing a set). You may do `i` full rounds for any `i` from 0 to `n`, provided `i * C <= m` where `C` is the sum of `a`. After doing `i` full rounds, you have time `m - i*C` left. You may then solve individual problems from the remaining `n-i` sets (each set contains one problem per topic). You can pick any individual problems, but you cannot get the bonus for incomplete sets. Each problem takes its topic's time. You must maximize total points (each solved problem gives 1 point, plus `k` points for each full round? Actually each full round gives `k` problems plus 1 bonus = `k+1` points). The array `a` may contain distinct or duplicate times, and all values are positive integers. Assume `1 <= n, k <= 100`, `0 <= m <= 10^6`, and each `a[i]` between 1 and 1000. The function should return the maximum possible points.
#include <cassert>
#include <vector>

// The solution function is declared above (or included in the same translation unit).
int main() {
    // Basic test: n=1, k=1, m=5, a=[3] -> one full round costs 3, gives 2 points.
    // Rest=2, no more problems, so 2 points. Or i=0: one problem of 3, rest=5, get 1 point.
    assert(maxPoints(1, 1, 5, {3}) == 2);

    // n=2, k=1, m=10, a=[3] -> C=3.
    // i=0: two problems (3,3) sorted, rest=10 -> get 2 points.
    // i=1: cost 3, rest=7, points=2, then one leftover problem of 3 -> total 3.
    // i=2: cost 6, rest=4, points=4 (2 rounds of k+1=2 each), no leftovers -> 4.
    // But check i=2: rest=4, no problems left, so 4. So max = 4.
    assert(maxPoints(2, 1, 10, {3}) == 4);

    // n=2, k=2, m=10, a=[1,5] -> C=6.
    // i=0: probs=[1,5,1,5] sorted -> [1,1,5,5] rest=10: take 1,1,5 (total 3) stop at 5? 1+1+5=7 <=10, then next 5 >3 rest -> 3 points.
    // i=1: cost=6, rest=4, points=3, leftover probs=[1,5] sorted [1,5] take 1 -> points=4.
    // i=2: cost=12 >10 break. So max=4.
    assert(maxPoints(2, 2, 10, {1,5}) == 4);

    // No time: m=0 always 0.
    assert(maxPoints(3, 2, 0, {1,2}) == 0);

    // Large n, small times
    // n=3, k=1, m=5, a=[2] -> C=2.
    // i=0: three problems 2 each, rest=5 -> can take 2,2 (2 points) stop at third 2>1
    // i=1: cost=2, rest=3, points=2, leftovers two problems of 2 -> take one (2) => points=3
    // i=2: cost=4, rest=1, points=4, leftover one problem 2 >1 => points=4
    // i=3: cost=6>5 break. Max=4.
    assert(maxPoints(3, 1, 5, {2}) == 4);

    // All topics same time
    assert(maxPoints(1, 3, 10, {2,2,2}) == 4); // one full round cost 6, gives 4 points, rest=4, no leftover, so 4.

    // Edge: m exactly enough for one full round, but leftover can be taken
    // n=2, k=2, m=6, a=[1,2] -> C=3.
    // i=0: probs=[1,2,1,2] sorted -> [1,1,2,2] rest=6: take all 4 -> 4 points.
    // i=1: cost=3, rest=3, points=3, leftovers=[1,2] take 1,2 -> total 5.
    // i=2: cost=6, rest=0, points=6 -> 6 points.
    assert(maxPoints(2, 2, 6, {1,2}) == 6);

    // Test with duplicate times in a
    assert(maxPoints(2, 2, 7, {2,3}) == 4); // C=5. i=1 cost=5 rest=2 points=3, leftovers [2,3] take 2 ->4. i=0: probs [2,3,2,3] sorted [2,2,3,3] rest=7: take 2,2,3=7 -> 3 points. i=2 cost=10 break. So 4.
}
#include <vector>
#include <algorithm>
#include <numeric>

// Given n complete sets (each set has k problems, one per topic), total time m,
// and per-topic times a[], return the maximum achievable points.
// Each complete set gives k+1 points (k problems + 1 bonus).
// After choosing i complete sets, remaining individual problems are taken greedily.
int maxPoints(int n, int k, int m, const std::vector<int>& a) {
    int C = std::accumulate(a.begin(), a.end(), 0);
    int best = 0;
    for (int i = 0; i <= n; ++i) {
        long long fullTime = 1LL * C * i;
        if (fullTime > m) break; // further i only increase time
        int rest = m - static_cast<int>(fullTime);
        int points = (k + 1) * i;
        // Build list of all individual problems available after i full rounds
        std::vector<int> probs;
        probs.reserve((n - i) * k);
        for (int j = 0; j < n - i; ++j) {
            probs.insert(probs.end(), a.begin(), a.end());
        }
        std::sort(probs.begin(), probs.end());
        // Greedily take smallest problems
        for (int t : probs) {
            if (t > rest) break;
            rest -= t;
            ++points;
        }
        best = std::max(best, points);
    }
    return best;
}
// The key observation is that the maximum points can be obtained by iterating over the number `i` of complete rounds (from 0 to `n`). For each `i`, the total time for the rounds is `i*C` where `C = sum(a)`. If this exceeds `m`, we break because increasing `i` only increases time. Otherwise, we have `rest = m - i*C` remaining. After these `i` rounds, we have `n-i` leftover sets of problems, each set containing one problem per topic (total `(n-i)*k` individual problems). We want to solve as many of these as possible within `rest` time. To maximize the count, we should sort all the individual problem times (all `a[j]` repeated `n-i` times) in ascending order and greedily take the smallest ones until time runs out. The total points for this `i` is `(k+1)*i` (for the full rounds) plus the number of individual problems we managed to solve. We take the maximum over all valid `i`. Edge cases: `m=0` gives 0 points. If `C=0`? But `a[i]` positive, so `C>0`. If `k=0`? The problem likely assumes `k>=1`. Also, `i` can be 0, meaning we just take individual problems. Sorting the combined list each time is O((n-i)*k log((n-i)*k)), which could be up to O(n*k log(n*k)) per iteration. Since n,k ≤ 100, this is fine. Time complexity: O(n * (n*k) log(n*k)) = O(n^2 k log(nk)), space O(n*k) for the temporary vector. We can optimize by precomputing sorted `a` and using prefix sums, but not necessary for constraints. The greedy is correct because all problems have non-negative time and give equal point value (1 point each), so taking smallest times first maximizes count.
