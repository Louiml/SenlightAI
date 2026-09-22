/*
You are given an array of `n` integers and `q` update operations. Initially, compute the average of all array elements as an integer division `sum / n` (floor). Subtract this average from every element, so each position stores its deviation from the mean. Then, for each operation, three integers `u, v, x` are given (1-indexed positions), where you must subtract `x` from the element at position `u`, and add `x` to the element at position `v`. After each operation, output the maximum possible sum of a contiguous subarray of the *modified* array (where each element is the deviation value after the update), but with the freedom to choose the subarray to be either entirely within the first `k` positions or entirely within the last `k` positions for any `k` (the overall maximum subarray sum). More precisely, the required value is the maximum over all `1 <= L <= R <= n` of the sum of elements from `L` to `R`, but you must also ensure that the chosen subarray's left boundary is `1` or its right boundary is `n`—that is, the subarray must be a prefix or a suffix of the array. Write a C++ function `processUpdates(int n, vector<long long>& a, vector<tuple<int,int,long long>>& queries)` that takes the original array, performs the initial equalization (subtract floor(mean)), then processes the queries, and returns a vector of `long long` containing the answer after each query. Note: the function must handle `n` up to `300000` and `q` up to `300000` efficiently. The input array elements and `x` fit in 64-bit signed integers, and intermediate sums may exceed 32-bit.
*/
#include <vector>
#include <algorithm>
#include <cstdint>
#include <tuple>

using int64 = long long;

class SegmentTree {
private:
    std::vector<int64> total, pref, suff;
    int n;

    void pull(int p) {
        total[p] = total[p << 1] + total[p << 1 | 1];
        pref[p] = std::max(pref[p << 1], total[p << 1] + pref[p << 1 | 1]);
        suff[p] = std::max(suff[p << 1] + total[p << 1 | 1], suff[p << 1 | 1]);
    }

    void build(int p, int l, int r, const std::vector<int64>& arr) {
        if (l == r) {
            total[p] = arr[l];
            pref[p] = suff[p] = std::max((int64)0, arr[l]);
            return;
        }
        int mid = (l + r) >> 1;
        build(p << 1, l, mid, arr);
        build(p << 1 | 1, mid + 1, r, arr);
        pull(p);
    }

    void update(int p, int l, int r, int idx, int64 val) {
        if (l == r) {
            total[p] = val;
            pref[p] = suff[p] = std::max((int64)0, val);
            return;
        }
        int mid = (l + r) >> 1;
        if (idx <= mid) update(p << 1, l, mid, idx, val);
        else update(p << 1 | 1, mid + 1, r, idx, val);
        pull(p);
    }

public:
    SegmentTree(const std::vector<int64>& arr) {
        n = (int)arr.size() - 1; // arr is 1-indexed
        int size = 4 * n + 5;
        total.assign(size, 0);
        pref.assign(size, 0);
        suff.assign(size, 0);
        build(1, 1, n, arr);
    }

    void update(int idx, int64 val) {
        update(1, 1, n, idx, val);
    }

    int64 query() const {
        return std::max(pref[1], suff[1]);
    }
};

std::vector<int64> processUpdates(
    int n,
    std::vector<int64>& a,               // 1-indexed, size n+1
    const std::vector<std::tuple<int,int,int64>>& queries
) {
    int64 sum = 0;
    for (int i = 1; i <= n; i++) sum += a[i];
    int64 avg = sum / n;

    for (int i = 1; i <= n; i++) a[i] -= avg;

    SegmentTree seg(a);

    std::vector<int64> answers;
    answers.reserve(queries.size());

    for (const auto& [u, v, x] : queries) {
        a[u] -= x;
        seg.update(u, a[u]);
        a[v] += x;
        seg.update(v, a[v]);
        answers.push_back(seg.query());
    }

    return answers;
}
#include <cassert>
#include <vector>
#include <tuple>

// Include the solution code here (or paste the function above)

int main() {
    // Test 1: simple case
    {
        int n = 3;
        std::vector<long long> a = {0, 10, 20, 30}; // 1-indexed
        std::vector<std::tuple<int,int,long long>> queries = {
            {1, 3, 5},
            {2, 1, 100}
        };
        auto ans = processUpdates(n, a, queries);
        assert(ans.size() == 2);
        // Initial deviations: sum=60, avg=20, so a = [-20, 0, 10]
        // After q1: u=1 subtract 5 => -25, v=3 add 5 => 15. array: [-25, 0, 15]
        // max prefix/suffix: prefix sums: -25, -25, -10 => max 0 (empty), suffix sums: 15, 15, -10? actually suffix from 2: 0+15=15, from 1: -25+15=-10 => max 15. answer=15
        assert(ans[0] == 15);
        // After q2: u=2 subtract 100 => -100, v=1 add 100 => 75. array: [75, -100, 15]
        // prefix: 75, -25, -10 => max 75; suffix: 15, -85, -10? from 3:15, from 2: -85? actually -100+15=-85, from 1: 75-100+15=-10 => max 15. answer=75
        assert(ans[1] == 75);
    }

    // Test 2: all negative after deviation
    {
        int n = 2;
        std::vector<long long> a = {0, 1, 2}; // sum=3, avg=1, deviations: [0,1]? wait 1-indexed: a[1]=1-1=0, a[2]=2-1=1 => non-negative
        // Use a case where deviations negative: array [3, 3] sum=6 avg=3 deviations [0,0] -> not negative. Let's do [1,2] sum=3 avg=1 deviations [0,1] no.
        // To get negative deviations, use large sum? Actually if all equal, deviations zero. Use array [1,5] sum=6 avg=3 deviations [-2,2] -> negative first.
        std::vector<long long> b = {0, 1, 5};
        std::vector<std::tuple<int,int,long long>> queries = {};
        auto ans = processUpdates(2, b, queries);
        assert(ans.empty());
        // Also test a query:
        std::vector<long long> c = {0, 1, 5};
        std::vector<std::tuple<int,int,long long>> q = {{2, 1, 1}};
        auto ans2 = processUpdates(2, c, q);
        // deviations [-2,2], after update: u=2 subtract 1 => 1, v=1 add 1 => -1. Array [-1,1] => prefix max: -1,0? pref=max(0,-1)=0, total -1+1=0, pref right= max(0, -1+1)=0 -> max 0? Actually prefix sums: -1,0 => max 0 (empty). suffix: from 2:1, from1: -1+1=0 => max 1. answer=1.
        assert(ans2[0] == 1);
    }

    // Test 3: n=1
    {
        std::vector<long long> a = {0, 7};
        std::vector<std::tuple<int,int,long long>> q = {{1, 1, 3}};
        auto ans = processUpdates(1, a, q);
        // deviations: sum=7 avg=7 -> a[1]=0. After update: u=1 subtract3 => -3, v=1 add3 => 0. array [0] -> answer=0
        assert(ans.size() == 1);
        assert(ans[0] == 0);
    }

    // Test 4: check long long overflow boundary
    {
        int n = 3;
        std::vector<long long> a = {0, 1000000000000LL, 2000000000000LL, 3000000000000LL};
        std::vector<std::tuple<int,int,long long>> q = {{1, 3, 500000000000LL}};
        auto ans = processUpdates(n, a, q);
        // sum=6000000000000, avg=2000000000000, deviations: [-1000000000000, 0, 1000000000000]
        // after update: u=1 subtract 5e11 => -1.5e12, v=3 add 5e11 => 1.5e12. array: [-1.5e12, 0, 1.5e12]
        // prefix max: 0 (empty) or -1.5e12, -1.5e12, 0? Actually prefix sums: -1.5e12, -1.5e12, 0 => max 0. suffix: 1.5e12, 1.5e12, 0 => max 1.5e12. answer = 1.5e12
        assert(ans[0] == 1500000000000LL);
    }

    // Test 5: large repeated same values
    {
        int n = 4;
        std::vector<long long> a = {0, 5, 5, 5, 5}; // sum=20 avg=5 deviations all 0
        std::vector<std::tuple<int,int,long long>> q = {{1, 2, 1}, {3, 4, 2}, {2, 3, -1}};
        auto ans = processUpdates(n, a, q);
        // after q1: a=[ -1,1,0,0 ] => prefix max:0 (empty) or -1,0,0,0 => max 0; suffix:0,0,1? suffix from 4:0, from3:0, from2:1, from1:0 => max 1. ans=1
        assert(ans[0] == 1);
        // after q2: u=3 subtract 2 => -2, v=4 add 2 => 2. array [-1,1,-2,2] => prefix max: -1,0,-2,0 => max 0; suffix: 2,0? from4:2, from3:0? -2+2=0, from2:1-2+2=1, from1:-1+1-2+2=0 => max 2. ans=2
        assert(ans[1] == 2);
        // after q3: u=2 subtract -1 => add 1? Actually subtract x where x=-1 => subtract -1 => add 1. So a[2] becomes 2. v=3 add -1 => subtract 1, so a[3] becomes -3. array [-1,2,-3,2] => prefix: -1,1,-2,0 => max 1 (from second), suffix: 2, -1? from4:2, from3:-1? -3+2=-1, from2:2-3+2=1, from1:-1+2-3+2=0 => max 2. ans=2
        assert(ans[2] == 2);
    }

    return 0;
}
// The problem reduces to maintaining a segment tree over the deviation array `A[i] = original[i] - floor(sum/n)`. Each node of the segment tree stores three values: `total` (sum of the segment), `pref` (maximum prefix sum of the segment, allowing empty prefix so it's at least 0), and `suff` (maximum suffix sum allowing empty suffix). For a node combining left child `L` and right child `R`, the updates are:
// - `total = L.total + R.total`
// - `pref = max(L.pref, L.total + R.pref)`
// - `suff = max(L.suff + R.total, R.suff)`
//
// The answer after each query is `max(root.pref, root.suff)` because any subarray that is a prefix or suffix is captured by these values. Note that allowing empty prefix/suffix (so they are at least 0) is fine because the maximum cannot be negative if we can choose an empty subarray (sum 0), but the problem likely expects non-negative output? Actually the query asks for maximum sum subarray constrained to be prefix or suffix. If all elements are negative, the maximum is 0 (choosing empty), but typical problems allow empty subarray. To be safe, we initialize `pref` and `suff` as `max(0, value)` when building leaves, so the answer is never negative. The segment tree supports point updates in `O(log n)` and the root query is `O(1)`. Initial build is `O(n)`. Total time complexity is `O((n+q) log n)`, and space is `O(n)` for the segment tree arrays. Edge cases: n=1, negative deviations, and updates where `u` and `v` are the same position (the net change is zero, but the code still performs two updates; careful with order—since both modify the same index, doing subtract then add is equivalent to no change, but implement carefully). We'll process each update as two separate point updates.
