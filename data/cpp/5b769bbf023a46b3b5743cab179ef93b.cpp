Write a C++ function named `sumMultiplesBelow` that takes a single positive integer `n` and returns the sum of all natural numbers strictly less than `n` that are divisible by 3 or 5. The function must handle very large inputs (up to 10^18), so it must not use a loop that iterates up to `n` (which would be infeasible for large values). Instead, derive a closed-form formula using arithmetic series. The function should return a `long long` (or `unsigned long long` if you prefer) because the sum can exceed 32-bit range. Ensure your solution works correctly for edge cases like `n = 1`, `n = 3`, and `n = 15`. The function must be self-contained, use `const` for input parameters where appropriate, and be efficient in both time and space.
// The straightforward loop approach in the snippet sums multiples of 3 and 5 below `n`, but that is O(n) and fails for large `n`. Instead, use the inclusion–exclusion principle: sum all multiples of 3 below `n`, add sum of multiples of 5 below `n`, and subtract sum of multiples of 15 below `n` (since they are counted twice). For a divisor `d` and limit `n`, the number of multiples strictly less than `n` is `m = (n-1) / d` (integer division). The sum of the first `m` multiples of `d` is `d * m * (m+1) / 2` (arithmetic series). This formula works for all `d` and `n`, including when `n <= d` (then `m=0`, sum=0). Handle potential overflow in intermediate multiplication by using `long long` and, if necessary, `__int128` for extra safety, but `long long` suffices for `n <= 10^18` because the total sum is bounded by about `(n^2 * (1/3+1/5-1/15))/2 ~ n^2/6` which for `n=1e18` is ~1.67e35, exceeding 64-bit. So we must note that even `long long` overflows for `n` beyond ~2e9. The task statement says "up to 10^18" but that contradicts 64-bit storage; to keep it realistic, we clarify that the function should work for `n` such that the sum fits in `long long`, i.e., `n < about 2*10^9`. Or we can use `__int128` and return `__int128`, but that complicates tests. For a teaching task, we'll stick to `long long` and mention the limitation. The algorithm is O(1) time and O(1) space. Edge cases: `n=1` → sum=0; `n=3` → only multiples of 3 below 3 are none (since 3 is not strictly less), sum=0; `n=4` → 3 gives 3; `n=10` → 3+5+6+9=23; `n=15` → 3+5+6+9+10+12=45.
#include <cstdint>

// Sum of all natural numbers strictly less than n that are divisible by 3 or 5.
// Uses inclusion-exclusion and arithmetic series for O(1) time and space.
long long sumMultiplesBelow(long long n) {
    if (n <= 1) return 0;  // No positive integers below n

    // Helper lambda to sum multiples of d below n: d * m * (m+1) / 2, where m = (n-1)/d
    auto sumMultiplesOf = [](long long d, long long n) -> long long {
        long long m = (n - 1) / d;  // count of multiples strictly less than n
        // To avoid overflow in d * m * (m+1), use careful ordering but still risk for large n.
        // For n up to about 2e9, long long is safe.
        return d * m * (m + 1) / 2;
    };

    long long sum3 = sumMultiplesOf(3, n);
    long long sum5 = sumMultiplesOf(5, n);
    long long sum15 = sumMultiplesOf(15, n);

    return sum3 + sum5 - sum15;
}
#include <cassert>

int main() {
    // Basic small cases
    assert(sumMultiplesBelow(1) == 0);
    assert(sumMultiplesBelow(3) == 0);
    assert(sumMultiplesBelow(4) == 3);
    assert(sumMultiplesBelow(6) == 8);       // 3 + 5
    assert(sumMultiplesBelow(10) == 23);     // 3+5+6+9
    assert(sumMultiplesBelow(15) == 45);     // 3+5+6+9+10+12
    assert(sumMultiplesBelow(16) == 60);     // adds 15

    // Edge case where n is exactly a multiple of 3 or 5 (should not include n itself)
    assert(sumMultiplesBelow(20) == 78);     // sum below 20: 3+5+6+9+10+12+15+18

    // Larger but safe value for long long
    assert(sumMultiplesBelow(1000) == 233168);

    // n=2 (only 1 below, no multiples)
    assert(sumMultiplesBelow(2) == 0);
}
