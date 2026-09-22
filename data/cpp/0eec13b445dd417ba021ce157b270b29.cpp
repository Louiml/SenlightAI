Write a C++ function named `goldbachDecomposition` that takes a single positive even integer `n` (with `n >= 4`) and returns a `std::string` containing three prime numbers separated by single spaces, such that their sum equals `n`. The output must be in the format `"p1 p2 p3"` where `p1`, `p2`, `p3` are prime numbers and `p1 + p2 + p3 == n`. If multiple valid triples exist, return the one found by the following deterministic rule: first, try the triple `(2, 2, n-4)` if `n-4` is prime. If that fails, search for a triple `(2, p, q)` with `p` and `q` odd primes, choosing the smallest possible `p` (and then the smallest possible `q`). If still no such triple exists (which is impossible for even `n >= 4` but handle gracefully), return `"0 0 0"`. The function must handle all even inputs from 4 up to 10^6 efficiently.

The problem is a three-prime Goldbach partition of an even integer. Since every even number ≥ 4 can be expressed as the sum of two primes (Goldbach's conjecture, verified for this range), and 2 is prime, one classic reduction is: if `n-2` is prime, then `(2, 2, n-4)` works if `n-4` is prime; otherwise, we can try `(2, p, q)` where `p` and `q` are odd primes summing to `n-2`. The search: for each odd `p` from 3 upward, check if `p` is prime and if `n-2-p` is also prime and ≥3 (odd). To make this efficient for up to 10^6, precompute a boolean `isPrime` array using a sieve (e.g., Sieve of Eratosthenes) once per function call (or maintain static cache). This gives O(n log log n) for sieve plus O(n) worst-case for the search, but due to Goldbach, the search terminates very early. Edge cases: n=4 → `(2,2,0)` invalid because 0 not prime, but `2+2+2=6` not 4; actually for n=4, only triple `(2,? ,?)`? Wait 2+2+0 invalid, 2+?+? with primes: 2+?+? =4 means sum of two primes =2, which is only 1+1 but 1 not prime. However Goldbach's three-prime theorem states every odd number >5 is sum of three primes, but for even numbers, we can always write n = 2 + (n-2) where (n-2) even, and by Goldbach (n-2) is sum of two primes. So for n=4, n-2=2, which is sum of two primes? 2 = 1+1 (1 not prime) or 0+2 (0 not). So n=4 cannot be expressed as sum of three primes? Actually 2+2+? =4 → ?=0. So no solution. But the problem statement says "even n >=4" – we should handle n=4 gracefully by returning "0 0 0". For n≥6, it's always possible. So the function should check n=4 specially. Time: sieve O(n log log n) and search O(n). Space: O(n) for sieve.

#include <string>
#include <vector>

// Returns a string "p1 p2 p3" such that p1+p2+p3 == n and all are prime.
// For n == 4, returns "0 0 0" as no triple exists.
// For n >= 6, always returns a valid triple.
std::string goldbachDecomposition(int n) {
    if (n == 4) {
        return "0 0 0";
    }

    // Sieve up to n (inclusive)
    std::vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; i * i <= n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) {
                isPrime[j] = false;
            }
        }
    }

    // Try (2, 2, n-4)
    if (n - 4 >= 2 && isPrime[n - 4]) {
        return "2 2 " + std::to_string(n - 4);
    }

    // Try (2, p, q) with p and q odd primes, smallest p first.
    // Since n is even, n-2 is even, need p+q = n-2.
    for (int p = 3; p <= n - 2; p += 2) {
        if (isPrime[p]) {
            int q = n - 2 - p;
            if (q >= 3 && (q & 1) && isPrime[q]) {
                return "2 " + std::to_string(p) + " " + std::to_string(q);
            }
        }
    }

    // Should never get here for n >= 6 due to Goldbach's conjecture.
    return "0 0 0";
}

#include <cassert>
#include <string>

// Forward declaration of the function under test.
std::string goldbachDecomposition(int n);

// Helper to check that the output is a valid triple of primes summing to n.
bool validTriple(int n, const std::string& output) {
    if (output == "0 0 0") {
        return n == 4; // Only n=4 should have no solution.
    }
    int a, b, c;
    sscanf(output.c_str(), "%d %d %d", &a, &b, &c);
    auto isPrime = [&](int x) {
        if (x < 2) return false;
        for (int i = 2; i * i <= x; ++i) if (x % i == 0) return false;
        return true;
    };
    return isPrime(a) && isPrime(b) && isPrime(c) && (a + b + c == n);
}

int main() {
    // Basic even numbers
    assert(validTriple(6, goldbachDecomposition(6)));
    assert(validTriple(10, goldbachDecomposition(10)));

    // n=4 edge case
    assert(goldbachDecomposition(4) == "0 0 0");

    // Larger even numbers
    assert(validTriple(100, goldbachDecomposition(100)));
    assert(validTriple(1000, goldbachDecomposition(1000)));

    // Very large within limit
    assert(validTriple(1000000, goldbachDecomposition(1000000)));

    // Specific known triple for 8: 2+3+3=8
    // Our function: n-4=4 not prime, then p=3, q=8-2-3=3 → "2 3 3"
    assert(goldbachDecomposition(8) == "2 3 3");

    // For n=12: n-4=8 not prime; p=3, q=12-2-3=7 → "2 3 7"
    assert(goldbachDecomposition(12) == "2 3 7");

    // For n=20: n-4=16 not prime; p=3, q=20-2-3=15 not prime; p=5, q=20-2-5=13 prime → "2 5 13"
    assert(goldbachDecomposition(20) == "2 5 13");

    return 0;
}
