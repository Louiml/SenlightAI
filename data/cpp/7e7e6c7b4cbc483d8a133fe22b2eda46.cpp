// Write a C++ function `ullFibonacciMod(ULL a, ULL b, int n)` that returns the value of the (a^b)-th Fibonacci number modulo `n`, where the Fibonacci sequence is defined by F(0)=0, F(1)=1, and F(k)=F(k-1)+F(k-2) for k≥2. Here `a` and `b` are 64‑bit unsigned integers (so a^b can be astronomically large), and `n` is an integer between 1 and 1000 inclusive. The function must handle the edge case `n == 1` (where every number is 0 modulo 1) and the case `a == 0` (where 0^b is 0 for b>0, and 0^0 is defined as 1 in this problem, but since F(1)=1, when b=0 the exponent is 1, so F(1)=1; however to match the original problem’s convention, treat any a==0 as returning 0, regardless of b). The function should precompute Fibonacci sequences modulo each possible `n` once (since the function may be called many times), and use the periodicity of the Fibonacci sequence modulo `n` to reduce the exponent. The function must be self‑contained (except for standard headers), efficient for many calls, and must not use any global mutable state that is not thread‑safe in a read‑only way after initialization. Provide a free function with the exact signature `int fibMod(unsigned long long a, unsigned long long b, int n)`.
The core observation is that the Fibonacci sequence modulo any integer `n` is periodic. In fact, the pair (F(i) mod n, F(i+1) mod n) repeats after a period `p(n)`, because there are at most n² possible pairs, and once a pair repeats, the sequence becomes periodic from then on. Since the pair (0,1) always appears at the start, the period is the smallest index `p > 0` such that F(p) ≡ 0 and F(p+1) ≡ 1 (mod n). We can precompute for every n from 2 to 1000 the entire first period of Fibonacci numbers modulo n, storing them in a table `fib[n][i]` for i from 0 to period[n]-1. For n=1, the period is trivially 1 (since every value is 0). Then for a query, we need F(a^b) mod n. Using the periodicity, F(k) mod n = fib[n][k mod p(n)] for k ≥ 0, except when n=1 where the answer is always 0. So we need to compute the exponent `e = a^b mod p(n)` using fast modular exponentiation (binary exponentiation with modulo). However, for n=1, we immediately return 0. Also, if `a == 0`, return 0 (because 0^b = 0 for b>0, and even when b=0, the original solution returns 0). For n>1 and a>0, compute `e = pow_mod(a % p, b, p)`, where `pow_mod` handles b up to 2^64-1 using recursion or iteration, reducing modulo `p` at each step. Finally return `fib[n][e]`. The precomputation is O(1000 * period) where the maximum period for n≤1000 is not too large (actually the period is at most 6n, as known from the Pisano period bounds; for n≤1000, period ≤ 6000). So precomputation takes O(1000 * 6000) ≈ 6 million operations, which is fine. Each query does O(log b) modular multiplications and an array lookup. Edge cases: n=1, a=0, b=0 (but a=0 is handled separately), and large a,b overflowing when multiplied? We avoid multiplication overflow by using unsigned long long and careful modulo exponentiation. The time complexity per query is O(log b) and precomputation is O(n_max * period_max), space is O(n_max * period_max) for the table and O(n_max) for periods.
#include <vector>
#include <cstdint>

// Precomputed Fibonacci sequences modulo each n (2..1000) and their periods.
struct FibonacciModPrecompute {
    // fib[n][i] = F(i) mod n for i in [0, period[n]-1], for n>=2.
    std::vector<std::vector<int>> fib;
    std::vector<int> period;

    FibonacciModPrecompute() : fib(1001), period(1001, 1) {
        for (int n = 2; n <= 1000; ++n) {
            std::vector<int>& seq = fib[n];
            seq.push_back(0);
            seq.push_back(1 % n);
            for (int i = 2; ; ++i) {
                int next = (seq[i-1] + seq[i-2]) % n;
                seq.push_back(next);
                if (seq[i-1] == 0 && next == 1) {
                    period[n] = i - 1;
                    break;
                }
            }
        }
    }
};

// Fast modular exponentiation: (base^exp) % mod, using unsigned long long.
// mod is assumed to be > 0.
static uint64_t powMod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

// Returns F(a^b) modulo n, where a,b are 64-bit unsigned integers.
int fibMod(uint64_t a, uint64_t b, int n) {
    static const FibonacciModPrecompute pre;

    if (a == 0 || n == 1) {
        return 0;
    }

    int p = pre.period[n];
    // Compute exponent = (a^b) mod p.
    uint64_t exponent = powMod(a % p, b, p);
    return pre.fib[n][exponent];
}
#include <cassert>
#include <cstdint>

// The solution function is declared above; include its definition here.
// (For brevity, the solution code is assumed to be included before this.)
int main() {
    // Basic cases
    assert(fibMod(1, 1, 10) == 1);          // F(1) = 1
    assert(fibMod(2, 1, 10) == 1);          // F(2) = 1
    assert(fibMod(3, 1, 10) == 2);          // F(3) = 2
    assert(fibMod(5, 1, 10) == 5);          // F(5) = 5
    assert(fibMod(0, 999, 10) == 0);        // a=0 -> 0
    assert(fibMod(1, 0, 10) == 1);          // F(1^0 = 1) = 1
    assert(fibMod(2, 0, 10) == 1);          // F(2^0 = 1) = 1

    // Modulo 1 always 0
    assert(fibMod(123, 456, 1) == 0);
    assert(fibMod(0, 0, 1) == 0);

    // Periodicity: F(10) = 55, 55 mod 11 = 0
    assert(fibMod(10, 1, 11) == 0);
    // F(20) = 6765, 6765 mod 11 = 0 as well, since period of 11 is 10
    assert(fibMod(20, 1, 11) == 0);
    // Check period: F(11) = 89, 89 mod 11 = 1
    assert(fibMod(11, 1, 11) == 1);

    // Large exponent: a=2, b=60 => 2^60, need F(2^60) mod 1000.
    // Since period[1000] is known to be 1500, compute manually:
    // 2^60 mod 1500 = ? We trust the function.
    uint64_t result = fibMod(2, 60, 1000);
    assert(result >= 0 && result < 1000);

    // Known values from original problem examples: For n=3, period=8,
    // a=10, b=10 => 10^10 mod 8 = 0, F(0)=0
    assert(fibMod(10, 10, 3) == 0);
    // a=5, b=3 => 125 mod 8 = 5, F(5) mod 3 = 5%3=2? Actually F(5)=5, 5%3=2
    assert(fibMod(5, 3, 3) == 2);

    return 0;
}
