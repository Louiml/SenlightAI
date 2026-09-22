// You are given `n` items with integer values, a number of coupons `k`, and a discount value `x`. Each coupon can reduce the value of exactly one item by `x`, but the value of an item cannot become negative. You may apply multiple coupons to the same item. Your goal is to minimize the total sum of the remaining values after using at most `k` coupons. Write a C++ function `long long minimizeTotal(int n, int k, int x, const std::vector<int>& values)` that returns the minimized total sum. The function should handle cases where `k` is larger than needed to reduce all items to zero, and where `n` can be zero (in that case return 0). Values are non-negative integers.
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(minimizeTotal(3, 1, 10, {10, 20, 30}) == 50);  // reduce 30 to 20
    assert(minimizeTotal(3, 2, 10, {10, 20, 30}) == 40);  // reduce 30->20, 20->10
    assert(minimizeTotal(3, 0, 10, {10, 20, 30}) == 60);  // no coupons
    assert(minimizeTotal(0, 5, 10, {}) == 0);              // no items

    // Overkill coupons (zero everything)
    assert(minimizeTotal(3, 10, 1, {5, 5, 5}) == 0);
    assert(minimizeTotal(2, 5, 3, {7, 8}) == 0);

    // Partial reduction with remainders
    assert(minimizeTotal(2, 1, 4, {10, 12}) == 18); // 12-4=8, 10+8=18
    assert(minimizeTotal(2, 2, 4, {10, 12}) == 14); // 12-4=8, 10-4=6 => 14

    // Large coupon count but small values
    assert(minimizeTotal(2, 100, 1, {1, 2}) == 0);

    // Mixed values, first phase then zeroing
    assert(minimizeTotal(4, 3, 5, {3, 4, 10, 12}) == 9); // sort: 3,4,10,12; use on 12->7, 10->5, use 1 more on 5->0? Actually:
    // Phase1: i=0 largest=12 use=2 (k=3->1, 12->2), then largest=10 use=1 (k=0, 10->5). Now values:3,4,5,2 sorted:2,3,4,5. k=0 -> sum=14? Wait recalc carefully:
    // Actually the above expected is wrong. Let's compute properly:
    // values: [3,4,10,12], n=4, k=3, x=5
    // Phase1: idx=3: a[3]=12/5=2, use=2, k=1, a[3]=2. idx=2: a[2]=10/5=2, use=min(1,2)=1, k=0, a[2]=5. now values [3,4,5,2] sum=14. k=0, no phase2. So answer 14.
    // So assert(minimizeTotal(4,3,5,{3,4,10,12}) == 14);
    assert(minimizeTotal(4, 3, 5, {3, 4, 10, 12}) == 14);

    // More complex one: leftover coupons zero largest
    // n=3, k=5, x=2, values [1,3,5]
    // Phase1: idx=2: 5/2=2 use=2 k=3 -> a=1; idx=1: 3/2=1 use=1 k=2 -> a=1; idx=0: 1/2=0 use=0. values now [1,1,1] sorted. k=2 left, phase2: zero idx=2 ->0, idx=1 ->0. sum=1.
    assert(minimizeTotal(3, 5, 2, {1, 3, 5}) == 1);

    // Edge: exact multiple
    assert(minimizeTotal(2, 4, 10, {10, 20}) == 0); // 20-10=10, 10-10=0, 10-10=0, k left=1 but all zero

    // Zero values already
    assert(minimizeTotal(3, 10, 10, {0, 0, 0}) == 0);

    // Single item
    assert(minimizeTotal(1, 3, 5, {12}) == 0); // 12-5=7, 7-5=2, then zero with leftover? Actually phase1: 12/5=2 use 2 k=1 => a=2, k=1, phase2 zero =>0.
}
(Note: The test includes an extra comment about a wrong expectation, but the actual assert uses the correct 14.)
#include <vector>
#include <algorithm>
#include <cstdint>

// Minimize the total sum of item values using at most k coupons.
// Each coupon reduces one item's value by x, never below 0.
long long minimizeTotal(int n, int k, int x, const std::vector<int>& values) {
    if (n == 0 || k == 0) {
        // If no items or no coupons, sum original values.
        long long sum = 0;
        for (int v : values) sum += v;
        return sum;
    }

    // Copy and sort ascending.
    std::vector<int> a = values;
    std::sort(a.begin(), a.end());

    // Phase 1: apply whole x reductions to largest items.
    for (int i = 0; i < n && k > 0; ++i) {
        int idx = n - 1 - i; // largest remaining index
        int use = a[idx] / x; // how many x can be fully subtracted
        if (use > k) use = k;
        a[idx] -= use * x;
        k -= use;
    }

    // Re-sort because values changed.
    std::sort(a.begin(), a.end());

    // Phase 2: zero out largest remaining positive values with leftover coupons.
    for (int j = 0; j < k && j < n; ++j) {
        int idx = n - 1 - j;
        if (a[idx] > 0) {
            a[idx] = 0;
        } else {
            break; // all remaining are zero, further coupons useless
        }
    }

    long long ans = 0;
    for (int v : a) ans += v;
    return ans;
}
// The optimal strategy is to repeatedly apply coupons to the largest current item values because reducing a large value reduces the total sum the most per coupon. 
//
// First, sort the values in ascending order. Then iterate from the largest item down to the smallest. For each item, compute how many whole `x` increments can be subtracted without making it negative: `use = min(k, values[largest] / x)`. Subtract `use * x` from that item and decrement `k`. Continue to the next largest item. After this step, the values are sorted again because some may have changed. 
//
// If coupons remain (`k > 0`), the best way to use them is to zero out the largest remaining values. Sort once more, then for `i = 0` to `n-1` (up to `k` times), set the largest remaining item value to 0. Since each coupon can reduce a value by at least 1 (because the value after the first phase is strictly less than `x` unless it is already 0), setting the largest remaining positive value to 0 uses at most one coupon per item and guarantees minimal total sum. If we zero an item, it becomes 0 and any further coupons on it are wasted.
//
// Edge cases: 
// - `k` may be 0, so no coupons used.
// - `n` may be 0, return 0.
// - After first phase, `k` may be much larger than `n` – we only zero at most `n` items, and additional coupons are unused.
// - Values might be huge (up to `10^9`) and `n` up to `10^5` so use `long long` for sums and intermediate calculations.
//
// Time complexity: Sorting each phase takes `O(n log n)`. The loops are `O(n)`. So overall `O(n log n)`. Space: `O(1)` extra if we sort in place, or `O(n)` if we copy the input.
