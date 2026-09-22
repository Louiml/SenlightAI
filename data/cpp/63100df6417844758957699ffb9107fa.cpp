// Write a C++ function `std::vector<int> restorePermutation(const std::vector<int>& b)` that takes a vector `b` of length `n` (1-indexed conceptually, but 0-indexed in code) where `b[i]` satisfies `b[i] = floor((i+1) / a[i])` for some unknown permutation `a` of integers `1..n`. The function must find and return the original permutation `a` as a 0-indexed vector of length `n`. It is guaranteed that a valid permutation exists. For each position `i` (0-indexed in the function, but think of position `p = i+1`), the valid values `a[i]` must satisfy:
// - from `b[i] * a[i] <= p` we get `a[i] <= floor(p / b[i])` if `b[i] > 0`, else `a[i] <= n` (any value).
// - from `p < (b[i]+1) * a[i]` we get `a[i] >= floor(p / (b[i]+1)) + 1`, with careful integer rounding.
// So each position has a contiguous allowed interval `[L_i, R_i]`. The problem reduces to assigning each position a distinct integer from `1..n` within its interval. Return the permutation. If no assignment exists, the behavior is undefined (but input guarantees existence). The input size `n` can be up to `5*10^5`, so an `O(n log n)` greedy with a sorted set is expected.

// For each index `p = i+1` (1-indexed) we derive its allowed range:
// - `R = (b[i] == 0) ? n : (p / b[i])` (integer division floor).
// - `L = (p + b[i]) / (b[i] + 1)` (integer division floor) after rearranging: `p < (b[i]+1)*a` ⇒ `a > p/(b[i]+1) - 1` ⇒ smallest integer `a` satisfying is `floor(p/(b[i]+1)) + 1`, which equals `(p + b[i]) / (b[i]+1)` when using integer arithmetic. For example: `(p + k) / (k+1)` works for `k = b[i]`.
// - We must also cap `L` at `1` and `R` at `n`, but the formulas already do that (for `b=0`, `L = (p+0)/1 = p`, `R = n`; for positive `b`, `L ≥ 1` and `R ≤ n` given valid input).
// - We have `n` intervals `[L_i, R_i]` for positions `i` (0-indexed). We need to assign each interval a distinct integer from `1..n` within its interval.
// - Classic greedy: sort intervals by right endpoint `R` ascending. Use a `std::set<int>` initially containing all numbers `1..n`. For each interval in sorted order, find the smallest available number `>= L` using `lower_bound(L)`. If that number is `<= R`, assign it and remove it from the set. Since the problem guarantees a solution, this will always succeed.
// - Edge cases: `b[i] = 0` means the position can be any number (but constrained by other positions). The greedy still works. Also, the input is guaranteed valid, so we don't need to handle failure, but we can assert.
// - Complexity: Sorting `O(n log n)`, each interval does one `lower_bound` and one `erase` on a `set`, each `O(log n)`, so total `O(n log n)` time and `O(n)` auxiliary space (for the set and output). The original code uses a similar approach.

#include <vector>
#include <set>
#include <algorithm>
#include <cassert>

struct Interval {
    int left;
    int right;
    int idx; // original index
};

// Given b[i] = floor((i+1) / a[i]) for some permutation a of 1..n,
// reconstruct and return that permutation a (0-indexed vector of length n).
std::vector<int> restorePermutation(const std::vector<int>& b) {
    const int n = static_cast<int>(b.size());
    std::vector<Interval> intervals;
    intervals.reserve(n);
    for (int i = 0; i < n; ++i) {
        const int p = i + 1; // 1-indexed position
        int right;
        if (b[i] == 0) {
            right = n; // any value allowed
        } else {
            right = p / b[i];
        }
        // Derive left bound: p < (b[i]+1)*a  => a > p/(b[i]+1) - 1
        // Smallest integer a satisfying is floor(p/(b[i]+1)) + 1
        int left = p / (b[i] + 1) + 1;
        // Clamp for safety (should already be within [1,n] for valid input)
        if (left < 1) left = 1;
        if (right > n) right = n;
        intervals.push_back({left, right, i});
    }

    // Sort by right endpoint ascending
    std::sort(intervals.begin(), intervals.end(),
              [](const Interval& a, const Interval& b) {
                  return a.right < b.right;
              });

    std::set<int> available;
    for (int v = 1; v <= n; ++v) {
        available.insert(v);
    }

    std::vector<int> result(n, -1);
    for (const auto& itv : intervals) {
        auto it = available.lower_bound(itv.left);
        // Since input is guaranteed valid, this must succeed
        assert(it != available.end());
        assert(*it <= itv.right);
        result[itv.idx] = *it;
        available.erase(it);
    }
    assert(available.empty());
    return result;
}

#include <cassert>
#include <vector>

// Declaration of the function under test (provided in the solution section)
std::vector<int> restorePermutation(const std::vector<int>& b);

// Helper to check that b[i] == floor((i+1) / a[i])
bool check(const std::vector<int>& b, const std::vector<int>& a) {
    int n = (int)b.size();
    for (int i = 0; i < n; ++i) {
        int p = i + 1;
        if (b[i] != p / a[i]) return false;
    }
    // Check a is a permutation of 1..n
    std::vector<int> seen(n+1, 0);
    for (int x : a) {
        if (x < 1 || x > n || seen[x]) return false;
        seen[x] = 1;
    }
    return true;
}

int main() {
    // Test 1: Example from the snippet with n=5 and b values
    assert(check({0, 0, 0, 0, 0}, restorePermutation({0, 0, 0, 0, 0})));
    assert(check({1, 1, 1, 1, 1}, restorePermutation({1, 1, 1, 1, 1})));
    
    // Test 2: n=7, a known permutation
    std::vector<int> b2 = {3, 1, 0, 1, 0, 0, 0};
    std::vector<int> a2 = restorePermutation(b2);
    assert(check(b2, a2));

    // Test 3: n=1, b={1}
    assert(restorePermutation({1}) == std::vector<int>{1});

    // Test 4: n=4, specific known result
    std::vector<int> b4 = {2, 0, 0, 0};
    std::vector<int> a4 = restorePermutation(b4);
    assert(check(b4, a4));

    // Test 5: n=3, b={0,1,1}
    std::vector<int> b5 = {0, 1, 1};
    std::vector<int> a5 = restorePermutation(b5);
    assert(check(b5, a5));

    // Test 6: Larger random-like n=100, verify
    for (int n : {100, 200, 500}) {
        std::vector<int> a(n);
        for (int i = 0; i < n; ++i) a[i] = i+1;
        // Shuffle deterministically
        for (int i = n-1; i>0; --i) {
            int j = (i*37 + 11) % (i+1);
            std::swap(a[i], a[j]);
        }
        std::vector<int> b(n);
        for (int i = 0; i < n; ++i) {
            int p = i+1;
            b[i] = p / a[i];
        }
        assert(check(b, restorePermutation(b)));
    }

    return 0;
}
