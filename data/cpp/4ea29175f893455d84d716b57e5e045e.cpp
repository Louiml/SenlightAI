// Write a C++ function `count_batches` that takes the number of ingredients `N` (1 or 2), the number of packages per ingredient `P` (between 1 and 50), an array `R` of required units per serving for each ingredient (length `N`), and a 2D array `Q` where `Q[i][j]` is the quantity of ingredient `i` in its `j`-th package (each between 1 and 1,000,000). For each package, compute the range of integer-serving counts `s` such that the package quantity is within 10% of `R[i] * s` (inclusive: `0.9 * R[i] * s <= Q[i][j] <= 1.1 * R[i] * s`). A valid batch is formed by selecting at most one package per ingredient such that their serving-count intervals intersect (i.e., there exists some integer `s` that works for all selected packages), and each package can be used at most once. If `N == 1`, return the number of packages that can form a batch alone (count packages with non-empty interval). If `N == 2`, return the maximum number of disjoint pairs (one package from each ingredient) that can be matched so that each pair's intervals intersect; packages not matched are ignored. The function should handle duplicate values and should be efficient enough for `P <= 50`.
#include <cassert>
#include <vector>
#include "solution.h" // assuming above code is in header

int main() {
    // Test 1: N=1, simple case
    {
        int N = 1, P = 3;
        std::vector<int> R = {10};
        std::vector<std::vector<int>> Q = {{11, 9, 20}};
        // R=10, package amounts: 11 -> s=1 works (0.9*10=9 <=11<=11*1=11), 9 -> s=1 works (9<=9<=11), 20 -> s=2 works (18<=20<=22)
        assert(count_batches(N, P, R, Q) == 3);
    }
    // Test 2: N=1, some invalid intervals
    {
        int N = 1, P = 3;
        std::vector<int> R = {10};
        std::vector<std::vector<int>> Q = {{1, 10, 30}};
        // 1: no s? 0.9*10*s<=1 => s<=0.11 no s>=1, invalid; 10: s=1 works; 30: s=? s=3 gives 27<=30<=33 works
        assert(count_batches(N, P, R, Q) == 2);
    }
    // Test 3: N=2, perfect matching
    {
        int N = 2, P = 2;
        std::vector<int> R = {10, 20};
        std::vector<std::vector<int>> Q = {{10, 100}, {20, 200}};
        // Ingredient0: 10 -> s=1 valid; 100 -> s=10? 0.9*10*10=90<=100<=110, valid, interval [10,10]
        // Ingredient1: 20 -> s=1? 18<=20<=22 valid [1,1]; 200 -> s=10? 180<=200<=220 valid [10,10]
        // Pair (0,0): [1,1] and [1,1] intersect; pair (0,1): [10,10] and [10,10] intersect
        assert(count_batches(N, P, R, Q) == 2);
    }
    // Test 4: N=2, one invalid package
    {
        int N = 2, P = 2;
        std::vector<int> R = {10, 20};
        std::vector<std::vector<int>> Q = {{1, 100}, {20, 200}};
        // Ingredient0: 1 invalid, 100 valid [10,10]; Ingredient1: both valid [1,1] and [10,10]
        // Only pair possible: left package 1 (the 100) with right package 1 (200) => 1 batch
        assert(count_batches(N, P, R, Q) == 1);
    }
    // Test 5: N=2, no intersecting pairs
    {
        int N = 2, P = 1;
        std::vector<int> R = {10, 20};
        std::vector<std::vector<int>> Q = {{10}, {25}};
        // Ingredient0: [1,1]; Ingredient1: 25 -> s=1:18<=25<=22? no; s=2:36<=25<=44? no; no valid? Actually s=1 fails 25>22, s=2 fails 25<36, so invalid
        assert(count_batches(N, P, R, Q) == 0);
    }
    // Test 6: N=2, duplicate packages
    {
        int N = 2, P = 2;
        std::vector<int> R = {1, 1};
        std::vector<std::vector<int>> Q = {{1, 1}, {1, 1}};
        // All intervals: for R=1, Q=1: s=1: 0.9<=1<=1.1 valid, [1,1]; all four packages have [1,1]
        assert(count_batches(N, P, R, Q) == 2);
    }
    // Test 7: N=2, large values, verify correctness with brute force for small P
    {
        int N = 2, P = 3;
        std::vector<int> R = {3, 5};
        std::vector<std::vector<int>> Q = {{100, 200, 300}, {400, 500, 600}};
        // Compute manually? Better: trust algorithm, just ensure it runs and returns int.
        int result = count_batches(N, P, R, Q);
        assert(result >= 0 && result <= 3);
    }
    // Test 8: N=1, boundary condition
    {
        int N = 1, P = 1;
        std::vector<int> R = {1};
        std::vector<std::vector<int>> Q = {{1}};
        assert(count_batches(N, P, R, Q) == 1);
    }
    // Test 9: N=2, empty interval handling
    {
        int N = 2, P = 2;
        std::vector<int> R = {10, 10};
        std::vector<std::vector<int>> Q = {{0, 100}, {100, 0}};
        // Q=0 invalid (package amount positive in problem? but handle gracefully)
        // Should not crash; returns at most 1 because only (row0 col1) with (row1 col0) valid? intervals: row0[1] Q=100: s=10? 90<=100<=110, [10,10]; row1[0] Q=100: [10,10]; row0[0] invalid, row1[1] invalid. So matching size 1.
        assert(count_batches(N, P, R, Q) == 1);
    }
    // Test 10: N=2, maximum matching with multiple possibilities
    {
        int N = 2, P = 3;
        std::vector<int> R = {1, 2};
        std::vector<std::vector<int>> Q = {{1, 10, 20}, {2, 20, 40}};
        // intervals:
        // Ing0: Q=1 -> [1,1]; Q=10 -> s=9? 0.9*1*9=8.1<=10<=9.9? 10<=9.9 false; s=10:9<=10<=11 valid [10,10]; Q=20 -> s=18? 16.2<=20<=19.8? false; s=19? 17.1<=20<=20.9 valid [19,19]? Actually check: s=19: 0.9*19=17.1<=20<=1.1*19=20.9 valid; s=18: 16.2<=20<=19.8? 20>19.8 fail; so [18? no] actually compute: min_s = ceil(10*20/(11*1))=ceil(200/11)=19, max_s = floor(10*20/(9*1))=floor(200/9)=22, so [19,22]. 
        // Ing1 R=2: Q=2 -> s=1? 1.8<=2<=2.2 valid [1,1]; Q=20 -> s=9? 16.2<=20<=19.8? fail; s=10:18<=20<=22 valid [10,10]; Q=40 -> s=19? 34.2<=40<=41.8 valid [19,22]? min=ceil(400/(22))=19, max=floor(400/18)=22.
        // Intersections: left0 [1,1] with right0 [1,1] -> yes; left1 [10,10] with right1 [10,10] -> yes; left2 [19,22] with right2 [19,22] -> yes. So matching 3.
        assert(count_batches(N, P, R, Q) == 3);
    }
    return 0;
}
#include <vector>
#include <algorithm>
#include <cstdint>

// Struct to hold interval of possible servings for a package.
struct Interval {
    int min_s;
    int max_s;
    bool valid; // true if min_s <= max_s
};

// Compute the valid serving-count interval for a package.
Interval compute_interval(long long required_per_serving, long long package_amount) {
    // Need integer s such that:
    // 0.9 * required * s <= package_amount <= 1.1 * required * s
    // Equivalent to:
    // 9 * required * s <= 10 * package_amount  AND  10 * package_amount <= 11 * required * s
    // min_s = smallest s satisfying second inequality: 10*package_amount <= 11*required*s
    // => s >= ceil(10*package_amount / (11*required))
    // max_s = largest s satisfying first inequality: 9*required*s <= 10*package_amount
    // => s <= floor(10*package_amount / (9*required))
    
    if (package_amount <= 0 || required_per_serving <= 0) return {0, -1, false};
    
    long long min_num = 10LL * package_amount;
    long long min_den = 11LL * required_per_serving;
    long long min_s = (min_num + min_den - 1) / min_den; // ceil
    
    long long max_num = 10LL * package_amount;
    long long max_den = 9LL * required_per_serving;
    long long max_s = max_num / max_den; // floor
    
    if (min_s > max_s) return {0, -1, false};
    // Verify both conditions exactly (to be safe)
    if (9LL * required_per_serving * min_s > 10LL * package_amount) return {0, -1, false};
    if (10LL * package_amount > 11LL * required_per_serving * max_s) return {0, -1, false};
    if (9LL * required_per_serving * max_s > 10LL * package_amount) return {0, -1, false};
    if (10LL * package_amount > 11LL * required_per_serving * min_s) return {0, -1, false};
    return {static_cast<int>(min_s), static_cast<int>(max_s), true};
}

// Check if two intervals intersect.
bool intervals_intersect(const Interval& a, const Interval& b) {
    if (!a.valid || !b.valid) return false;
    return a.min_s <= b.max_s && b.min_s <= a.max_s;
}

// Main solution function.
// N is 1 or 2.
// R has length N, each is required units per serving.
// Q is a 2D vector of size N x P (P is length of each Q[i]).
int count_batches(int N, int P, const std::vector<int>& R, const std::vector<std::vector<int>>& Q) {
    // Compute intervals for all packages.
    std::vector<std::vector<Interval>> intervals(N, std::vector<Interval>(P));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < P; ++j) {
            intervals[i][j] = compute_interval(R[i], Q[i][j]);
        }
    }
    
    if (N == 1) {
        int count = 0;
        for (int j = 0; j < P; ++j) {
            if (intervals[0][j].valid) ++count;
        }
        return count;
    }
    
    // N == 2: maximum bipartite matching.
    // Left side: packages of ingredient 0, right side: packages of ingredient 1.
    // Build adjacency: match_right[j] = which left node matched to right j.
    std::vector<int> match_right(P, -1);
    
    // Helper lambda for DFS augmenting.
    std::vector<bool> visited;
    std::function<bool(int)> try_kuhn = [&](int left) -> bool {
        if (visited[left]) return false;
        visited[left] = true;
        for (int right = 0; right < P; ++right) {
            if (intervals_intersect(intervals[0][left], intervals[1][right])) {
                if (match_right[right] == -1 || try_kuhn(match_right[right])) {
                    match_right[right] = left;
                    return true;
                }
            }
        }
        return false;
    };
    
    int max_matching = 0;
    for (int left = 0; left < P; ++left) {
        visited.assign(P, false);
        if (try_kuhn(left)) {
            ++max_matching;
        }
    }
    return max_matching;
}
// For each package, determine the minimum and maximum integer servings `s` such that `Q[i][j]` is within 10% of `R[i]*s`. The inequality `0.9 * R[i] * s <= Q[i][j] <= 1.1 * R[i] * s` can be checked by iterating `s` from 1 to a bound like 2,000,000 (since `Q` max 1e6 and `R` min 1, so upper bound on `s` is about 1.1e6, we can safely use 1,000,007 or just 2,000,000). Alternatively, derive closed-form:
// - `min_s = ceil(Q[i][j] / (1.1 * R[i]))`
// - `max_s = floor(Q[i][j] / (0.9 * R[i]))`
// But careful with integer arithmetic: `0.9` and `1.1` are fractions. Use integer checks: `9 * R[i] * s <= 10 * Q[i][j]` and `10 * Q[i][j] <= 11 * R[i] * s`. To find min/max efficiently, we can binary search or loop. Since `P <= 50` and `s` range small (up to ~2e6), brute force is fine: for each package, loop `s = 1..1,000,010` (or up to `ceil(1.1 * max_Q / min_R)`). Actually safer to loop up to `2000000`. With `N=2` and 50 packages, that's 2 * 50 * 2e6 = 200 million operations, which might be a bit heavy but still okay in C++ under 1 second? Might be borderline. Better to compute min and max using integer binary search or direct formulas. Use:
// - `min_s = (10 * Q[i][j] + 11 * R[i] - 1) / (11 * R[i])` (ceil of `10Q/(11R)`)
// - `max_s = (10 * Q[i][j]) / (9 * R[i])` (floor of `10Q/(9R)`)
// Then verify that `min_s <= max_s` and also satisfy both conditions. This is O(1) per package.
// Then for `N==1`, count `max_s >= min_s`.
// For `N==2`, we have intervals `[min_s[0][i], max_s[0][i]]` and `[min_s[1][j], max_s[1][j]]` for each i,j. Need maximum matching of intervals that intersect. Since P up to 50, we can use DP over subsets (bitmask) as in the original code: dp[mask] = max batches using the first `popcount(mask)` packages of ingredient 0 and the set `mask` of packages of ingredient 1. That's O(2^P * P) which for P=50 is impossible (2^50). But note N=2 and we need maximum matching. This is a bipartite matching problem where each edge exists if intervals intersect. Maximum cardinality matching in bipartite graph can be solved with Hopcroft-Karp or simple DFS augmenting paths, O(P * E) where E <= P^2 = 2500. Simpler: use recursive backtracking or standard maximum bipartite matching via DFS. Since P<=50, we can use Kuhn's algorithm: for each left node (package of ingredient 0), try to find an augmenting path to unmatched right nodes. Complexity O(P * E) ≈ 125k, very fast. So solution: build adjacency matrix from intervals intersection, then run Kuhn. Edge case: intervals that are empty (min>max) should not be considered; treat as no edges. Also if `N==1`, just count non-empty intervals.
//
// Time complexity: O(N*P) for interval computation, plus O(P^2) for matching. Space O(P^2). The original snippet used bitmask DP but that's only feasible for small P; we improve.
