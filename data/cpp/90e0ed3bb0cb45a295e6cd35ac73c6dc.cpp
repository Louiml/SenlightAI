Write a C++ function `int nthPrime(int n)` that returns the nth prime number, where the first prime number is 2 (n=1), the second is 3 (n=2), and so on. The function must handle `n >= 1` and should return `-1` for invalid input (`n < 1`). Use a trial-division primality check (similar to the snippet’s `isPrime` logic) to test each candidate number. The function must be efficient enough to compute the 1000th prime (which is 7919) within a reasonable time, and it must be `const`-correct (no modification of inputs, no side effects). Do not use a sieve or any precomputed list — the solution must generate primes on the fly.
// The solution repeatedly tests consecutive integers starting at 2, counting how many are prime, until the count reaches `n`. To test each candidate `x`, we check divisibility by all integers from 2 up to `floor(sqrt(x))`. If any divides evenly, `x` is composite; otherwise it’s prime. The first prime encountered is 2 (n=1), then 3 (n=2), then 5 (n=3), etc. Edge cases: `n < 1` returns -1. The candidate loop increments by 1, but since 2 is the only even prime, we could optimize by checking 2 separately and then only odd numbers, but that's optional. Complexity: For the k-th prime p_k, the primality test runs in O(sqrt(p_k)) time. The total time for nthPrime(n) is O(Σ_{k=1..n} sqrt(p_k)), which is approximately O(n * sqrt(n log n)). Space is O(1) auxiliary. The function is `const`-correct because it takes `n` by value and does not modify any global state.
#include <cmath>

// Returns the nth prime number (1-indexed). Returns -1 if n < 1.
int nthPrime(int n) {
    if (n < 1) {
        return -1;
    }

    int count = 0;
    int candidate = 2;

    while (count < n) {
        // Check if candidate is prime
        bool prime = true;
        int limit = static_cast<int>(std::sqrt(candidate));
        for (int divisor = 2; divisor <= limit; ++divisor) {
            if (candidate % divisor == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            ++count;
            if (count == n) {
                return candidate;
            }
        }
        ++candidate;
    }
    return -1; // Should never reach here for valid n
}
#include <cassert>

int main() {
    assert(nthPrime(1) == 2);
    assert(nthPrime(2) == 3);
    assert(nthPrime(3) == 5);
    assert(nthPrime(4) == 7);
    assert(nthPrime(5) == 11);
    assert(nthPrime(10) == 29);
    assert(nthPrime(100) == 541);
    assert(nthPrime(1000) == 7919);
    assert(nthPrime(0) == -1);
    assert(nthPrime(-5) == -1);
    return 0;
}
