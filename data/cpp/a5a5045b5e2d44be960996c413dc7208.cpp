/*
Write a C++ function `vector<long long> factorGrouping(long long n)` that, given a positive integer `n`, returns a sequence of integers such that their product equals `n`, the sequence has the maximum possible length, and among all maximum-length sequences, the integers are chosen in the mathematically simplest grouped form: first, factor `n` into prime powers. Then, sort the prime factor exponents in non-increasing order. If the largest exponent is `k`, the output sequence has exactly `k` integers, each initially 1. For every prime factor `p` with exponent `e`, multiply the last `e` integers of this sequence by `p`. Return the sequence in the order produced. For example, for `n = 12 = 2^2 * 3^1`, the exponents are {2,1}, so `k=2`; the sequence starts as [1,1], multiply last 2 by 2 → [2,2], then multiply last 1 by 3 → [2,6]. The product is 12, length is 2 (maximum possible because no sequence of length 3 can have product 12 with positive integers unless one is 1, but 1 is not allowed? Actually, the problem implicitly assumes integers > 1? The code snippet uses all 1s then multiplies, but the final elements are all >1, so output length is the maximum number of factors >1, which equals the largest exponent). The function must handle `n=1` by returning a vector containing just `{1}` (length 1). You do not need to worry about overflow, but use `long long`.
*/

#include <vector>
#include <algorithm>
#include <cmath>

// Returns a maximum-length sequence of integers >1 whose product equals n.
// For n=1, returns {1}.
std::vector<long long> factorGrouping(long long n) {
    if (n == 1) {
        return {1};
    }

    // Store pairs of (exponent, prime)
    std::vector<std::pair<long long, long long>> factors;
    long long temp = n;
    for (long long p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            long long exp = 0;
            while (temp % p == 0) {
                ++exp;
                temp /= p;
            }
            factors.push_back({exp, p});
        }
    }
    if (temp > 1) {
        factors.push_back({1, temp});
    }

    // Sort by descending exponent
    std::sort(factors.begin(), factors.end(), 
              [](const auto& a, const auto& b) { return a.first > b.first; });

    long long k = factors[0].first;
    std::vector<long long> ans(k, 1);
    for (const auto& [exp, prime] : factors) {
        for (long long i = k - exp; i < k; ++i) {
            ans[i] *= prime;
        }
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above (factorGrouping).

int main() {
    // Basic examples
    assert((factorGrouping(12) == std::vector<long long>{2, 6}));
    assert((factorGrouping(8) == std::vector<long long>{2, 2, 2}));
    assert((factorGrouping(1) == std::vector<long long>{1}));
    assert((factorGrouping(7) == std::vector<long long>{7}));

    // Check product and maximum length for various n
    auto check = [](long long n) {
        auto v = factorGrouping(n);
        long long prod = 1;
        for (auto x : v) prod *= x;
        assert(prod == n);
        // This is a heuristic check for maximum length: length equals largest prime exponent
        long long temp = n;
        long long maxExp = 0;
        for (long long p = 2; p * p <= temp; ++p) {
            long long e = 0;
            while (temp % p == 0) { ++e; temp /= p; }
            maxExp = std::max(maxExp, e);
        }
        if (temp > 1) maxExp = std::max(maxExp, 1LL);
        assert(v.size() == (n == 1 ? 1LL : maxExp));
    };

    check(72);      // 2^3 * 3^2 -> length 3
    check(100);     // 2^2 * 5^2 -> length 2
    check(360);     // 2^3 * 3^2 * 5 -> length 3
    check(2*2*2*3*3*5*5); // 2^3*3^2*5^2 -> length 3
    check(97*97);   // prime^2 -> length 2

    std::cout << "All tests passed\n";
}

// The goal is to maximize the number of factors in a product equal to `n`, with each factor > 1. The optimal way is to use prime factorization. Write `n = p1^e1 * p2^e2 * ... * pm^em`. The maximum number of factors is the maximum exponent, say `k = max(ei)`, because you cannot have more than that many factors unless you break a prime into smaller primes, but since primes are indivisible, you can at most have `k` factors (one for each copy of the prime with the highest exponent). For example, if `n = 2^3 * 3^2`, you can have at most 3 factors: e.g., [2,2,2] * [3,3] distributed as [2*3, 2*3, 2] = [6,6,2] length 3. The algorithm: factor `n` by trial division up to sqrt(n). For each prime factor, record its exponent. Sort the exponent-prime pairs in decreasing exponent order (ties arbitrary). Let `k = largest exponent`. Initialize a vector `ans` of length `k` with all 1s. For each pair (e, p) in sorted order, multiply the last `e` elements of `ans` by `p`. This ensures that the highest-exponent prime contributes to all positions, the next one to the next `e` positions, etc. Resulting product is `n`, length `k` is maximum. Edge case: `n=1` has no prime factors, but the snippet would crash because it accesses `factors[0]`. So we must special-case: return `{1}`. Also note that if `n` is prime, `k=1` and ans = [n]. Complexity: trial division up to sqrt(n) gives O(sqrt(n)) time, and sorting the factors O(m log m) where m is number of distinct prime factors (small). Space O(m + k).
