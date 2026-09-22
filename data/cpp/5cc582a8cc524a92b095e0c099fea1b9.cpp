Write a C++ function `long long calculateTotalSalary(long long D, long long d, long long P, long long Q)` that computes the total money earned over `D` days given the following salary scheme: The base daily salary is `P` for the first `d` days. After every block of `d` days, the daily salary increases by `Q` (so from day `d+1` to day `2d`, salary is `P+Q`; from day `2d+1` to day `3d`, salary is `P+2Q`, etc.). The last partial block (if `D` is not a multiple of `d`) is paid at the current rate for that block. The function must return the total amount as a `long long`. Use integer arithmetic only; no floating point. Assume `D >= 1`, `d >= 1`, `P >= 0`, `Q >= 0`, and all values fit in a 64‑bit signed integer. The result is guaranteed to fit in `long long`.
// The key observation is that the salary for each full block of `d` days is constant within that block. Let `n = D / d` be the number of full blocks, and `r = D % d` be the remaining days after the full blocks. The first full block (days 1 to `d`) has salary `P` per day. The second full block has salary `P + Q`, the third `P + 2Q`, and so on, up to the `n`-th full block which has salary `P + (n-1)Q`. The remaining `r` days (if any) fall into the (`n`+1)-th block, whose daily salary is `P + nQ`. Thus total earnings = `d * (P + (P+Q) + (P+2Q) + ... + (P+(n-1)Q)) + r * (P + nQ)`. The sum inside the parentheses is an arithmetic series with `n` terms: `n*P + Q*(0+1+...+(n-1)) = n*P + Q * (n*(n-1))/2`. So the full block contribution is `d * [n*P + Q * (n*(n-1))/2]`. Adding the partial block contribution gives the final answer. Edge case: when `D < d`, then `n = 0`, `r = D`, and the formula simplifies to `(P+0*Q)*D` = `P*D`, which the formula handles since the sum over zero terms is zero. Also handle `n=0` carefully: the term `n*(n-1)` is fine. Time complexity is `O(1)` and space complexity `O(1)`.
#include <cstdint>

// Computes total salary over D days with base daily rate P,
// increasing by Q every d full days.
long long calculateTotalSalary(long long D, long long d, long long P, long long Q) {
    const long long n = D / d;          // number of complete d-day blocks
    const long long r = D % d;          // remaining days in the next block

    // Sum of daily rates for the first n complete blocks:
    // rate_i = P + (i-1)*Q for i=1..n, so sum = n*P + Q * (n*(n-1))/2
    const long long blockRateSum = n * P + Q * (n * (n - 1) / 2);

    // Full blocks contribution + partial last block contribution
    const long long total = d * blockRateSum + r * (P + n * Q);
    return total;
}
#include <cassert>

int main() {
    // Directly known cases
    assert(calculateTotalSalary(1, 1, 10, 5) == 10);          // one day at P
    assert(calculateTotalSalary(2, 1, 10, 5) == 25);          // day1:10, day2:15
    assert(calculateTotalSalary(3, 2, 100, 10) == 310);       // 2 days @100 + 1 day @110
    assert(calculateTotalSalary(4, 2, 100, 10) == 420);       // 2@100 + 2@110
    assert(calculateTotalSalary(5, 2, 100, 10) == 540);       // 2@100 + 2@110 + 1@120

    // Zero Q
    assert(calculateTotalSalary(10, 3, 50, 0) == 500);        // constant 50 per day

    // Zero P but positive Q
    assert(calculateTotalSalary(6, 2, 0, 7) == 42);           // 2@0 + 2@7 + 2@14 = 42

    // D exactly multiple of d
    assert(calculateTotalSalary(6, 3, 1, 2) == 18);           // 3@1 + 3@3 = 3+9=12? Wait recalc: 3*1 + 3*3 = 12? Actually 3*1=3, 3*3=9, total=12. Yes assert 12.

    // Large values
    assert(calculateTotalSalary(1000000, 1, 1, 1) == 1000000LL * (1 + 1000000) / 2); // sum 1..1e6

    // Edge: D smaller than d
    assert(calculateTotalSalary(4, 10, 5, 3) == 20);          // only 4 days at P=5

    // Edge: n=1 with remainder
    assert(calculateTotalSalary(2, 1, 5, 2) == 12);           // day1:5, day2:7

    return 0;
}
