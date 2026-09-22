// Write a C++ function `long long minimumUnhappiness(long long n, long long m)` that solves the following problem. You have `n` items arranged in a line, and you must paint exactly `m` of them black (the remaining `n - m` remain white). The total "unhappiness" of a coloring is the sum over all pairs of items `(i, j)` with `i < j` of a penalty: if both items are white, the penalty is `1`; otherwise (if at least one is black), the penalty is `0`. You may choose any subset of `m` items to be black. The function should return the minimum possible total unhappiness. The constraints are: `0 ≤ m ≤ n ≤ 10^12`, and both are given as 64-bit integers. For example, if `n = 5` and `m = 2`, you could paint items 2 and 4 black, leaving whites at positions 1, 3, 5. Then each pair of whites contributes 1, so the total unhappiness is the number of white pairs = C(3,2) = 3. Other colorings might give more or fewer unhappiness, but the minimum is 3. If `m = 0` (all white), the unhappiness is C(n,2) = n(n-1)/2. If `m >= n/2`, you can break all white pairs, so the minimum is 0. For `m < n/2`, you must place `n - m` whites in `m + 1` gaps (before first black, between blacks, after last black), and the optimal is to distribute whites as evenly as possible across these gaps, because the number of white pairs is the sum of C(k_i,2) over each gap size `k_i`. The minimum occurs when the gap sizes differ by at most 1.

#include <cassert>

// The computeValue function is declared above.

int main() {
    // m == 0
    assert(computeValue(0, 0) == 0);
    assert(computeValue(5, 0) == 0);
    assert(computeValue(100, 0) == 0);

    // 2*m >= n
    assert(computeValue(1, 1) == 1 * 2 / 2 - (1 - 1) = 1 - 0 = 1);
    assert(computeValue(2, 1) == 2 * 3 / 2 - (2 - 1) = 3 - 1 = 2);
    assert(computeValue(5, 3) == 5 * 6 / 2 - (5 - 3) = 15 - 2 = 13);
    assert(computeValue(6, 3) == 6 * 7 / 2 - (6 - 3) = 21 - 3 = 18);

    // General case m > 0 and 2*m < n
    // n=5,m=2: t=3,g=3,q=1,r=0 -> total=15, subtract=3*1*2/2=3 -> 12
    assert(computeValue(5, 2) == 12);
    // n=7,m=2: t=5,g=3,q=1,r=2 -> total=28, subtract=2*2*3/2 + 1*1*2/2 = 6+1=7 -> 21
    assert(computeValue(7, 2) == 21);
    // n=9,m=2: t=7,g=3,q=2,r=1 -> total=45, subtract=1*3*4/2 + 2*2*3/2 = 6+6=12 -> 33
    assert(computeValue(9, 2) == 33);

    // Boundary: m=1, n=3 (2*m=2 <3) -> t=2,g=2,q=1,r=0 -> total=6, subtract=2*1*2/2=2 -> 4
    assert(computeValue(3, 1) == 4);
    // n=4,m=1 (2*m=2 <4) -> t=3,g=2,q=1,r=1 -> total=10, subtract=1*2*3/2 + 1*1*2/2 = 3+1=4 -> 6
    assert(computeValue(4, 1) == 6);

    // Larger test, n=1000000, m=1 (2*m < n) -> t=999999, g=2, q=499999, r=1
    // total = 1000000*1000001/2 = 500000500000
    // subtract = 1*(500000)*(500001)/2 + 1*499999*500000/2 = 125000250000 + 124999750000 = 250000000000
    // result = 500000500000 - 250000000000 = 250000500000
    assert(computeValue(1000000, 1) == 250000500000LL);

    return 0;
}

#include <algorithm>

// Returns the minimum total unhappiness for n items with exactly m black.
// Unhappiness = number of pairs of white items.
long long minimumUnhappiness(long long n, long long m) {
    if (m == 0) {
        // All items white -> all pairs contribute.
        return n * (n - 1) / 2;
    }
    if (2 * m >= n) {
        // Enough black items to isolate every white item.
        return 0;
    }
    // Total number of white items.
    long long totalWhite = n - m;
    // Number of gaps = m + 1.
    long long gaps = m + 1;
    // Base size of each gap.
    long long base = totalWhite / gaps;
    long long extra = totalWhite % gaps;
    // extra gaps have size (base+1), the rest have size base.
    // Unhappiness = extra * C(base+1,2) + (gaps-extra) * C(base,2).
    long long unhappiness = 0;
    if (extra > 0) {
        long long sizeBig = base + 1;
        unhappiness += extra * sizeBig * (sizeBig - 1) / 2;
    }
    if (gaps - extra > 0) {
        unhappiness += (gaps - extra) * base * (base - 1) / 2;
    }
    return unhappiness;
}

// The key insight is that the unhappiness depends only on the positions of the white items, specifically the number of pairs of whites. Since black items break the adjacency, the whites are partitioned into groups separated by black items. If there are `m` black items, there are `m+1` gaps (including the ends). Let `k_i` be the number of white items in gap `i`. The total unhappiness is `sum_{i} C(k_i, 2) = sum_{i} k_i*(k_i-1)/2`. We need to minimize this sum given that `sum k_i = n - m` (the total whites) and each `k_i ≥ 0`. The sum of squares (or sum of triangular numbers) is convex, so the minimum is achieved when the `k_i` are as equal as possible. So if `q = (n-m) / (m+1)` (integer division) and `r = (n-m) % (m+1)`, then `r` gaps have size `q+1` and the remaining `(m+1-r)` gaps have size `q`. The minimum unhappiness is `r * C(q+1,2) + (m+1-r) * C(q,2)`. Edge cases: if `m == 0`, then there is only one gap containing all `n` whites, so unhappiness is `C(n,2)` = n*(n-1)/2. If `m >= n/2` (i.e., `2*m >= n`), then `n-m ≤ m` and you can place each white in its own gap (or no two whites in the same gap), so all gaps have size 0 or 1, giving unhappiness 0. The main formula also works for `m=0` if you handle division by zero separately. Time complexity is O(1), space O(1).
