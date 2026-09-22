// Given an array of `n` positive integers (`1 ≤ n ≤ 10^5`, each `a[i] ≤ 10^9`), write a C++ function `int maxBalancedPairs(const std::vector<int>& a)` that determines the maximum number `x` such that you can form `x` disjoint pairs, each pair consisting of one "small" element and one "large" element, where for every pair the small element is strictly less than half of the large element (i.e., `small * 2 < large`​ — note: the original snippet uses `<=`? Actually the snippet uses `a[i]*2 > a[n-x+i]` returning false if `small*2 > large`, so valid when `small*2 <= large`. We adapt to that). More precisely, after sorting the array, you must select `x` pairs such that for every `i` from 1 to `x`, the `i`-th smallest selected small element (among the first `x` elements in the sorted order) and the `i`-th largest selected large element (among the last `x` elements) satisfy `small * 2 <= large`. Return the maximum possible `x`. If no pairs can be formed, return 0. The original code reads from stdin; your function should operate directly on the given integer vector.
We first sort the array in non-decreasing order. The key observation is that to maximize the number of valid pairs, we should pair the smallest elements with the largest elements in a symmetric way. Specifically, if we decide to form `x` pairs, the optimal choice is to take the `x` smallest elements (`a[0]` to `a[x-1]` in 0-indexed) as the small halves, and the `x` largest elements (`a[n-x]` to `a[n-1]`) as the large halves. We then pair them in order: the `i`-th smallest small with the `i`-th largest large (i.e., `a[i]` with `a[n-x+i]` in 0‑indexed). This greedy pairing is optimal because if any valid pairing exists for a given `x`, this symmetric ordering also yields a valid pairing – swapping pairs cannot help when both sequences are sorted. The predicate `check(x)` returns true if for all `i` in `[0, x-1]` we have `a[i] * 2 <= a[n-x+i]`. This predicate is monotonic: if `check(x)` is true, then `check(x-1)` is also true (removing the most restrictive pair). Therefore we binary search on `x` from 0 to `n/2`. The lower bound is 0 (always valid), and the upper bound can be `n/2+1` as an exclusive upper bound. For each `mid`, we test the `mid` pairs in `O(mid)` time. The total time complexity is `O(n log n)` due to sorting plus `O(n log n)` for the binary search (since sum of `mid` over all checks is `O(n log n)`). Space complexity is `O(1)` extra (excluding the input vector and sort overhead). Edge cases: when `n < 2`, the maximum `x` is 0; when some `a[i]` is very large, multiplication `a[i]*2` fits in `long long` but the values are up to 1e9 so `int` is sufficient (2×1e9 = 2e9 < 2^31-1), but to be safe use `long long` in the comparison. Also note the original snippet uses `a[i]*2 > a[n-x+i]` returning false, which is equivalent to requiring `a[i]*2 <= a[n-x+i]`.
#include <vector>
#include <algorithm>

// Returns the maximum number of valid disjoint pairs from array a.
// After sorting, for k pairs we must have a[i]*2 <= a[n-k+i] for all i in [0,k-1].
// The predicate is monotonic, so binary search on k.
int maxBalancedPairs(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    if (n < 2) return 0;

    std::vector<int> sorted = a;
    std::sort(sorted.begin(), sorted.end());

    // check if we can form k valid pairs using smallest k and largest k elements.
    auto can_form = [&](int k) -> bool {
        for (int i = 0; i < k; ++i) {
            // Use long long to avoid overflow (though int is safe for <=1e9).
            long long small = sorted[i];
            long long large = sorted[n - k + i];
            if (small * 2 > large) return false;
        }
        return true;
    };

    int lo = 0;              // always valid
    int hi = n / 2 + 1;      // exclusive upper bound, invalid or not needed
    while (lo < hi - 1) {
        int mid = lo + (hi - lo) / 2;
        if (can_form(mid)) lo = mid;
        else hi = mid;
    }
    return lo;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above (in the same translation unit).
// To keep this self-contained, the function definition is included here again.
// In a real separate file, just include the header or copy the function.
int maxBalancedPairs(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    if (n < 2) return 0;
    std::vector<int> sorted = a;
    std::sort(sorted.begin(), sorted.end());
    auto can_form = [&](int k) -> bool {
        for (int i = 0; i < k; ++i) {
            long long small = sorted[i];
            long long large = sorted[n - k + i];
            if (small * 2 > large) return false;
        }
        return true;
    };
    int lo = 0, hi = n / 2 + 1;
    while (lo < hi - 1) {
        int mid = lo + (hi - lo) / 2;
        if (can_form(mid)) lo = mid;
        else hi = mid;
    }
    return lo;
}

int main() {
    // Basic examples
    assert(maxBalancedPairs({1, 2}) == 1);            // 1*2 <= 2
    assert(maxBalancedPairs({1, 3}) == 1);            // 1*2 <= 3
    assert(maxBalancedPairs({1, 2, 3, 4}) == 1);      // (1,4) valid, (2,3) not
    assert(maxBalancedPairs({1, 2, 3, 5}) == 2);      // (1,5) and (2,3) both valid? 2*2<=3 false; actually (1,5) and (2,3) not valid, but (1,3) and (2,5) valid => 2
    assert(maxBalancedPairs({2, 4, 6, 8}) == 2);      // (2,6) and (4,8) both valid
    assert(maxBalancedPairs({1, 2, 4, 4}) == 1);      // only one valid pair
    assert(maxBalancedPairs({5, 5, 5, 5}) == 0);      // 5*2 > 5 always
    assert(maxBalancedPairs({1}) == 0);               // single element
    assert(maxBalancedPairs({}) == 0);                // empty
    // Larger test: n=10, all 1s except last 100
    std::vector<int> v(10, 1);
    v[9] = 100;
    assert(maxBalancedPairs(v) == 1); // only one pair possible? Actually (1,100) valid, but need 2 pairs: (1,1?) smalls are 1,1; larges are 1,100; first pair (1,1) invalid, so 1
    // Another larger: n=6, [1,2,3,4,5,6] -> max? (1,6) and (2,5) and (3,4)? (3,4) invalid -> so 2
    assert(maxBalancedPairs({1,2,3,4,5,6}) == 2);
    return 0;
}
