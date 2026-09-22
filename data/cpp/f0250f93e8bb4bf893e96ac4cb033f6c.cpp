/*
Write a C++ function `bool dividesFactorialRange(int a, int b, int c, int d)` that determines whether the product of all integers in the range `[c, d]` (inclusive) is divisible by the product of all integers in the range `[a, b]` (inclusive). The ranges satisfy `1 ≤ a < b ≤ 10^7`, `1 ≤ c < d ≤ 10^7`. The function should return `true` if `(c * (c+1) * ... * d)` is divisible by `(a * (a+1) * ... * b)`, and `false` otherwise. You may assume the input ranges are valid (a < b, c < d) and positive. The function must be efficient for the given upper bound.
*/

#include <bitset>
#include <vector>
#include <cstdint>

constexpr int MAX_N = 10'000'000;

class PrimeCounter {
private:
    std::bitset<MAX_N + 1> isComposite;
    std::vector<int> primes;

    void sieve() {
        isComposite[0] = isComposite[1] = true;
        for (int i = 2; i <= MAX_N; ++i) {
            if (!isComposite[i]) primes.push_back(i);
            for (int p : primes) {
                if (i * p > MAX_N) break;
                isComposite[i * p] = true;
                if (i % p == 0) break;
            }
        }
    }

    // Count total exponent of prime p in product of all integers in [1, x]
    long long countExponentUpTo(int x, int p) const {
        long long count = 0;
        long long power = p;
        while (power <= x) {
            count += x / power;
            if (power > MAX_N / p) break; // avoid overflow
            power *= p;
        }
        return count;
    }

public:
    PrimeCounter() { sieve(); }

    // Count exponent of prime p in the product of integers in [l, r]
    long long countExponentInRange(int l, int r, int p) const {
        if (l > r) return 0;
        return countExponentUpTo(r, p) - countExponentUpTo(l - 1, p);
    }

    // Check if product [c, d] is divisible by product [a, b]
    bool divides(int a, int b, int c, int d) const {
        for (int p : primes) {
            if (p > b && p > d) break; // no need to check further
            long long numerator = countExponentInRange(c, d, p);
            long long denominator = countExponentInRange(a, b, p);
            if (numerator < denominator) return false;
        }
        return true;
    }
};

#include <cassert>
#include "prime_counter.h" // Assume the above solution is in this header

int main() {
    PrimeCounter pc;
    // Simple cases
    assert(pc.divides(2, 3, 1, 6) == true);  // 6 divides 720
    assert(pc.divides(4, 5, 1, 4) == false); // 20 does NOT divide 24
    assert(pc.divides(1, 1, 2, 2) == true);  // 1 divides 2
    assert(pc.divides(10, 10, 5, 9) == false); // 10 does not divide 5*6*7*8*9
    // Larger but manageable cases
    assert(pc.divides(100, 100, 90, 110) == true); // 100 | 90*...*110 (since 110! includes 100)
    assert(pc.divides(100, 100, 95, 99) == false); // 100 = 2^2*5^2, range lacks factors of 5
    // Edge: ranges starting at 1
    assert(pc.divides(1, 5, 1, 5) == true);  // equal ranges
    assert(pc.divides(5, 5, 1, 4) == false); // 5 does not divide 24
    // Cross-check with a brute force for small numbers
    for (int a = 1; a <= 8; ++a) {
        for (int b = a; b <= 8; ++b) {
            for (int c = 1; c <= 8; ++c) {
                for (int d = c; d <= 8; ++d) {
                    long long num = 1, den = 1;
                    for (int i = c; i <= d; ++i) num *= i;
                    for (int i = a; i <= b; ++i) den *= i;
                    bool expected = (num % den == 0);
                    bool actual = pc.divides(a, b, c, d);
                    assert(actual == expected);
                }
            }
        }
    }
    return 0;
}

// The core idea is to compare the prime factorizations of the two products. Instead of computing the actual products (which would overflow), we count the exponent of each prime in both products and check that for every prime, the exponent in the numerator (`[c,d]`) is at least the exponent in the denominator (`[a,b]`). To count exponents efficiently, we precompute all primes up to the maximum possible bound (10^7) using a linear sieve. For a given range `[l, r]`, the exponent of prime `p` in the product `l * (l+1) * ... * r` is the sum over `k ≥ 1` of `floor(r / p^k) - floor((l-1) / p^k)`. This works because for each power `p^k`, every multiple of `p^k` within the range contributes one factor of `p`. We compute this for both ranges and compare counts. Edge cases include ranges that start at 1 (where `l-1 = 0` works fine) and very large primes that only appear once. Time complexity: sieve is O(N log log N) but with the linear sieve it's O(N), and for each query we iterate over all primes up to the max bound, doing O(log_p N) work each, so total per query is O(π(N) * log N) ≈ O(N / log N * log N) = O(N) in the worst case, but since N is fixed at 10^7, this is acceptable. Space complexity is O(N) for the sieve bitset and O(π(N)) for the prime list.
