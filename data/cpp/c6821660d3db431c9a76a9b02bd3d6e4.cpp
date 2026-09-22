// You are given the prices of `n` items (as integers, each representing a percentage of some base value), and two special discount schemes. For a chosen prefix of length `m` (i.e., you only consider the first `m` items after sorting them in descending order of price), every item whose 1‑based position in that prefix is a multiple of `a` gets an extra `x` percent of its price added to the total, and every item whose position is a multiple of `b` gets an extra `y` percent added. Items that satisfy both conditions get both bonuses (`x + y` percent). Write a C++ function `int minTickets(vector<int>& prices, int x, int a, int y, int b, long long k)` that, given the unsorted prices (each already divided by 100 in the original snippet, but you should treat them as raw percent values), returns the smallest integer `m` such that by selecting the `m` most expensive items (i.e., sort descending, take first `m`), the total sum of (price + bonuses) is at least `k`. If no such `m` exists for any prefix (including the full list), return `-1`. The function must not modify the input vector. Constraints: `1 ≤ n ≤ 200,000`, prices are positive integers up to `10^9`, `x,y ≤ 100`, `1 ≤ a,b ≤ n`, `k` fits in 64‑bit. The answer must be optimal; you may assume there is always at least one item. Note: the original code divides prices by 100, but this task avoids that scaling; you must handle large sums using `long long`.

The core idea is to sort the prices in descending order and precompute prefix sums of the sorted array. For a candidate prefix length `m`, we need to know how many items in that prefix are divisible by both `a` and `b`, only by `a`, and only by `b`. These counts can be computed analytically using counting loops up to `m` (which is fine because each query is O(m)). However, a naive binary search that calls this counting for each mid would be O(n log n) after sorting, which is acceptable for n=200,000. More precisely, we binary search on `m` from 1 to n. At each `mid`, we count:
- `cxy` = number of i in [1, mid] such that i % a == 0 and i % b == 0.
- `cx` = number of i such that i % a == 0 but not both.
- `cy` = number of i such that i % b == 0 but not both.
Then we assign the highest prices to the most beneficial categories: first the `cxy` items get `x+y` bonus, then the next `cx` items get `x` bonus, then the next `cy` items get `y` bonus (after ensuring `x >= y` by swapping). The total is computed using prefix sums: `sum[cxy]*(x+y) + (sum[cx+cxy]-sum[cxy])*x + (sum[cx+cy+cxy]-sum[cx+cxy])*y`. If this total >= k, we try a smaller `m`; otherwise larger. The binary search runs O(log n) iterations, each O(m) counting, so total O(n log n) time. Space is O(n) for the sorted array and prefix sums. Edge cases: when `x < y`, swap both `x` and `a` with `y` and `b` to ensure the higher bonus is assigned to the `cxy` items and the next larger bonus to `cx`. If even with `m = n` the sum is below `k`, return `-1`. The counting loop is straightforward and handles large `m` correctly because positions are 1‑based.

#include <vector>
#include <algorithm>

// Returns the smallest prefix length m such that the sum of (price + bonus)
// for the m most expensive items is at least k. Returns -1 if impossible.
// prices are raw percent values (not divided by 100).
int minTickets(std::vector<int>& prices, int x, int a, int y, int b, long long k) {
    int n = static_cast<int>(prices.size());
    // Sort descending
    std::sort(prices.begin(), prices.end(), std::greater<int>());
    
    // Prefix sums
    std::vector<long long> sum(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        sum[i + 1] = sum[i] + static_cast<long long>(prices[i]);
    }
    
    // Ensure x >= y by swapping both bonus and divisor
    if (x < y) {
        std::swap(x, y);
        std::swap(a, b);
    }
    
    // Helper lambda to compute total for a given prefix length m
    auto calc = [&](int m) -> long long {
        int cxy = 0, cx = 0, cy = 0;
        for (int i = 1; i <= m; ++i) {
            if (i % a == 0 && i % b == 0) {
                ++cxy;
            } else if (i % a == 0) {
                ++cx;
            } else if (i % b == 0) {
                ++cy;
            }
        }
        long long total = 0;
        if (cxy > 0) {
            total += sum[cxy] * static_cast<long long>(x + y);
        }
        if (cx > 0) {
            total += (sum[cx + cxy] - sum[cxy]) * static_cast<long long>(x);
        }
        if (cy > 0) {
            total += (sum[cx + cy + cxy] - sum[cx + cxy]) * static_cast<long long>(y);
        }
        return total;
    };
    
    // Binary search for minimal m
    int lo = 1, hi = n;
    int ans = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (calc(mid) >= k) {
            ans = mid;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}

#include <cassert>
#include <vector>

// Forward declaration (the solution function is assumed to be in scope)
int minTickets(std::vector<int>& prices, int x, int a, int y, int b, long long k);

int main() {
    // Basic test: prices [100, 200, 300], a=2, b=3, x=10, y=20
    // Sorted: [300, 200, 100]
    // Prefix len 1: only position 1 – none of a or b divides 1 → total = 300
    // Prefix len 2: positions 1,2 – 2 divisible by a (10% of 200=20), total=300+200+20=520
    // Prefix len 3: positions 1,2,3 – 2 divisible by a, 3 divisible by b (20% of 100=20), total=300+220+120=640
    std::vector<int> p1 = {100, 200, 300};
    assert(minTickets(p1, 10, 2, 20, 3, 500) == 2);  // 520 >= 500
    assert(minTickets(p1, 10, 2, 20, 3, 520) == 2);
    assert(minTickets(p1, 10, 2, 20, 3, 521) == 3);  // need 640
    assert(minTickets(p1, 10, 2, 20, 3, 1000) == -1); // max 640
    
    // Test with both bonuses for same position: a=2, b=4, x=5, y=7
    // Sorted: [100, 80, 60, 40]
    // Prefix len 1: pos1 no bonus → 100
    // Prefix len 2: pos2 divisible by 2 and 4? 2%2==0, 2%4!=0 → only a → 80+4=84, total 184
    // Prefix len 3: pos3? 3%2,3%4 no → 60, total 244
    // Prefix len 4: pos4 divisible by both → 40+ (5+7)% of 40 = 40+4.8=44.8 → total 288.8, but integer total = 100+84+60+44 = 288
    std::vector<int> p2 = {100, 80, 60, 40};
    assert(minTickets(p2, 5, 2, 7, 4, 200) == 3); // 244 >= 200
    assert(minTickets(p2, 5, 2, 7, 4, 244) == 3);
    assert(minTickets(p2, 5, 2, 7, 4, 245) == 4); // 288
    
    // Test swapping: x < y → we swap so x=7, a=4; y=5, b=2
    // Same as above but now positions divisible by 4 get x+y, divisible by 2 get x
    // Prefix len 4: pos4 gets 7+5=12% of 40 = 4.8, pos2 gets 7% of 80=5.6 → total = 100+80+60+40+5+4 = 289
    assert(minTickets(p2, 5, 2, 7, 4, 289) == 4);
    
    // Single item test
    std::vector<int> p3 = {50};
    assert(minTickets(p3, 10, 1, 20, 1, 60) == 1); // 50+30% = 65 >= 60
    assert(minTickets(p3, 10, 1, 20, 1, 100) == -1); // max 65 < 100
    
    // All items multiples of both a and b
    std::vector<int> p4 = {10, 20, 30};
    // a=1, b=1 → every position gets x+y=50% bonus
    // Sorted: [30,20,10] → total for m=1: 30*1.5=45, m=2: 45+20*1.5=75, m=3: 75+15=90
    assert(minTickets(p4, 20, 1, 30, 1, 75) == 2);
    assert(minTickets(p4, 20, 1, 30, 1, 76) == 3);
    
    // Large n test (just to ensure no overflow, not checking exact answer)
    std::vector<int> p5(1000, 1000); // all 1000
    // a=2, b=5, x=10, y=20 → sorted all equal
    // For any m, we need to compute manually; basic sanity: total for m=5 should be >= k for k=5000
    assert(minTickets(p5, 10, 2, 20, 5, 5000) <= 5);
    
    return 0;
}
