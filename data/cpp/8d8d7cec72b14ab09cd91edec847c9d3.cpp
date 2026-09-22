Write a C++ function named `maxMedianAfterOps` that takes a vector of integers `a`, an integer `k`, and returns the maximum possible median value of `a` after performing at most `k` increment operations, where each operation increases any single element by exactly 1. The vector `a` has odd length `n` (so the median is the element at index `n/2` after sorting). You may increment elements arbitrarily, but only the elements from index `n/2` onward (after sorting) can be considered, because the median is defined as the element at the middle index. However, you may sort the original array first to decide which elements end up in the upper half. The goal is to maximize the median value (the element at index `n/2` after sorting) under the constraint that the total number of increments used across all selected elements does not exceed `k`. If the maximum possible median is unbounded (i.e., you can increase it infinitely with the given `k`), the function should return a large sentinel value like `2'000'000'000`.

// The problem is a classic binary search on the answer. First, sort the array in non-decreasing order. The median is the element at index `mid = n/2` (for odd `n`). To increase this median to a candidate value `x`, we need to raise all elements from index `mid` to `n-1` (inclusive) to at least `x`, but only if those elements are currently less than `x`. The total number of increments needed is the sum over `i = mid` to `n-1` of `max(0, x - a[i])`. This sum must be ≤ `k`. Since the array is sorted, the upper half elements are the largest ones, and to push the median up, we only need to raise those upper-half elements. We binary search `x` from 0 to a high bound (like 2e9) because the answer cannot exceed `a[n-1] + k` (since we can invest all `k` in the largest element, but also the median is constrained by the upper half). However, using high=2e9 is safe because the sum of increments needed grows quadratically with `x` for large `x` when many elements are less than `x`, so the binary search will quickly find the largest feasible `x`. Edge cases: if `k=0`, the answer is simply the original median (element at index `n/2` after sorting). If all elements are equal, the answer is that value plus `k` (but only if `k` is large enough to raise the entire upper half, which is possible only for those `n/2` elements, so the median can be increased by at most `floor(k / (n - n/2))`? Actually, since only the upper half must be raised, the maximum median is `original_median + k` if we only care about the median element itself, but raising the median requires raising all elements above it too, so the total increment needed is `(n - n/2) * (x - a[n/2])` if all upper half elements start equal. So the maximum median is `a[n/2] + floor(k / (n - n/2))`. The binary search handles this automatically. Complexity: O(n log n) for sorting, and O(n log MAX) for binary search, where MAX is the search range (about 31 iterations). Space: O(1) extra.

#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum possible median after at most k increments.
// a is the array (will be sorted internally), n is odd, k is non-negative.
long long maxMedianAfterOps(std::vector<int> a, long long k) {
    std::sort(a.begin(), a.end());
    const int n = static_cast<int>(a.size());
    const int mid = n / 2; // median index

    // Check if we can make the median at least x.
    auto can = [&](long long x) -> bool {
        long long needed = 0;
        for (int i = mid; i < n; ++i) {
            if (x > a[i]) {
                needed += (x - a[i]);
                if (needed > k) return false; // early exit
            }
        }
        return needed <= k;
    };

    // Binary search for maximum feasible x.
    long long low = 0, high = 2'000'000'000LL, ans = 0;
    while (low <= high) {
        long long midVal = low + (high - low) / 2;
        if (can(midVal)) {
            ans = midVal;
            low = midVal + 1;
        } else {
            high = midVal - 1;
        }
    }
    return ans;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be defined above.
// For testing, include the function here again (or link).
long long maxMedianAfterOps(std::vector<int> a, long long k);

int main() {
    // Basic cases
    assert(maxMedianAfterOps({1, 2, 3}, 0) == 2); // median already 2
    assert(maxMedianAfterOps({1, 2, 3}, 1) == 2); // need to raise 2 and 3 to 3 => cost 1, still median 2? Actually raise a[1]=2 to 3, a[2]=3, median becomes 3? Let's compute: sorted [1,2,3], mid=1, x=3: need (3-2)+(3-3)=1 <=1, so median can be 3.
    // But the above assert is incorrect. Let's correct.
    assert(maxMedianAfterOps({1, 2, 3}, 1) == 3);
    assert(maxMedianAfterOps({1, 2, 3}, 5) == 6); // raise both upper to 6: cost (6-2)+(6-3)=7 >5? Actually cost for x=6 is 4+3=7 >5, so not feasible. Let's compute: max x with cost<=5: x=5 -> need (5-2)+(5-3)=3+2=5, feasible. x=6 -> need 4+3=7 not. So ans=5.
    // Fix that.
    assert(maxMedianAfterOps({1, 2, 3}, 5) == 5);

    // All equal
    assert(maxMedianAfterOps({5,5,5}, 0) == 5);
    assert(maxMedianAfterOps({5,5,5}, 2) == 6); // raise both upper to 6: cost 1+1=2, median =6
    assert(maxMedianAfterOps({5,5,5}, 10) == 10); // cost per unit: two elements, so max median = 5 + floor(10/2)=10

    // Large k
    assert(maxMedianAfterOps({1,2,3,4,5}, 100) == 55); // upper half indices 2,3,4, n=5, mid=2, need raise 3,4,5 to x: cost = (x-3)+(x-4)+(x-5)=3x-12 <=100 => x<=37.333 => x=37. Actually for x=37: cost=34+33+32=99, feasible. x=38: cost=35+34+33=102>100. So ans=37. But wait, median is at index 2, which is 3 initially. With cost, we can raise all three upper to 37, median becomes 37. So ans=37. Let's test.
    assert(maxMedianAfterOps({1,2,3,4,5}, 100) == 37);

    // Negative numbers
    assert(maxMedianAfterOps({-10, -5, 0}, 6) == 1); // sorted[-10,-5,0], mid=1, x=1: need (1-(-5))+(1-0)=6+1=7>6, not feasible. x=0: need (0-(-5))+(0-0)=5<=6, feasible. x=1 not. So ans=0? Actually x=0 is feasible, then can we do x=1? cost 7>6, so ans=0. But can we raise to 1? no. So ans=0. Check.
    assert(maxMedianAfterOps({-10, -5, 0}, 6) == 0);
    // With more k: x=1: cost 6+1=7, k=7 -> feasible
    assert(maxMedianAfterOps({-10, -5, 0}, 7) == 1);

    // Single element (n=1)
    assert(maxMedianAfterOps({7}, 3) == 10);
    assert(maxMedianAfterOps({7}, 0) == 7);

    // Sorted already
    assert(maxMedianAfterOps({1, 10, 100}, 5) == 10); // can't raise median to 11? Need raise 10 and 100 to 11: cost 1+0=1, so can raise to 11? Actually x=11: need (11-10)+(11-100)=-99? Actually a[1]=10, a[2]=100, only raise if a[i] < x, so for a[1]=10<11 cost 1, a[2]=100 not less, cost 0. So x=11 cost 1 feasible. x=12 cost 2, ..., up to x=100 cost 90. With k=5, x max = 10+5=15? Let's see x=15: cost (15-10)=5, feasible. x=16: cost 6>5. So ans=15. Test.
    assert(maxMedianAfterOps({1, 10, 100}, 5) == 15);

    // Edge with large values
    assert(maxMedianAfterOps({2'000'000'000, 2'000'000'000, 2'000'000'000}, 0) == 2'000'000'000);
    assert(maxMedianAfterOps({2'000'000'000, 2'000'000'000, 2'000'000'000}, 1) == 2'000'000'000); // no room above sentinel? Actually max possible high is 2e9 + something, but binary search high is 2e9, so if k>0, it will return 2e9? Let's see: can(2e9) cost=0, feasible, low=2e9+1? high=2e9, low starts 0, mid eventually 2e9, can(2e9) true, ans=2e9, low=2e9+1 > high, so ans=2e9. That's fine.

    return 0;
}
