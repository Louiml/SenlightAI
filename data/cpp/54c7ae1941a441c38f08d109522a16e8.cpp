// Given a sequence of `n` integers `x[1], x[2], …, x[n]` (where `n ≥ 1`), and three integer multipliers `p`, `q`, `r` (which may be negative, zero, or positive), find the maximum possible value of the expression `p·x[a] + q·x[b] + r·x[c]` over all indices `1 ≤ a ≤ b ≤ c ≤ n`. Write a self-contained C++ function `long long maximumWeightedTriple(const std::vector<long long>& x, long long p, long long q, long long r)` that returns this maximum value. The result may exceed 32‑bit range, so use `long long`. The input vector is non‑empty. Consider carefully that `p`, `q`, `r` can be negative, and the optimal choice may need to skip elements (by repeating indices) to avoid negative contributions.

The problem is a classic dynamic programming with three stages, where we process the array left‑to‑right maintaining the best value achievable after selecting the first term, then after the second, then after the third. Define three states:
- `best1` = maximum value of `p·x[i]` for some index `i` processed so far.
- `best2` = maximum value of `p·x[a] + q·x[b]` for `a ≤ b` among processed indices.
- `best3` = maximum value of `p·x[a] + q·x[b] + r·x[c]` for `a ≤ b ≤ c` among processed indices.

Initialize `best1` to `p·x[0]` (but careful: we can start at index 1; we'll handle first element separately). For each subsequent element `x[i]`, we update:
- `best3 = max(best3, best2 + r·x[i])` (the third element is `x[i]`).
- `best2 = max(best2, best1 + q·x[i])`.
- `best1 = max(best1, p·x[i])`.

Why this order? Because `best2` must be computed before updating `best3` (since `best3` depends on `best2` from previous steps), and `best1` must be computed before updating `best2`. This ensures we allow `a ≤ b ≤ c` with possible equality. For the first element, initialize all three states as `best1 = p*x[0]`, `best2 = best1 + q*x[0]`, `best3 = best2 + r*x[0]`. Then iterate from index 1 to n-1. Edge cases: if `n = 1`, answer is simply `p*x[0] + q*x[0] + r*x[0]`. Because multipliers can be negative, we always take the maximum (not the minimum). Time complexity is O(n), space O(1). No overflow issues since we use `long long` and the limits are within 64‑bit (if inputs are within typical 1e9 and multipliers within 1e9, product could be up to 1e18, which fits in signed 64‑bit).

#include <vector>
#include <algorithm>
#include <cstddef>

// Computes max_{1<=a<=b<=c<=n} (p*x[a] + q*x[b] + r*x[c])
long long maximumWeightedTriple(const std::vector<long long>& x,
                                long long p,
                                long long q,
                                long long r) {
    const std::size_t n = x.size();
    if (n == 0) return 0; // not expected, but safe

    // Initialize states with the first element.
    long long best1 = p * x[0];
    long long best2 = best1 + q * x[0];
    long long best3 = best2 + r * x[0];

    // Process remaining elements.
    for (std::size_t i = 1; i < n; ++i) {
        long long val = x[i];
        // Update third term first (uses previous best2).
        best3 = std::max(best3, best2 + r * val);
        // Update second term (uses previous best1).
        best2 = std::max(best2, best1 + q * val);
        // Update first term.
        best1 = std::max(best1, p * val);
    }

    return best3;
}

#include <cassert>
#include <vector>
#include <iostream>

// Declaration of the function under test.
long long maximumWeightedTriple(const std::vector<long long>& x,
                                long long p,
                                long long q,
                                long long r);

int main() {
    // Test 1: Simple positive case
    assert(maximumWeightedTriple({1, 2, 3}, 1, 1, 1) == 1*1 + 2*1 + 3*1);
    // = 1+2+3=6

    // Test 2: Single element
    assert(maximumWeightedTriple({5}, 2, -3, 4) == 2*5 + (-3)*5 + 4*5);
    // = 10 -15 +20 = 15

    // Test 3: Negative multipliers, may skip elements by repeating
    // p=1, q=-1, r=1 : best is pick the same smallest? Actually we want max.
    // For x = {10, -5, 8}: choose a=2,b=2,c=3? Let's compute manually:
    // all possibilities: a=1,b=1,c=1:10-10+10=10; a=1,b=1,c=2:10-10+8=8; a=1,b=1,c=3:10-10+(-5)=-5; a=1,b=2,c=2:10-(-5)+(-5)=10; a=1,b=2,c=3:10-(-5)+8=23; a=1,b=3,c=3:10-8+8=10; a=2,b=2,c=2: -5+5-5=-5; a=2,b=3,c=3:-5-8+8=-5; a=3,b=3,c=3:8-8+8=8. Max = 23 at (a=1,b=2,c=3). Test:
    assert(maximumWeightedTriple({10, -5, 8}, 1, -1, 1) == 23);

    // Test 4: All negative values, r positive maybe pick last negative?
    // x = {-1, -2}, p=-1,q=1,r=-1: maximize -(-a)+b -c? 
    // Try: a=1,b=2,c=2: -(-1)+(-2)-(-2)=1-2+2=1. a=1,b=1,c=1:1-1+1=1. a=2,b=2,c=2:2-2+2=2. So max=2.
    assert(maximumWeightedTriple({-1, -2}, -1, 1, -1) == 2);

    // Test 5: Zeros and large numbers
    assert(maximumWeightedTriple({0, 1000000000}, 1000000000, 1000000000, 1000000000) == 0 + 1e18 + 1e18);
    // Actually best: a=1,b=2,c=2: 0*1e9 + 1e9*1e9? Wait: p*x[1]=0, q*x[2]=1e9*1e9=1e18? No q=1e9, x[2]=1e9 => 1e18, r similarly => total 2e18? But also a=2,b=2,c=2 gives 3e18. So answer 3e18.
    assert(maximumWeightedTriple({0, 1000000000}, 1000000000, 1000000000, 1000000000) == 3000000000000000000LL);

    // Test 6: Only one index allowed (n=3 but all same values)
    assert(maximumWeightedTriple({7, 7, 7}, -2, 3, -1) == -2*7 + 3*7 -1*7);
    // = -14+21-7=0

    // Test 7: Long vector simple
    std::vector<long long> vec = {1, 2, 3, 4, 5};
    assert(maximumWeightedTriple(vec, -1, 2, -3) == -1*1 + 2*5 -3*5);
    // best: choose a=1,b=5,c=5: -1 + 10 -15 = -6. Try other combos: a=1,b=4,c=5: -1+8-15=-8; a=2,b=5,c=5:-2+10-15=-7; a=5... So -6 is max.
    assert(maximumWeightedTriple(vec, -1, 2, -3) == -6);

    // Test 8: All zero
    assert(maximumWeightedTriple({0, 0, 0}, -5, 2, 1) == 0);

    std::cout << "All tests passed.\n";
    return 0;
}
