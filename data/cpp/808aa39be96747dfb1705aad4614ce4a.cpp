Write a C++ function named `nextMultipleOfThree` that takes a single non-negative integer `n` and returns the smallest integer greater than or equal to `n` that is divisible by 3. For example, for `n = 5`, the answer is `6`; for `n = 6`, the answer is `6`; for `n = 7`, the answer is `9`. The input `n` can be as large as 10^18, so use an appropriate 64-bit integer type. The function must handle the edge case where `n` itself is already a multiple of 3, returning `n` unchanged.
// The simplest approach is to check the three candidates: `n`, `n+1`, and `n+2`. Among these, exactly one is divisible by 3, because consecutive integers cycle through remainders 0, 1, 2 modulo 3. So we test each candidate in order and return the first one that is divisible by 3. This works for all non-negative integers and correctly handles the case where `n` is already a multiple of 3 (candidate `n` is returned), and for `n` slightly off by 1 or 2, we add the needed 1 or 2. An alternative formula `n + ((3 - n % 3) % 3)` also works, but the three-step check is clearer and less error-prone. Because `n` can be up to 10^18, use `long long` (64-bit). The time complexity is O(1) and space complexity is O(1). Edge cases include `n = 0` (returns 0), and very large values where `n+2` stays within 64-bit range (max 10^18+2, which is safe).
#include <cstdint>

// Returns the smallest integer >= n that is divisible by 3.
// The input n must be a non-negative 64-bit integer.
long long nextMultipleOfThree(long long n) {
    // Check n, n+1, n+2 in order; exactly one is divisible by 3.
    if (n % 3 == 0) {
        return n;
    }
    if ((n + 1) % 3 == 0) {
        return n + 1;
    }
    // If neither n nor n+1 works, then n+2 must be divisible by 3.
    return n + 2;
}
#include <cassert>

int main() {
    assert(nextMultipleOfThree(0) == 0);
    assert(nextMultipleOfThree(1) == 3);
    assert(nextMultipleOfThree(2) == 3);
    assert(nextMultipleOfThree(3) == 3);
    assert(nextMultipleOfThree(4) == 6);
    assert(nextMultipleOfThree(5) == 6);
    assert(nextMultipleOfThree(6) == 6);
    assert(nextMultipleOfThree(7) == 9);
    assert(nextMultipleOfThree(8) == 9);
    assert(nextMultipleOfThree(9) == 9);
    assert(nextMultipleOfThree(1000000000000000000LL) == 1000000000000000002LL); // 10^18 is not divisible by 3 (sum digits = 1), so next is +2
    assert(nextMultipleOfThree(999999999999999999LL) == 999999999999999999LL); // divisible by 3
    return 0;
}
