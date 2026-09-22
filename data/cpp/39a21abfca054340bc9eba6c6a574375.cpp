// Write a C++ function that takes a positive integer `n` and returns the count of integers from 1 to `n` that have exactly one proper divisor (i.e., exactly one divisor strictly between 1 and the number itself). For example, for `n = 10`, the numbers 4, 6, 8, 9, and 10 each have exactly one proper divisor (2 for 4, 2 and 3 for 6 — wait, 6 has proper divisors 2 and 3, so it has two, not one). Actually, the correct interpretation is: count numbers `i` in `[1, n]` such that the number of divisors of `i` is exactly 3 (because a number with exactly one proper divisor has two trivial divisors 1 and itself, plus one more, totaling 3 divisors). For `n = 10`, numbers with exactly 3 divisors are: 4 (divisors 1,2,4), 9 (1,3,9) — so count is 2. But the given code counts something else: it computes the sum of the number of divisors for each `i` from 1 to `n`, and outputs that sum. So the task should match the code: write a function that, given `n`, returns the sum of the number of divisors of all integers from 1 to `n`. For example, `n=4` gives divisor counts: 1→1, 2→2, 3→2, 4→3, sum = 8. The function should be efficient enough for `n` up to 10^7, but the given code uses a sieve-like O(n log n) approach. The task must be standalone and self-contained.
The problem is to compute the sum of the number of divisors for all integers from 1 to `n`. The straightforward approach is to initialize a vector `divCount` of size `n+1` with zeros, then for each `i` from 1 to `n`, add 1 to every multiple of `i` (i.e., for `j = i; j <= n; j += i` increment `divCount[j]`). After this sieve-like loop, `divCount[x]` contains the exact number of divisors of `x`. Then sum all values from index 1 to `n` and return that sum. This runs in O(n log n) time because the inner loop runs about n/1 + n/2 + ... + n/n = n * H_n ≈ n log n. Space is O(n). Edge cases: `n = 1` returns 1 (only 1 has 1 divisor), `n = 0` is not expected but could return 0. The sum can grow large; for `n=10^7`, the sum is approximately n log n + (2γ−1)n ≈ 10^7 * 16.1 ≈ 1.6e8, which fits in a 64-bit integer, so use `long long`. The function should use `const` where appropriate and return a `long long`.
#include <vector>
#include <cstdint>

// Sum of the number of divisors of all integers from 1 to n.
long long sumOfDivisorCounts(int n) {
    if (n <= 0) return 0;
    std::vector<int> divCount(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        for (int j = i; j <= n; j += i) {
            ++divCount[j];
        }
    }
    long long result = 0;
    for (int i = 1; i <= n; ++i) {
        result += divCount[i];
    }
    return result;
}
#include <cassert>

int main() {
    assert(sumOfDivisorCounts(1) == 1);
    assert(sumOfDivisorCounts(2) == 3);  // 1+2=3
    assert(sumOfDivisorCounts(3) == 5);  // 1+2+2=5
    assert(sumOfDivisorCounts(4) == 8);  // 1+2+2+3=8
    assert(sumOfDivisorCounts(5) == 10); // 1+2+2+3+2=10
    assert(sumOfDivisorCounts(6) == 14); // 1+2+2+3+2+4=14
    assert(sumOfDivisorCounts(10) == 27); // 1+2+2+3+2+4+2+4+3+4=27
    assert(sumOfDivisorCounts(100) == 482);
    assert(sumOfDivisorCounts(1000) == 7069);
    assert(sumOfDivisorCounts(0) == 0);
    return 0;
}
