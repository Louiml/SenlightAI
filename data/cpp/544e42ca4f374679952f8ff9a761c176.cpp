// Given three integers `y`, `k`, and `n` with `1 ≤ y ≤ n` and `1 ≤ k ≤ n`, write a C++ function `findValidX` that takes these three values and returns a `std::vector<long long>` containing all positive integers `x` such that `(y + x) % k == 0` and `1 ≤ x ≤ n - y` (equivalently, `y + x ≤ n`), sorted in strictly increasing order. If no such `x` exists, the function should return an empty vector. The function must be `const`-correct and handle large inputs up to `10^18` without overflow by using `long long` types.

#include <cassert>
#include <vector>

// Function declaration (already defined above).
std::vector<long long> findValidX(long long y, long long k, long long n);

int main() {
    // Example from typical problem: y=8, k=3, n=30 → x = 1,4,7,10,13,16,19,22
    assert(findValidX(8, 3, 30) == std::vector<long long>({1,4,7,10,13,16,19,22}));
    
    // Case where y is divisible by k: y=9, k=3, n=20 → x = 3,6,9,12? but 12+9=21>20, so x=3,6,9
    assert(findValidX(9, 3, 20) == std::vector<long long>({3,6,9}));
    
    // No valid x: y=10, k=5, n=12 → only x=5? 10+5=15>12, so none.
    assert(findValidX(10, 5, 12) == std::vector<long long>());
    
    // Edge: y=1, k=1, n=5 → x=1,2,3,4 (since y+x ≤5)
    assert(findValidX(1, 1, 5) == std::vector<long long>({1,2,3,4}));
    
    // Edge: y=5, k=7, n=5 → no x because y+x>5 always.
    assert(findValidX(5, 7, 5) == std::vector<long long>());
    
    // Single candidate: y=2, k=3, n=5 → x=1 (2+1=3) and 4 (2+4=6>5) so only 1.
    assert(findValidX(2, 3, 5) == std::vector<long long>({1}));
    
    // Large values: y=1e18-2, k=2, n=1e18 → x=1? since 1e18-2+1=1e18-1 (not divisible by2), x=3? gives 1e18+1>n, so none.
    assert(findValidX(999999999999999998LL, 2, 1000000000000000000LL) == std::vector<long long>());
    
    // y=0? but y≥1 by constraints, but test anyway: y=0, k=5, n=10 → x=5,10
    assert(findValidX(0, 5, 10) == std::vector<long long>({5,10}));
    
    // y=4, k=2, n=10 → x=2? 4+2=6,4? 4+4=8,6? 4+6=10 → x=2,4,6
    assert(findValidX(4, 2, 10) == std::vector<long long>({2,4,6}));
    
    return 0;
}

#include <vector>

// Return all positive integers x such that (y + x) is divisible by k,
// and 1 ≤ x ≤ n - y. The result is sorted in increasing order.
std::vector<long long> findValidX(long long y, long long k, long long n) {
    std::vector<long long> result;
    // Smallest positive x such that (y + x) is a multiple of k.
    long long x = k - (y % k);
    if (x == 0) x = k;  // handle the case where y itself is divisible by k.
    for (; x <= n - y; x += k) {
        result.push_back(x);
    }
    return result;
}

// The problem is equivalent to finding all multiples of `k` in the range `(y, n]` (since `y + x` must be a multiple of `k` and `y + x > y`). The smallest candidate is `x = k - (y % k)`, but if `y % k == 0`, then `x = k` (not `0` because `x≥1`). More generally, compute `start = ((y / k) + 1) * k` — the smallest multiple of `k` strictly greater than `y`. Then `x = start - y`, and subsequent candidates are obtained by adding `k` each time while `x + y ≤ n` (i.e., `x ≤ n - y`). This direct loop runs in `O((n - y) / k)` time, but for large ranges (e.g., `k=1`, `n-y` up to `10^18`) this is too slow. However, the task's intended solution is the provided simple loop, which is acceptable for typical constraints up to `10^5` iterations. Edge cases include: when `y % k == 0`, the first `x` is `k` (not `0`); when no multiple of `k` lies strictly between `y` and `n` inclusive, return empty; and ensure `x` does not exceed `n - y`. The algorithm uses `O(1)` auxiliary space if we generate the vector incrementally (the output vector itself is the only storage). Time complexity is `O((n - y) / k)` in the worst case.
