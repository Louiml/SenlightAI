Write a C++ function `long long minimumAbsoluteDifference(long long n, long long m, const std::vector<long long>& a, const std::vector<long long>& b, long long q, const std::vector<std::tuple<long long, long long, long long>>& queries)` that computes the following:  
You are given two arrays `a` (length `n`) and `b` (length `m`, where `m >= n`). Define an alternating sum of an array `x[1..k]` as `x[1] - x[2] + x[3] - x[4] + ...` (i.e., odd indices are added, even indices are subtracted).  
Consider all contiguous subarrays of `b` of length exactly `n`. For each such subarray, compute the value `V = (alternating sum of the subarray of b) - (alternating sum of a)`.  
Over all such subarrays, store these `V` values in a multiset-like structure (you may use `std::set` with duplicates).  
Initially, output the minimum absolute value of any integer `x` such that `x` equals `V` (this is simply the minimum absolute value among all `V` values), which is the answer for the initial state. Then, there are `q` updates: each update gives a range `[li, ri]` (1-indexed) and an increment `xi` that is added to every element of `a` in that range (i.e., `a[li] += xi, a[li+1] += xi, ..., a[ri] += xi`). After each update, output the minimum absolute value among all `V` values (where `V` is recomputed with the updated `a`). The function should return a vector of `q+1` answers, where the first element is the initial answer, and the `(i+1)`-th element is the answer after the `i`-th update (in order).  
Constraints: `1 <= n <= m <= 100000`, `1 <= q <= 100000`, array elements and `xi` fit in `long long` (absolute value up to `10^9`), and the answers fit in `long long`.  
The solution must be efficient: an `O((m + q) log m)` time algorithm is expected, and the space complexity should be `O(m)`.
#include <cassert>
#include <vector>
#include <tuple>

// The solution function is declared in the same translation unit, but for testing we re-declare it.
std::vector<long long> minimumAbsoluteDifference(long long, long long, const std::vector<long long>&, const std::vector<long long>&, long long, const std::vector<std::tuple<long long, long long, long long>>&);

int main() {
    // Helper to convert initializer list to vector
    auto vec = [](std::initializer_list<long long> l) { return std::vector<long long>(l); };

    // Test 1: single subarray, simple
    {
        std::vector<long long> a = {1, 2, 3};
        std::vector<long long> b = {5, 4, 3, 2, 1};
        // n=3, m=5. a alt sum = 1-2+3=2.
        // Subarrays of b length 3:
        // [5,4,3] alt = 5-4+3=4 -> V=2
        // [4,3,2] alt = 4-3+2=3 -> V=1
        // [3,2,1] alt = 3-2+1=2 -> V=0
        // min |V| = 0.
        std::vector<std::tuple<long long, long long, long long>> queries;
        auto ans = minimumAbsoluteDifference(3, 5, a, b, 0, queries);
        assert(ans.size() == 1);
        assert(ans[0] == 0);
    }

    // Test 2: with update that changes sa by zero
    {
        std::vector<long long> a = {10, 20, 30};
        std::vector<long long> b = {1, 2, 3, 4, 5};
        // n=3, m=5. a alt sum = 10-20+30=20.
        // b subarrays alt: [1,2,3] = 1-2+3=2 -> V=-18; [2,3,4]=2-3+4=3 -> V=-17; [3,4,5]=3-4+5=4 -> V=-16.
        // min |V| = 16.
        std::vector<std::tuple<long long, long long, long long>> queries = {
            {1, 2, 5} // even length 2, delta=0, no change
        };
        auto ans = minimumAbsoluteDifference(3, 5, a, b, 1, queries);
        assert(ans.size() == 2);
        assert(ans[0] == 16);
        assert(ans[1] == 16);
    }

    // Test 3: with update that changes sa by +xi
    {
        std::vector<long long> a = {1, 1, 1};
        std::vector<long long> b = {3, 4, 5, 6};
        // n=3, m=4. a alt sum = 1-1+1=1.
        // b subarrays: [3,4,5] alt=3-4+5=4 -> V=3; [4,5,6] alt=4-5+6=5 -> V=4.
        // min |V| = 3.
        std::vector<std::tuple<long long, long long, long long>> queries = {
            {1, 1, 2} // odd length 1, li=1 odd -> delta=+2, so sa becomes 3, V become 1 and 2, min=1
        };
        auto ans = minimumAbsoluteDifference(3, 4, a, b, 1, queries);
        assert(ans[0] == 3);
        assert(ans[1] == 1);
    }

    // Test 4: with update that changes sa by -xi
    {
        std::vector<long long> a = {5, 5, 5};
        std::vector<long long> b = {1, 2, 3, 4, 5};
        // n=3, m=5. a alt sum = 5-5+5=5.
        // subarrays alt: [1,2,3]=2 -> V=-3; [2,3,4]=3 -> V=-2; [3,4,5]=4 -> V=-1. min=1.
        std::vector<std::tuple<long long, long long, long long>> queries = {
            {2, 2, 1} // li=2 even, len odd -> delta=-1, sa becomes 4, V become -2,-1,0 -> min=0
        };
        auto ans = minimumAbsoluteDifference(3, 5, a, b, 1, queries);
        assert(ans[0] == 1);
        assert(ans[1] == 0);
    }

    // Test 5: n == m, only one subarray, update shifts it
    {
        std::vector<long long> a = {2, 3};
        std::vector<long long> b = {7, 8};
        // n=2, m=2. a alt = 2-3=-1. subarray alt = 7-8=-1. V=0. min=0.
        std::vector<std::tuple<long long, long long, long long>> queries = {
            {1, 2, 3} // len even -> delta=0, no change
        };
        auto ans = minimumAbsoluteDifference(2, 2, a, b, 1, queries);
        assert(ans[0] == 0);
        assert(ans[1] == 0);
    }

    // Test 6: multiple queries, check order
    {
        std::vector<long long> a = {0, 0, 0};
        std::vector<long long> b = {10, 20, 30, 40, 50};
        // n=3, m=5. a alt=0.
        // subarrays alt: [10,20,30]=10-20+30=20; [20,30,40]=20-30+40=30; [30,40,50]=30-40+50=40.
        // V = 20,30,40. min=20.
        std::vector<std::tuple<long long, long long, long long>> queries = {
            {2, 4, 5},  // len=3 odd, li=2 even -> delta=-5, sa= -5? Actually sa = alternating sum of a after update.
                        // Update adds 5 to a[2..4] -> a becomes [0,5,5]? Actually a[2] and a[3] and a[4]? n=3 so indices 1,2,3. a[2] and a[3] get +5 -> a=[0,5,5] -> alt=0-5+5=0? Wait li=2,ri=4 but n=3 so only positions 2,3 exist in a. That's out of range? The problem statement says range [li,ri] is indices in a, and must be within 1..n. So li=2, ri=4 is invalid for n=3. We must ensure valid inputs. So I'll adjust test to li=1,ri=3.
            {1, 3, 2}   // len=3 odd, li=1 odd -> delta=2, sa becomes 2? After first update we'll have sa=0? Let's just compute directly: start a=[0,0,0] alt=0.
                        // Query1: li=1,ri=3,xi=2 -> len=3 odd, li odd -> delta=+2, sa becomes 2, so V become 20-2=18,30-2=28,40-2=38 -> min=18.
                        // Query2: li=1,ri=2,xi=1 -> len=2 even -> delta=0, sa remains 2, so V still 18,28,38 -> min=18.
        };
        auto ans = minimumAbsoluteDifference(3, 5, a, b, 2, queries);
        assert(ans.size() == 3);
        assert(ans[0] == 20);
        assert(ans[1] == 18);
        assert(ans[2] == 18);
    }

    // Test 7: negative values
    {
        std::vector<long long> a = {-1, 2, -3};
        std::vector<long long> b = {1, -2, 3, -4, 5};
        // a alt = -1 -2 -3? Wait -1 - (+2?) Actually -1 - 2 + (-3)? Let's compute: alt = a1 - a2 + a3 = -1 - 2 + (-3) = -6.
        // b subarrays alt:
        // [1,-2,3] = 1 - (-2) + 3 = 6 -> V=12
        // [-2,3,-4] = -2 - 3 + (-4) = -9 -> V=-3
        // [3,-4,5] = 3 - (-4) + 5 = 12 -> V=18
        // min |V| = 3 (from -3).
        std::vector<std::tuple<long long, long long, long long>> queries;
        auto ans = minimumAbsoluteDifference(3, 5, a, b, 0, queries);
        assert(ans[0] == 3);
    }

    // Test 8: duplicates in V
    {
        std::vector<long long> a = {1, 2, 1};
        std::vector<long long> b = {1, 2, 3, 1, 2};
        // a alt = 1-2+1=0.
        // subarrays alt: [1,2,3]=1-2+3=2; [2,3,1]=2-3+1=0; [3,1,2]=3-1+2=4? Wait indices: [3,1,2] alt=3-1+2=4. So V=2,0,4 -> min=0.
        std::vector<std::tuple<long long, long long, long long>> queries;
        auto ans = minimumAbsoluteDifference(3, 5, a, b, 0, queries);
        assert(ans[0] == 0);
    }

    // Test 9: large values, simple check
    {
        std::vector<long long> a = {1000000000, 1000000000};
        std::vector<long long> b = {2000000000, 1000000000, 2000000000};
        // n=2, m=3. a alt = 1000000000 - 1000000000 = 0.
        // subarrays length2: [2000000000,1000000000] alt = 2000000000-1000000000=1000000000 -> V=1e9
        // [1000000000,2000000000] alt = 1000000000-2000000000=-1e9 -> V=-1e9
        // min=1e9.
        std::vector<std::tuple<long long, long long, long long>> queries;
        auto ans = minimumAbsoluteDifference(2, 3, a, b, 0, queries);
        assert(ans[0] == 1000000000);
    }

    // Test 10: multiple updates, shifting set
    {
        std::vector<long long> a = {0, 0};
        std::vector<long long> b = {1, 2, 3, 4, 5};
        // n=2, m=5. a alt=0.
        // subarrays length2 alt: [1,2]=1-2=-1; [2,3]=2-3=-1; [3,4]=3-4=-1; [4,5]=4-5=-1 -> all V=-1, min=1.
        std::vector<std::tuple<long long, long long, long long>> queries = {
            {1, 1, 1},  // len1 odd, li odd -> delta=+1, sa=1, V become -2, min=2
            {2, 2, 3},  // len1 odd, li even -> delta=-3, sa=1-3=-2, V become 1, min=1
            {1, 2, 2}   // len2 even -> delta=0, sa stays -2, V=1, min=1
        };
        auto ans = minimumAbsoluteDifference(2, 5, a, b, 3, queries);
        assert(ans.size() == 4);
        assert(ans[0] == 1);
        assert(ans[1] == 2);
        assert(ans[2] == 1);
        assert(ans[3] == 1);
    }

    return 0;
}
#include <vector>
#include <set>
#include <tuple>
#include <cstdint>
#include <algorithm>

/**
 * Computes minimum absolute difference between all possible V values and a global shift.
 * 
 * Given arrays a (length n) and b (length m >= n), define the alternating sum of an array
 * x[1..k] as x[1] - x[2] + x[3] - ... (odd indices plus, even minus).
 * For every contiguous subarray of b of length n, let V = (alternating sum of that subarray)
 * - (alternating sum of a). Initially output min |V|. Then process updates: each update adds
 * xi to a[li..ri] (1-indexed). After each update, output min |V| with updated a.
 *
 * @param n length of a
 * @param m length of b
 * @param a input array a (1-indexed by convention, but passed as vector with a[0] = a_1, ...)
 * @param b input array b
 * @param q number of updates
 * @param queries vector of (li, ri, xi) 1-indexed updates
 * @return vector of q+1 answers: initial answer then answers after each update in order.
 */
std::vector<long long> minimumAbsoluteDifference(
    long long n,
    long long m,
    const std::vector<long long>& a,
    const std::vector<long long>& b,
    long long q,
    const std::vector<std::tuple<long long, long long, long long>>& queries)
{
    // Alternating sum of a: a[1] - a[2] + a[3] - ...
    long long sa = 0;
    for (long long i = 0; i < n; ++i) {
        if ((i + 1) & 1) sa += a[i]; // 1-indexed odd position
        else             sa -= a[i];
    }

    // Build transformed c[i] = b[i] * sign(i), sign(i) = +1 if i odd, -1 if even (1-indexed)
    // and prefix sums of c.
    std::vector<long long> prefC(m + 1, 0);
    for (long long i = 1; i <= m; ++i) {
        long long val = b[i - 1];
        if (!(i & 1)) val = -val; // sign flip for even indices
        prefC[i] = prefC[i - 1] + val;
    }

    // Insert all original V values into a multiset.
    std::multiset<long long> vals;
    for (long long s = 1; s <= m - n + 1; ++s) {
        long long subB = prefC[s + n - 1] - prefC[s - 1]; // alternating sum of b[s..s+n-1]
        vals.insert(subB - sa);
    }

    // Helper: given shift dlt, return min |x + dlt| for x in vals.
    auto closest = [&](long long dlt) -> long long {
        long long target = -dlt; // we want min |x - target|
        auto it = vals.lower_bound(target);
        long long best = 1000000000000000000LL;
        if (it != vals.end()) {
            best = std::min(best, *it - target);
        }
        if (it != vals.begin()) {
            --it;
            best = std::min(best, target - *it);
        }
        return best;
    };

    std::vector<long long> answers;
    answers.push_back(closest(0));

    long long dlt = 0;
    for (const auto& t : queries) {
        long long li, ri, xi;
        std::tie(li, ri, xi) = t;
        long long len = ri - li + 1;
        long long delta = 0;
        if (len & 1) { // odd length: one more of the starting parity
            if (li & 1) delta = xi;
            else        delta = -xi;
        }
        dlt -= delta; // since sa increases by delta, V decreases by delta
        answers.push_back(closest(dlt));
    }

    return answers;
}
// The key insight is to interpret the alternating sum of a subarray of `b` cleverly.  
// For a subarray of `b` starting at index `s` and ending at `s+n-1`, its alternating sum is `b[s] - b[s+1] + b[s+2] - ...` (with sign pattern depending on the starting index's parity).  
// We can precompute a transformed version of `b` to make all subarray alternating sums equal to a simple sliding-window expression. Define a transformed array `c[i] = B[i]` where for each `i`, we flip the sign of `b[i]` if `i` is even: specifically, let `c[i] = b[i] * sign(i)`, where `sign(i) = +1` if `i` is odd, `-1` if `i` is even (using 1-indexing). Then the alternating sum of any subarray of `b` from `s` to `s+n-1` is exactly `(sign(s) * c[s])`? Let's check: for `s` odd, the alternating sum is `b[s] - b[s+1] + b[s+2] - ...`. In terms of `c`, `c[s] = b[s]`, `c[s+1] = -b[s+1]`, `c[s+2] = b[s+2]`, ... So the alternating sum equals `c[s] + c[s+1] + c[s+2] + ... + c[s+n-1]`. For `s` even, alternating sum is `-b[s] + b[s+1] - b[s+2] + ...`. Here `c[s] = -b[s]`, `c[s+1] = b[s+1]`, `c[s+2] = -b[s+2]`, ... so the sum of `c` from `s` to `s+n-1` also equals the alternating sum. Thus, for every starting index `s`, the alternating sum of `b[s..s+n-1]` equals `prefixC[s+n-1] - prefixC[s-1]`, where `prefixC[i]` is the prefix sum of `c`.  
// Now define `sa = alternating sum of a`. For each starting index `s` (from 1 to `m-n+1`), the value `V_s = (prefixC[s+n-1] - prefixC[s-1]) - sa`. We need to maintain the set of all `V_s` under updates to `a`.  
// Updates: adding `xi` to `a[li..ri]` changes `sa` by `delta = xi * (sum of signs over that range)`, where the sign for position `i` in the alternating sum of `a` is `+1` if `i` odd, `-1` if even. The change to `sa` is `delta = xi * ((number of odd indices in [li,ri]) - (number of even indices in [li,ri]))`. This depends only on parity of `li` and `ri` and the length. Specifically, if `(ri-li+1)` is even, then the number of odds equals evens, so `delta = 0`. If the length is odd, then the parity of `li` decides: if `li` is odd, there is one more odd than even, so `delta = +xi`; if `li` is even, `delta = -xi`.  
// Thus, every update adds either `+xi`, `-xi`, or `0` to `sa`. Since all `V_s` are of the form `(constant_s) - sa`, when `sa` changes by `delta`, all `V_s` shift by `-delta`. So we can maintain a global shift `dlt` such that the actual `V_s` = `originalV_s + dlt`. Initially `dlt = 0`. After each update, `dlt -= delta` (because `sa` increases by `delta`, so `V_s` decreases by `delta`).  
// Now the problem reduces to: we have a multiset of numbers `originalV_s` (computed from initial `a`), and we need to answer queries: given a shift `dlt`, find the minimum absolute value among `(x + dlt)` for all `x` in the multiset. This is a standard problem: we can store the original values in two `std::set`s (or one `std::multiset`? but we need to handle duplicates and min absolute value). Since we only need the minimum absolute value, we can simply keep all original values in one `std::multiset<long long>`, and for a given `dlt`, we find the lower_bound of `-dlt` (or `0`? Actually we need min over `|x + dlt|` = min over `|x - (-dlt)|`, so we want the closest value to `-dlt`). So we query the multiset for the closest element to `-dlt`: check iterator at `lower_bound(-dlt)` and the previous one, take the minimum distance.  
// Thus, the algorithm:
// 1. Compute alternating sum of `a`: `sa`.
// 2. Transform `b` to `c` where `c[i] = b[i] * (i%2 ? +1 : -1)` (1-indexed). Build prefix sums `prefC`.
// 3. For each `s` from 1 to `m-n+1`, compute `originalV = prefC[s+n-1] - prefC[s-1] - sa`, insert into a `std::multiset<long long> vals`.
// 4. The initial answer is the closest to 0 in `vals`.
// 5. For each update `(li, ri, xi)`: compute `delta` as described (0 if length even, else `xi` if `li` odd, else `-xi`). Update `dlt -= delta`. Then answer is closest to `-dlt` in `vals`.
// 6. Return the vector of answers.
//
// Edge cases: `n` can equal `m`, so there is exactly one subarray. The multiset may have duplicates—that's fine. The `delta` calculation must use 1-indexed positions. Since all values can be large, use `long long`.  
// Time complexity: `O(m)` to build the multiset (insert `m-n+1` elements), each query is `O(log m)` for `lower_bound`, total `O((m+q) log m)`, and space `O(m)` for the multiset and prefix arrays.
