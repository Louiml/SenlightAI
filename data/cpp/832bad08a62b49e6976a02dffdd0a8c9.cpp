Write a C++ function `maxOverlappingIntervals` that takes a vector of half-open integer intervals `[l, r)` and a list of queries, each query being a power-of-two threshold `w`. For each query, the function must return the maximum number of intervals that overlap at any single integer point after applying the following transformation: first, each interval’s length is computed as `r - l` (with both endpoints 1-indexed in input, but internally 0-indexed). If an interval’s length is at least `w`, then the interval is considered to cover its left endpoint only (i.e., it becomes a point at `l`). Otherwise, the interval remains as the original half-open range `[l, r)`. After applying this transformation to all intervals for the given `w`, compute the maximum overlap count over all integer coordinates (including endpoints). The function should accept the original intervals and a vector of query values (each guaranteed to be a power of two) and return a vector of the same size containing the answers in order.
// The key insight is that the transformation depends only on the highest power-of-two threshold, and we can process all possible powers of two (from `2^0` to `2^29`) in a single sweep using coordinate compression and prefix sums. For each power `k` (i.e., `mod = 1 << k`), we sort all interval endpoints by their value modulo `mod` using a stable merge sort (since endpoints are already sorted by absolute value in a previous step). Then we compress the resulting modular values to dense indices. For each interval, if its original length is at least `mod`, we replace its range with a degenerate range `[l, l]` (since it covers its left endpoint). Then we apply difference-array technique on the compressed coordinates: for each interval `[l, r]` (where `l` may equal `r`), we increment `p[l]` and decrement `p[r]`; for the degenerate case `l == r`, this correctly represents a single point coverage. After building the prefix sums, the maximum value across all positions is the answer for that `k`. For a query value `w`, since `w` is a power of two, `w = 1 << a`, and `a` is the exponent found via `__builtin_ctz(w)`. The preprocessing runs once for all 30 possible exponents, so each query is answered in `O(1)`. Time complexity: `O(n log n * 30)` for the initial sort and each merge + compression + prefix sum, which simplifies to `O(30 * n log n)` worst case. Space complexity: `O(n)` for all auxiliary arrays.
//
// Edge cases: intervals may be empty (l == r after 0-indexing, meaning original l+1 == r), but these are still valid; the transformation for length >= mod turns them into points. Queries are all powers of two, so the exponent is well-defined. Also, note that negative coordinates are not expected; the input intervals are positive integers, and after subtraction of 1 they become non-negative.
#include <bits/stdc++.h>
using namespace std;

// Returns the maximum overlap count for each query threshold, given intervals [l, r) (1-indexed input).
vector<int> maxOverlappingIntervals(const vector<pair<int, int>>& intervals, const vector<int>& queries) {
    const int n = (int)intervals.size();
    // Work with 0-indexed half-open intervals
    vector<pair<int, int>> s = intervals;
    for (auto& [l, r] : s) --l;  // now l is 0-indexed, r stays as exclusive endpoint (1-indexed original r)
    
    vector<int> len(n);
    for (int i = 0; i < n; ++i) len[i] = s[i].second - s[i].first;
    
    // Values list for coordinate compression
    vector<int> allVals;
    allVals.reserve(2 * n);
    for (const auto& [l, r] : s) {
        allVals.push_back(l);
        allVals.push_back(r);
    }
    
    // We'll sort values and keep pointers to original positions
    vector<pair<int, int*>> vals; // value, pointer to original location
    for (int i = 0; i < n; ++i) {
        vals.emplace_back(s[i].first, &s[i].first);
        vals.emplace_back(s[i].second, &s[i].second);
    }
    sort(vals.begin(), vals.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
    
    vector<int> ans(30, 0);
    const int maxK = 29; // since we handle powers up to 2^29
    
    for (int k = maxK; k >= 0; --k) {
        const int mod = 1 << k;
        vector<pair<int, int*>> l, r;
        l.reserve(2 * n);
        r.reserve(2 * n);
        for (const auto& [x, p] : vals) {
            if (x < mod) l.emplace_back(x % mod, p);
            else r.emplace_back(x % mod, p);
        }
        // Merge the two parts by modular value to maintain stable order
        vector<pair<int, int*>> merged;
        merged.reserve(2 * n);
        merge(l.begin(), l.end(), r.begin(), r.end(), back_inserter(merged), 
              [](const auto& a, const auto& b) { return a.first < b.first; });
        vals = move(merged);
        
        // Coordinate compress modular values
        int cur = -1;
        int prev = -1;
        for (int i = 0; i < (int)vals.size(); ++i) {
            if (i == 0 || vals[i].first != prev) {
                ++cur;
                prev = vals[i].first;
            }
            *vals[i].second = cur;
        }
        int m = cur + 1; // number of distinct coordinates
        
        // Apply transformation: if length >= mod, collapse to point at left endpoint
        for (int i = 0; i < n; ++i) {
            if (len[i] >= mod) {
                s[i].second = s[i].first; // now [l, l) is a point
            }
        }
        
        // Difference array
        vector<int> p(m + 1, 0);
        for (const auto& [l, r] : s) {
            if (l < r) {
                p[l]++;
                p[r]--;
            } else { // degenerate point
                p[l]++;
                p[m]--;
                p[0]++;
                p[r]--; // r == l, so p[l]-- cancels the +1, but the wrap-around covers it
            }
        }
        for (int i = 1; i <= m; ++i) p[i] += p[i - 1];
        for (int i = 0; i < m; ++i) ans[k] = max(ans[k], p[i]);
    }
    
    // Answer each query
    vector<int> result;
    result.reserve(queries.size());
    for (int w : queries) {
        int a = __builtin_ctz(w); // since w is power of two
        result.push_back(ans[a]);
    }
    return result;
}
#include <bits/stdc++.h>
using namespace std;

// Include the solution function here (or link)

int main() {
    // Example 1: simple intervals
    vector<pair<int, int>> intervals1 = {{1, 3}, {2, 4}, {3, 5}};
    vector<int> queries1 = {1, 2, 4, 8};
    vector<int> res1 = maxOverlappingIntervals(intervals1, queries1);
    // For w=1: lengths 2,2,2 all >=1, so each becomes point at 1,2,3 => max overlap 1
    // For w=2: lengths 2,2,2 all >=2, same => 1
    // For w=4: lengths 2,2,2 all <4, original ranges [1,3),[2,4),[3,5) => overlap at 3 => 2
    // For w=8: same as w=4 => 2
    assert((res1 == vector<int>{1, 1, 2, 2}));

    // Example 2: one long interval and one short
    vector<pair<int, int>> intervals2 = {{1, 10}, {2, 3}};
    vector<int> queries2 = {1, 2, 4, 8, 16};
    vector<int> res2 = maxOverlappingIntervals(intervals2, queries2);
    // w=1: both lengths 9 and 1 >=1 => both points at 1 and 2 => max overlap 1
    // w=2: len1=9>=2 point at 0, len2=1<2 stays [1,2) => overlap at 1? point at 0 no, so max 1
    // w=4: len1=9>=4 point at 0, len2=1<4 stays [1,2) => overlap? no => 1
    // w=8: len1=9>=8 point at 0, len2=1<8 stays [1,2) => 1
    // w=16: len1=9<16 stays [0,9), len2=1<16 stays [1,2) => overlap at 1..2 => 2
    assert((res2 == vector<int>{1, 1, 1, 1, 2}));

    // Example 3: all same length 1
    vector<pair<int, int>> intervals3 = {{1, 2}, {2, 3}, {3, 4}};
    vector<int> queries3 = {1, 2};
    vector<int> res3 = maxOverlappingIntervals(intervals3, queries3);
    // w=1: all lengths 1 >=1 => points at 1,2,3 => max 1
    // w=2: all lengths 1 <2 => original [1,2),[2,3),[3,4) => no overlap (half-open) => max 1
    assert((res3 == vector<int>{1, 1}));

    // Example 4: nested intervals
    vector<pair<int, int>> intervals4 = {{1, 5}, {2, 4}, {3, 6}};
    vector<int> queries4 = {1, 4};
    vector<int> res4 = maxOverlappingIntervals(intervals4, queries4);
    // w=1: lengths 4,2,3 all >=1 => points at 1,2,3 => max 1
    // w=4: len1=4>=4 point at 1, len2=2<4 stays [1,3), len3=3<4 stays [2,5) => overlap between len2 and len3 at [2,3) => 2
    assert((res4 == vector<int>{1, 2}));

    // Example 5: single interval
    vector<pair<int, int>> intervals5 = {{5, 10}};
    vector<int> queries5 = {1, 2, 4, 8, 16};
    vector<int> res5 = maxOverlappingIntervals(intervals5, queries5);
    // All cases: max overlap is 1 (either point or range)
    assert((res5 == vector<int>{1, 1, 1, 1, 1}));

    return 0;
}
