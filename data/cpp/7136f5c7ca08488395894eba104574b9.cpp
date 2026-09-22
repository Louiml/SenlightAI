/*
Given a positive integer `n` (1 ≤ n ≤ 10^18), write a C++ function that computes the sum of the divisors of `n` after removing all factors of 2 from it, then multiplies together the sums of the geometric series of the remaining prime factors 3, 5, and 7. Specifically, let `m` be the result of dividing `n` by all factors of 2. Then compute the product over each prime `p` in {3, 5, 7} of (1 + p + p^2 + ... + p^k) where `k` is the exponent of `p` in the factorization of `m` (if `p` does not divide `m`, the term is 1). Return this product as a `long long`. Note: if `n` has factors other than 2, 3, 5, 7, those factors are ignored after dividing by 2, so only the mentioned primes contribute.
*/

#include <cstdint>

// Compute the product of geometric series sums for primes 3, 5, and 7,
// after removing all factors of 2 from the input number.
// Returns the product as a long long.
long long computeProductAfterRemoving2(long long n) {
    // Remove all factors of 2
    while (n % 2 == 0) {
        n /= 2;
    }
    
    // Helper lambda to compute sum of 1 + p + p^2 + ... + p^k
    // where k is the exponent of p in n (after removing 2s).
    auto sumGeometric = [&](long long p) -> long long {
        long long sum = 1;  // account for p^0
        long long power = 1; // current power of p
        while (n % p == 0) {
            n /= p;
            power *= p;
            sum += power;
        }
        return sum;
    };
    
    long long sum3 = sumGeometric(3);
    long long sum5 = sumGeometric(5);
    long long sum7 = sumGeometric(7);
    
    return sum3 * sum5 * sum7;
}

#include <cassert>
#include <cstdint>

// Include the solution function here (or link appropriately)
// The function is defined above.

int main() {
    // Test 1: n = 1 -> no factors, all sums are 1, product = 1
    assert(computeProductAfterRemoving2(1LL) == 1LL);
    
    // Test 2: n = 2 -> after removing 2, n=1, product = 1
    assert(computeProductAfterRemoving2(2LL) == 1LL);
    
    // Test 3: n = 3 -> sum3 = 1+3 = 4, others 1 -> product = 4
    assert(computeProductAfterRemoving2(3LL) == 4LL);
    
    // Test 4: n = 6 = 2*3 -> remove 2 -> n=3, product = 4
    assert(computeProductAfterRemoving2(6LL) == 4LL);
    
    // Test 5: n = 9 = 3^2 -> sum3 = 1+3+9=13, product = 13
    assert(computeProductAfterRemoving2(9LL) == 13LL);
    
    // Test 6: n = 15 = 3*5 -> sums: 3:4, 5:6, 7:1 -> product = 24
    assert(computeProductAfterRemoving2(15LL) == 24LL);
    
    // Test 7: n = 21 = 3*7 -> sums: 3:4, 5:1, 7:8 -> product = 32
    assert(computeProductAfterRemoving2(21LL) == 32LL);
    
    // Test 8: n = 105 = 3*5*7 -> sums: 4*6*8 = 192
    assert(computeProductAfterRemoving2(105LL) == 192LL);
    
    // Test 9: n = 210 = 2*3*5*7 -> remove 2 -> 105 -> product = 192
    assert(computeProductAfterRemoving2(210LL) == 192LL);
    
    // Test 10: n = 27 = 3^3 -> sum3 = 1+3+9+27=40, product = 40
    assert(computeProductAfterRemoving2(27LL) == 40LL);
    
    return 0;
}

// The provided snippet processes a number `n` by repeatedly dividing out 2, then for each of the primes 3, 5, and 7, it computes the sum of the geometric series of that prime up to its exponent. The key observation is that while `n % p == 0`, we divide `n` by `p` and accumulate the sum `1 + p + p^2 + ...` by using a variable `g` that multiplies by `p` each iteration. Initially `f[p]` is set to 1, then inside the loop `g` starts as 1 and becomes `p`, `p^2`, etc., adding each to `f[p]`. The product of `f[3]`, `f[5]`, and `f[7]` is returned. Edge cases: if `n` is a power of 2, after dividing out all 2s `n` becomes 1, and for primes not dividing the number the arrays remain 1, so the product is 1. If `n` is large (up to 10^18), the sums might overflow 64-bit, but since the problem constraints presumably keep the result within `long long`, we proceed. The while loops run at most logarithmic times in `n`, so time complexity is O(log n) and space is O(1). The algorithm correctly ignores factors other than 2, 3, 5, 7.
