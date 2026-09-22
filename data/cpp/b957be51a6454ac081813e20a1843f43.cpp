// Given two positive integers `n` and `k` (`1 <= n, k <= 10^9`), write a C++ function that computes the value of `(3*n + k) / 2` using integer division (truncation toward zero). The function must not use floating-point arithmetic or built-in division by 2 (i.e., you cannot simply write `(3*n+k)/2`); instead, implement the computation using bitwise operations and addition/multiplication only. The input values may be large, so use a 64-bit type. The function should return the quotient as a `long long`.
// The expression `3*n + k` can be computed directly using a 64-bit type to avoid overflow. To divide by 2 without using `/ 2` or `>> 1`? Actually the task says "must not use floating-point arithmetic or built-in division by 2" – but bitwise right shift is acceptable because it's not "built-in division". However, to be safe, we'll compute the division by 2 using a bitwise right shift (`>> 1`), which is a standard integer operation. Alternatively, we could use repeated subtraction, but that would be inefficient for large values. The key is to handle large `n` and `k` correctly: since `n` and `k` are up to 1e9, `3*n + k` can reach about 4e9, which fits in `long long`. The algorithm is straightforward: compute `sum = 3LL * n + k`, then `result = sum >> 1`. Edge cases: when `sum` is odd, integer division truncates, so `3` becomes `1`, etc. Time complexity is O(1), space O(1). No special edge cases beyond overflow prevention.
#include <cstdint>

// Computes (3*n + k) / 2 using bitwise shift for division by 2.
// n and k are positive integers up to 1e9.
long long computeQuotient(long long n, long long k) {
    long long sum = 3LL * n + k; // 64-bit to avoid overflow
    return sum >> 1;             // integer division by 2 (truncation)
}
#include <cassert>

int main() {
    assert(computeQuotient(1, 1) == (3LL*1+1)/2); // 2
    assert(computeQuotient(2, 3) == (3LL*2+3)/2); // 4
    assert(computeQuotient(1000000000, 1000000000) == (3LL*1000000000+1000000000)/2); // 2000000000
    assert(computeQuotient(5, 0) == (3LL*5+0)/2); // 7
    assert(computeQuotient(100, 1) == (3LL*100+1)/2); // 150
    assert(computeQuotient(3, 2) == (3LL*3+2)/2); // 5
}
