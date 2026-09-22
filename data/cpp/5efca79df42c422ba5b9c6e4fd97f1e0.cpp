/*
Given an array `h` of heights and an array `t` of types (0 or 1), each indexed from 1 to `n`. For each index `i`, define `ans[i]` using the following iterative procedure over heights from 1 to `max(h)`: Let `s[x] = count of all indices j ≤ x` (i.e., cumulative count of elements whose height ≤ x). Let `p[x]` be a cumulative sum of some "score" values — specifically, for indices with type 1 at height x, their `ans` equals `(p[x - t[i]] > 0 ? p[x - t[i]] / s[x - t[i]] + 1 : 1)`, and then `p[x] = sum of all ans for type 1 at height x + p[x-1]`. For indices with type 0 at height x, if all indices up to height x are type 0 (i.e., `s[x] == count of type0 up to x`), set their `ans` and `p[x]` to infinity. Otherwise, set their `ans = (p[x] + s[x]) / (s[x] - count_type0_at_height_x)`, and add `ans * count_type0_at_height_x` to `p[x]`. Write a C++ function that takes `n`, `vector<int> h`, `vector<int> t` (1-indexed internally) and returns a `vector<double>` containing `ans[i]` for i=1..n, with values exceeding 1e18 replaced by 0.0. The function must handle up to 1e6 elements efficiently.
*/
#include <vector>
#include <algorithm>
#include <cmath>

// Compute ans for the described process.
// h: heights (1-indexed internally), t: types (1-indexed internally)
// returns vector<double> ans for each index, with values >1e18 replaced by 0.0
std::vector<double> computeScores(int n, const std::vector<int>& h, const std::vector<int>& t) {
    const double INF = 1e50;
    const double LIMIT = 1e18;

    // 1-indexed vectors
    std::vector<int> hh(n + 1), tt(n + 1);
    for (int i = 1; i <= n; ++i) {
        hh[i] = h[i - 1];
        tt[i] = t[i - 1];
    }

    int mx = *std::max_element(hh.begin() + 1, hh.end());

    // buckets: heights -> indices
    std::vector<std::vector<int>> t0(mx + 1), t1(mx + 1);
    for (int i = 1; i <= n; ++i) {
        if (tt[i]) t1[hh[i]].push_back(i);
        else t0[hh[i]].push_back(i);
    }

    std::vector<double> s(mx + 1, 0.0);
    std::vector<double> p(mx + 1, 0.0);
    std::vector<double> ans(n + 1, 0.0);

    for (int i = 1; i <= mx; ++i) {
        // cumulative count up to height i
        s[i] = (double)(t1[i].size() + t0[i].size()) + s[i - 1];

        // process type 1 at this height
        double dps = 0.0;
        for (int u : t1[i]) {
            int back = tt[u]; // t[u] is 1 for type 1? Wait careful: t[u] is 0 or 1. For type1, t[u]=1.
            // Actually tt[u] is the type, not a distance! The snippet uses t[u] as "t[i]" but it's the type value (0/1). 
            // In original snippet, t[u] is 0 or 1, and they use t[u] as a distance? Wait: look at code: t[i] is 0 or 1, and they use i - t[u] as index. 
            // That means t[u] is the type value (0 or 1), so for type1 (t[u]=1), they index i-1, for type0 (t[u]=0) index i.
            // But in this task description it said "looks back t[i] units in height" — that's wrong. Actually original snippet uses t[u] as shift amount equal to its type value (0 or 1). So type 1 looks back 1 height, type 0 looks back 0. Let me adjust.
            // Therefore for type 1 elements (tt[u]=1), back = 1; for type 0, back = 0.
            int backShift = (tt[u] == 1) ? 1 : 0;
            int idx = i - backShift;
            if (idx >= 1 && s[idx] > 0.0) {
                ans[u] = p[idx] / s[idx] + 1.0;
            } else {
                ans[u] = 1.0;
            }
            dps += ans[u];
        }
        p[i] = dps + (i > 0 ? p[i - 1] : 0.0);

        // process type 0 at this height
        if (t0[i].empty()) continue;

        // check if all elements up to i are type 0
        if ((long long)s[i] == (long long)t0[i].size() && (i == 1 || (long long)s[i-1] == 0)) {
            // Actually the condition in original: if(int(s[i]) == t0[i].size()) set p[i]=inf and ans for these to inf
            // But careful: s[i] is cumulative count, t0[i].size() is only at this height. The condition is s[i] == t0[i].size() means no type1 anywhere up to i.
            // Let's just replicate: if int(s[i]) == t0[i].size() (i.e., no type1 up to i), set p[i]=inf and ans for these = inf.
            p[i] = INF;
            for (int u : t0[i]) ans[u] = INF;
            continue;
        }

        // normal case: compute dps for type0 at this height
        double denom = s[i] - (double)t0[i].size(); // exclude current type0 count from total up to i? Wait s[i] includes them, so denominator = s[i] - t0[i].size() is total up to i excluding current type0.
        // Actually original: (p[i] + s[i]) / (s[i] - t0[i].size()) — p[i] already includes previous heights' scores, s[i] includes current heights' counts.
        dps = (p[i] + s[i]) / denom;
        for (int u : t0[i]) {
            ans[u] = dps;
        }
        p[i] += dps * (double)t0[i].size();
    }

    // prepare output: replace >1e18 with 0.0
    std::vector<double> result(n);
    for (int i = 1; i <= n; ++i) {
        if (ans[i] > LIMIT) result[i - 1] = 0.0;
        else result[i - 1] = ans[i];
    }
    return result;
}
#include <cassert>
#include <vector>
#include <cmath>

// Assume computeScores is defined above.

int main() {
    // Test 1: simple case with type1 only
    {
        std::vector<int> h = {1, 2, 3};
        std::vector<int> t = {1, 1, 1};
        auto res = computeScores(3, h, t);
        // Height 1: type1, idx = 0, s[0]=0 -> ans=1.0
        // Height 2: type1, idx=1, s[1]=1, p[1]=1 -> ans=1/1+1=2.0
        // Height 3: type1, idx=2, s[2]=2, p[2]=3 -> ans=3/2+1=2.5
        assert(fabs(res[0] - 1.0) < 1e-9);
        assert(fabs(res[1] - 2.0) < 1e-9);
        assert(fabs(res[2] - 2.5) < 1e-9);
    }

    // Test 2: type0 only, multiple heights
    {
        std::vector<int> h = {1, 2};
        std::vector<int> t = {0, 0};
        auto res = computeScores(2, h, t);
        // Height 1: s[1]=1, no type1, t0 count=1, s[1]==t0.size() -> inf -> output 0
        // Height 2: same, inf -> output 0
        assert(res[0] == 0.0);
        assert(res[1] == 0.0);
    }

    // Test 3: mixed types
    {
        std::vector<int> h = {1, 1, 2};
        std::vector<int> t = {1, 0, 1};
        auto res = computeScores(3, h, t);
        // Height1: t1 has index1, t0 has index2. 
        // Process t1: idx=0 -> s[0]=0 -> ans[1]=1
        // p[1] = 1 + 0 = 1
        // t0[i] nonempty, s[1]=2, t0.size()=1, s[1]!=t0.size() (2!=1) -> denom=2-1=1, dps=(1+2)/1=3, ans[2]=3, p[1]=1+3*1=4
        // Height2: t1 has index3, idx=1, s[1]=2, p[1]=4 -> ans[3]=4/2+1=3
        // p[2] = 3 + 4 = 7, no t0 at height2
        assert(fabs(res[0] - 1.0) < 1e-9);
        assert(fabs(res[1] - 3.0) < 1e-9);
        assert(fabs(res[2] - 3.0) < 1e-9);
    }

    // Test 4: larger, check overflow handling
    {
        std::vector<int> h = {1, 2, 3, 4, 5};
        std::vector<int> t = {0, 0, 0, 0, 0};
        auto res = computeScores(5, h, t);
        // all type0, each height only has its own count, s[i] == t0[i].size() at each i because no type1 anywhere, so inf -> 0
        for (double v : res) assert(v == 0.0);
    }

    // Test 5: single element type1
    {
        std::vector<int> h = {10};
        std::vector<int> t = {1};
        auto res = computeScores(1, h, t);
        assert(fabs(res[0] - 1.0) < 1e-9);
    }

    // Test 6: multiple type1 at same height
    {
        std::vector<int> h = {2, 2};
        std::vector<int> t = {1, 1};
        auto res = computeScores(2, h, t);
        // height1: no elements, s[1]=0, p[1]=0
        // height2: both type1, idx=1, s[1]=0 -> ans=1 each, p[2]=2+0=2
        assert(fabs(res[0] - 1.0) < 1e-9);
        assert(fabs(res[1] - 1.0) < 1e-9);
    }

    return 0;
}
// The problem describes a dynamic process that processes heights in increasing order. For each height `i`, we maintain two arrays: `s[i]` = cumulative count of all elements with height ≤ i, and `p[i]` = a running cumulative score. For type 1 (t[i]=1) at height i, each element looks back `t[i]` units in height to compute a ratio using the previous cumulative score and count; if `s[i - t[i]] > 0`, the ratio is `p[i - t[i]] / s[i - t[i]] + 1`, else 1. For type 0 (t[i]=0) elements at height i: if there are no type 1 elements at this height and all elements up to i are type 0 (i.e., `s[i] == count_type0[i]`), then the entire process becomes degenerate (no seeds), so all type 0 at this height get infinity and `p[i]` becomes infinity too. Otherwise, compute a single value `dps = (p[i] + s[i]) / (s[i] - count_type0_at_height_i)` and assign it to all type 0 at this height, then add `dps * count_type0_at_height_i` to `p[i]`. Note: `p[i]` at the start of processing height i includes contributions from all previous heights. After processing all heights, any `ans` exceeding 1e18 is replaced by 0.0 in the output. The algorithm runs in O(n + maxHeight) time and O(n + maxHeight) space. Edge cases: large values causing potential overflow – use `long double` or `double` with care, and output formatting requires printing `0` when >1e18. Also handle when `s[i - t[i]] == 0` for type 1 (set ans=1). The arrays are 1-indexed internally; the function should convert input to 1-indexed vectors.
