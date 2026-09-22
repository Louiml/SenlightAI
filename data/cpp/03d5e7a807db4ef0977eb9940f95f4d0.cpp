/*
Given a positive integer `n` (2 ≤ n ≤ 10^6), write a C++ function `int sumOfRadicals(int n)` that returns the sum of the "radical products" obtained by repeatedly extracting one copy of each distinct prime factor of `n` until all prime factors are exhausted. More precisely: factorize `n` into its prime factors with exponents. Then, in each round, for every distinct prime factor that still has positive remaining exponent, multiply those primes together to get a product `p` (the radical of the remaining distinct primes), decrement each of their exponents by 1, and add `p` to the total. Continue until all exponents reach zero. The function should return the total sum. For example, for `n = 12 = 2^2 * 3^1`, the first round yields `2*3=6`, the second round yields `2` (since only 2 remains), total = 8. For `n = 30 = 2*3*5`, only one round yields `30`. For `n = 8 = 2^3`, yields `2+2+2=6`.
*/

#include <map>
#include <cmath>

// Returns the sum of radical products obtained by repeatedly
// extracting one copy of each distinct prime factor of n.
int sumOfRadicals(int n) {
    int original = n;
    std::map<int, int> factors; // prime -> remaining exponent
    
    // Factorize n using trial division
    for (int p = 2; p * p <= original; ++p) {
        if (n % p == 0) {
            int count = 0;
            while (n % p == 0) {
                n /= p;
                ++count;
            }
            factors[p] = count;
        }
    }
    if (n > 1) {
        factors[n] = 1;
    }
    
    // Determine maximum exponent
    int maxExp = 0;
    for (const auto& entry : factors) {
        if (entry.second > maxExp) {
            maxExp = entry.second;
        }
    }
    
    int result = 0;
    for (int round = 0; round < maxExp; ++round) {
        int product = 1;
        for (auto& entry : factors) {
            if (entry.second > 0) {
                product *= entry.first;
                --entry.second;
            }
        }
        result += product;
    }
    
    return result;
}

#include <cassert>
#include <iostream>

// Assume sumOfRadicals is defined above.

int main() {
    // 2^1 -> 2
    assert(sumOfRadicals(2) == 2);
    // 7^1 -> 7
    assert(sumOfRadicals(7) == 7);
    // 2^2 * 3^1 = 12 -> (2*3)+(2)=8
    assert(sumOfRadicals(12) == 8);
    // 2^3 = 8 -> 2+2+2=6
    assert(sumOfRadicals(8) == 6);
    // 2*3*5 = 30 -> 30
    assert(sumOfRadicals(30) == 30);
    // 2^2 * 3^2 = 36 -> (2*3)+(2*3)=12
    assert(sumOfRadicals(36) == 12);
    // 2^3 * 3^2 = 72 -> (2*3)+(2*3)+(2)=14
    assert(sumOfRadicals(72) == 14);
    // 2^1 * 3^1 * 5^1 = 30 (already tested) but check 210 = 2*3*5*7
    assert(sumOfRadicals(210) == 210);
    // 2^4 = 16 -> 2+2+2+2=8
    assert(sumOfRadicals(16) == 8);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// We factorize `n` using trial division up to sqrt(n), storing each distinct prime factor and its exponent in a map (or vector of pairs). The key insight is that the number of rounds is the maximum exponent among all primes (since each round removes one exponent from every prime that still has a positive exponent). For each round from 0 to maxExp-1, we iterate through all distinct primes; if its current remaining exponent is > 0, we multiply it into a running product and decrement that exponent. After processing all primes, we add the product to the result. Edge cases: prime numbers (like n=7) have only one round with product = n; perfect powers (like n=8) have multiple rounds each producing the prime (if a single prime); numbers like 72 = 2^3 * 3^2 have maxExp=3, rounds: (2*3)=6, then (2*3)=6, then (2)=2, total 14. Since n ≤ 10^6, factorization is O(sqrt(n)), and each round processes at most ~7 distinct primes (since 2*3*5*7*11*13*17 > 1e6), with at most 19 rounds (since 2^19 < 1e6). So time complexity is O(sqrt(n) + distinctPrimes * maxExp) ≈ O(sqrt(n)), which is fine. Space O(distinctPrimes).
