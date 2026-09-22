// Given a positive integer `n` (where `1 <= n <= 10^9`), write a C++ function `ll countSequences(ll n)` that returns the number of ways to represent `n` as a sum of one or more positive integers, where the order of the summands matters (i.e., compositions of `n`). For example, for `n = 3`, the valid sequences are `1+1+1`, `1+2`, `2+1`, `3` — so the answer is `4`. Since the result can be very large, return the answer modulo `123456789`. The function must be efficient enough to handle `n` up to `10^9` (so you cannot use an O(n) dynamic programming loop). Note that the number of compositions of `n` is known to be `2^(n-1)` for `n >= 1`.
#include <cassert>

// Assume countSequences is already defined above.

int main() {
    // n = 1 -> only [1]
    assert(countSequences(1) == 1);
    // n = 2 -> [1+1], [2] => 2
    assert(countSequences(2) == 2);
    // n = 3 -> 4 compositions
    assert(countSequences(3) == 4);
    // n = 4 -> 8 compositions
    assert(countSequences(4) == 8);
    // n = 10 -> 2^9 = 512
    assert(countSequences(10) == 512);
    // Check modulo for a larger n: compute 2^19 mod 123456789 manually via powmod
    // 2^19 = 524288, 524288 < 123456789, so result is 524288
    assert(countSequences(20) == 524288);
    // n = 31 -> 2^30 = 1073741824; modulo 123456789 = 1073741824 % 123456789 = 1073741824 - 8*123456789 = 1073741824 - 987654312 = 86087512
    assert(countSequences(31) == (1073741824LL % 123456789LL));
    // n = 1,000,000,000 (just check it doesn't overflow and returns within modulus)
    ll big = countSequences(1000000000LL);
    assert(big >= 0 && big < MOD);
    return 0;
}
#include <cstdint>

using ll = long long;
const ll MOD = 123456789;

// Return the number of compositions of n modulo MOD, where n >= 1.
// The answer is 2^(n-1) modulo MOD.
ll countSequences(ll n) {
    if (n == 1) {
        return 1; // 2^0 = 1
    }
    // Fast exponentiation: compute base^exp % MOD
    ll base = 2;
    ll exp = n - 1;
    ll result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp /= 2;
    }
    return result;
}
// The problem asks for the number of compositions (ordered partitions) of a positive integer `n`. A well-known combinatorial identity states that the number of compositions of `n` is exactly `2^(n-1)`. This can be proven by considering the `n-1` gaps between the `n` ones in a sum; each gap can either be a separator (breaking into a new summand) or not, giving `2^(n-1)` possible outcomes. For `n = 1`, the formula gives `2^0 = 1`, which matches the single sequence `[1]`. Because `n` can be as large as `10^9`, we must compute `2^(n-1) mod 123456789` using fast exponentiation (binary exponentiation) in `O(log n)` time. The modulo value is not necessarily prime, but fast exponentiation works for any modulus because it only uses multiplication and modulo operations. Edge cases include `n = 1` (returns 1) and `n = 0` (if the function were called with 0, the formula would give `2^(-1)`, which is invalid; but the problem guarantees `n >= 1`). The time complexity is `O(log n)` due to exponentiation, and space complexity is `O(1)`.
