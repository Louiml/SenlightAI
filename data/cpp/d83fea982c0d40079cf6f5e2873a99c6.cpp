Write a C++ function `long long countGoodNumbers(long long n)` that returns the number of good digit strings of length `n` modulo 1’000’000’007. A digit string is considered good if at every even index (0-based) the digit is even (0, 2, 4, 6, 8) and at every odd index the digit is prime (2, 3, 5, 7). The input `n` is a positive integer (1 ≤ n ≤ 10^12). Use fast modular exponentiation to compute the answer efficiently, since `n` can be extremely large.
The key insight is that positions are independent: each even position has exactly 5 choices (even digits), and each odd position has exactly 4 choices (prime digits). For a string of length `n`, the number of even positions is `(n + 1) / 2` (indices 0, 2, 4, …), and the number of odd positions is `n / 2`. The total number of good strings is `5^(even_count) * 4^(odd_count)`. Since `n` can be up to 10^12, direct multiplication is infeasible; instead, use binary exponentiation with modular arithmetic at each step to keep values within `long long` range. Base cases: when `n = 1`, there is 1 even position and 0 odd positions, so the answer is 5. When `n = 2`, there is 1 even and 1 odd, which gives 5 * 4 = 20. The time complexity is O(log n) for exponentiation, and space complexity is O(1). The only edge case is `n` being odd vs. even, handled by the formulas for even/odd counts.
#include <cstdint>

// Count good digit strings of length n modulo 1e9+7.
// Even positions (0-indexed) must be even digits, odd positions must be prime digits.
int64_t countGoodNumbers(const int64_t n) {
    const int64_t MOD = 1000000007LL;
    const int64_t even_positions = (n + 1) / 2;  // indices 0, 2, 4, ...
    const int64_t odd_positions = n / 2;         // indices 1, 3, 5, ...

    // Fast modular exponentiation: base^exp % MOD
    auto mod_pow = [](const int64_t base, int64_t exp, const int64_t mod) {
        int64_t result = 1;
        int64_t current_base = base % mod;
        while (exp > 0) {
            if (exp % 2 == 1) {
                result = (result * current_base) % mod;
            }
            current_base = (current_base * current_base) % mod;
            exp /= 2;
        }
        return result;
    };

    const int64_t even_choices = 5; // {0,2,4,6,8}
    const int64_t odd_choices = 4;  // {2,3,5,7}

    int64_t answer = mod_pow(even_choices, even_positions, MOD);
    answer = (answer * mod_pow(odd_choices, odd_positions, MOD)) % MOD;
    return answer;
}
#include <cassert>

int main() {
    // n = 1: 5 choices for the single even position
    assert(countGoodNumbers(1) == 5);
    // n = 2: 5 * 4 = 20
    assert(countGoodNumbers(2) == 20);
    // n = 3: positions 0(even),1(odd),2(even) => 5*4*5 = 100
    assert(countGoodNumbers(3) == 100);
    // n = 4: 2 even and 2 odd => 5^2 * 4^2 = 400
    assert(countGoodNumbers(4) == 400);
    // n = 5: 3 even, 2 odd => 5^3 * 4^2 = 125 * 16 = 2000
    assert(countGoodNumbers(5) == 2000);
    // n = 7: 4 even, 3 odd => 5^4 * 4^3 = 625 * 64 = 40000
    assert(countGoodNumbers(7) == 40000);
    // n = 10: 5 even, 5 odd => 5^5 * 4^5 = 3125 * 1024 = 3200000
    assert(countGoodNumbers(10) == 3200000);
    // Large n: verify result is within modulus and non-zero
    int64_t result_large = countGoodNumbers(1000000000000LL);
    assert(result_large >= 0 && result_large < 1000000007LL);
    // Known modulo result for n = 20: compute via direct formula (no overflow) for sanity
    int64_t expected = 1;
    for (int i = 0; i < 10; ++i) expected = (expected * 5) % 1000000007LL;
    for (int i = 0; i < 10; ++i) expected = (expected * 4) % 1000000007LL;
    assert(countGoodNumbers(20) == expected);
    return 0;
}
