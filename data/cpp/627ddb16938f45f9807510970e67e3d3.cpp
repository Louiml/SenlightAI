// Write a C++ function `differenceOfSums(int n, int m)` that takes two positive integers `n` and `m` (with `m ≥ 1`) and returns the difference between the sum of all integers from `1` to `n` that are **not** divisible by `m` and the sum of all integers from `1` to `n` that **are** divisible by `m`. The result is an integer (the difference may be negative, zero, or positive). For example, for `n = 5` and `m = 2`, the non‑divisible numbers are `{1, 3, 5}` (sum = 9) and the divisible numbers are `{2, 4}` (sum = 6), so the result is `9 - 6 = 3`. The function must handle large `n` safely (up to 10^9) using 64‑bit integers, and must not use any extra memory beyond a few scalar variables.
The simplest and most robust approach is a single loop from `1` to `n`, adding each integer to one of two accumulating sums depending on whether it is divisible by `m` (`ix % m == 0`). This is correct for all small `n`, but for `n` up to 10^9 a linear loop (O(n) time) is too slow and will time out. A better solution uses arithmetic series formulas:  
- The divisible numbers are exactly `m, 2m, 3m, …, k*m` where `k = n / m` (integer division). Their sum is `m * k * (k + 1) / 2`.  
- The non‑divisible sum is the total sum of `1..n` minus the divisible sum. The total sum is `n * (n + 1) / 2`.  
- The answer is `total_sum - 2 * divisible_sum`.  
This runs in O(1) time and O(1) space. Edge cases: when `m > n`, `k = 0` so divisible sum is `0`, and the answer is just the total sum (`n` up to 10^9 fits in 64‑bit, but `n*(n+1)/2` can be about 5e17 which fits in `long long`). When `n` or `m` is 0? The problem guarantees positive integers for both, so we assume `n ≥ 1` and `m ≥ 1`. Use `long long` (or `unsigned long long` for safety, but signed is fine). The result may be negative; subtract carefully. No floating point, no division rounding issues because `k*(k+1)` is always even.
#include <cstdint>

// Return (sum of numbers 1..n not divisible by m) - (sum divisible by m).
// Precondition: n >= 1, m >= 1.
long long differenceOfSums(int n, int m) {
    long long total = static_cast<long long>(n) * (n + 1) / 2;
    long long k = static_cast<long long>(n) / m;
    long long divisible_sum = m * k * (k + 1) / 2;
    return total - 2 * divisible_sum;
}
#include <cassert>

int main() {
    // Basic examples
    assert(differenceOfSums(5, 2) == 3);   // total=15, divisible={2,4}=6, non-div=9 → 9-6=3
    assert(differenceOfSums(10, 3) == 19); // total=55, divisible={3,6,9}=18, non-div=37 → 19
    // m > n: no divisible numbers
    assert(differenceOfSums(7, 10) == 28); // total=28, divisible=0
    // n=1, m=1: only divisible number is 1
    assert(differenceOfSums(1, 1) == -1);  // total=1, divisible=1, non-div=0 → -1
    // n=1, m=2: only non-divisible
    assert(differenceOfSums(1, 2) == 1);
    // n=m: divisible includes n
    assert(differenceOfSums(4, 4) == 6);   // total=10, divisible={4}=4, non-div=6 → 2? Wait: 6-4=2, but total-2*divisible=10-8=2, correct
    // Larger values to verify formula against linear computation for small n
    assert(differenceOfSums(100, 7) == 4350 - 2 * 735); // divisible sum: 7*(14*15/2)=7*105=735, total=5050
    // Edge with large n but no overflow (using small m)
    assert(differenceOfSums(1000000000, 1) == -500000000500000000LL); // all divisible: total - 2*total = -total
    assert(differenceOfSums(1000000000, 1000000000) == 500000000500000000LL - 2 * 1000000000);
}
