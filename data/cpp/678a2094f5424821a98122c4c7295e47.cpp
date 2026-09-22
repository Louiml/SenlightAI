Write a C++ function that, given a positive integer `n`, returns the value of `2^n` modulo 1,000,000,007 (a prime number). The input may be as large as 10^9, so the solution must be efficient. The function should handle the base case `n = 0` (returning 1) correctly and should not rely on floating-point arithmetic or overflow-prone native integer exponentiation.
// The straightforward iterative multiplication used in the provided snippet multiplies by 2 exactly `n` times, each time reducing modulo 1,000,000,007. This works but runs in O(n) time, which is too slow for n up to 10^9. To handle large `n`, use exponentiation by squaring (binary exponentiation). The recurrence is: if `n` is even, `2^n = (2^(n/2))^2`; if odd, `2^n = 2 * (2^((n-1)/2))^2`. Each recursive or iterative step halves `n`, yielding O(log n) multiplications. Modular multiplication is safe because the product of two numbers below the modulus fits in a 64-bit integer (the modulus is ~1e9, product ~1e18 < 9.22e18, the limit of `long long`). Edge cases include `n = 0` returning 1 (since any number to the power 0 is 1), and `n` being large but still within the limits of the input type. Use `const long long MOD = 1000000007LL` for clarity. Time complexity: O(log n). Space complexity: O(1) for an iterative loop, or O(log n) for recursion stack if recursive; the iterative version is preferred.
#include <cstdint>

// Compute (base^exponent) % MOD using binary exponentiation.
long long powerMod(long long base, long long exponent, const long long MOD) {
    long long result = 1;
    base %= MOD;
    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exponent >>= 1;
    }
    return result;
}

// Return (2^n) % 1000000007.
long long powerOfTwoMod(long long n) {
    const long long MOD = 1000000007LL;
    return powerMod(2LL, n, MOD);
}
#include <cassert>
#include <cstdint>

// Declare the function (or include the header where it's defined).
long long powerOfTwoMod(long long n);

int main() {
    const long long MOD = 1000000007LL;

    // Base case
    assert(powerOfTwoMod(0) == 1);
    
    // Small values
    assert(powerOfTwoMod(1) == 2);
    assert(powerOfTwoMod(2) == 4);
    assert(powerOfTwoMod(3) == 8);
    assert(powerOfTwoMod(10) == 1024);
    
    // Values that wrap around the modulus
    // 2^30 = 1073741824, mod 1e9+7 = 73741817
    assert(powerOfTwoMod(30) == 73741817LL);
    
    // Large n: 2^1000000000 mod 1e9+7 (known result, you can verify)
    // Here we just check it returns a value in valid range
    long long large = powerOfTwoMod(1000000000LL);
    assert(large >= 0 && large < MOD);
    
    // Compare with iterative method for moderate n (e.g., 1000)
    long long expected = 1;
    for (int i = 0; i < 1000; ++i) {
        expected = (expected * 2) % MOD;
    }
    assert(powerOfTwoMod(1000) == expected);
    
    // Even/odd powers
    assert(powerOfTwoMod(31) == (2LL * powerOfTwoMod(30)) % MOD);
    assert(powerOfTwoMod(62) == (powerOfTwoMod(31) * powerOfTwoMod(31)) % MOD);
    
    return 0;
}
