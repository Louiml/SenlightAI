Write a standalone C++ function `uint64_t factor_one(uint64_t n)` that, for any composite input `n > 1`, returns a single non-trivial factor (a divisor strictly between 1 and `n`). If the input is prime or less than 2, return `n` itself (or 1 for `n==1`). You may use a simple deterministic trial division for small numbers (e.g., `n < 1'000'000`) and for larger numbers use a Pollard’s rho algorithm with a fixed starting constant `c = 1` and `c = 2` fallback. You must implement a helper `gcd` (binary GCD) and use `uint64_t` arithmetic with careful overflow handling in modular multiplication (e.g., using `__int128`). The function must be self-contained (no external libraries beyond `<cstdint>`, `<cstddef>`, `<cmath>`, `<cstdlib>`, `<limits>`).

#include <cassert>
#include <cstdint>

// Declare the function (assume the solution above is included).
uint64_t factor_one(uint64_t n);

int main() {
    // Small composite numbers
    assert(factor_one(4) == 2);
    assert(factor_one(9) == 3);
    assert(factor_one(15) == 3 || factor_one(15) == 5);
    assert(factor_one(49) == 7);
    assert(factor_one(100) == 2 || factor_one(100) == 5);
    
    // Small primes: return n itself
    assert(factor_one(2) == 2);
    assert(factor_one(3) == 3);
    assert(factor_one(17) == 17);
    assert(factor_one(97) == 97);
    
    // Edge cases
    assert(factor_one(1) == 1);
    assert(factor_one(0) == 0);
    
    // Large composite (e.g., 2^32 + 1 = 4294967297 = 641 * 6700417)
    uint64_t large = 4294967297ULL;
    uint64_t f = factor_one(large);
    assert(f > 1 && f < large && large % f == 0);
    
    // Large prime (e.g., 2^31 - 1 = 2147483647)
    uint64_t prime = 2147483647ULL;
    assert(factor_one(prime) == prime);
    
    // Even large number
    assert(factor_one(18446744073709551615ULL) == 3); // 2^64-1 is divisible by 3
    
    return 0;
}

#include <cstdint>
#include <cstddef>
#include <cmath>
#include <cstdlib>
#include <limits>

// Binary GCD (from the snippet, iterative form to avoid recursion depth).
static uint64_t gcd_binary(uint64_t a, uint64_t b) {
    if (a == 0) return b;
    if (b == 0) return a;
    int shift = 0;
    while (((a | b) & 1) == 0) {
        a >>= 1;
        b >>= 1;
        ++shift;
    }
    while ((a & 1) == 0) a >>= 1;
    do {
        while ((b & 1) == 0) b >>= 1;
        if (a > b) { uint64_t t = a; a = b; b = t; }
        b -= a;
    } while (b != 0);
    return a << shift;
}

// Modular multiplication without overflow using __int128.
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__int128)a * b % mod);
}

// Trial division for small n. Returns a factor or n if prime.
static uint64_t trial_small(uint64_t n) {
    if (n % 2 == 0) return 2;
    // Check odd divisors up to sqrt(n)
    for (uint64_t i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return i;
    }
    return n; // prime
}

// Pollard's rho with retry on failure. Returns a non-trivial factor or n.
static uint64_t pollard_rho(uint64_t n) {
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (uint64_t c = 1; c <= 2; ++c) {
        uint64_t t = 2, h = 2, d = 1;
        // Limit iterations to avoid infinite loops.
        for (int iter = 0; iter < 1000 && d == 1; ++iter) {
            t = (mod_mul(t, t, n) + c) % n;
            h = (mod_mul(h, h, n) + c) % n;
            h = (mod_mul(h, h, n) + c) % n;
            d = gcd_binary(t > h ? t - h : h - t, n);
        }
        if (d > 1 && d < n) return d;
    }
    return n; // fallback after retries
}

// Public function: return a non-trivial factor of n, or n if prime/trivial.
uint64_t factor_one(uint64_t n) {
    if (n <= 1) return n;
    if (n <= 1000000) return trial_small(n);
    // For larger n, use rho.
    return pollard_rho(n);
}

// The solution combines two strategies:  
// 1. **Trial division** for small `n` (<1e6): iterate `i` from 2 to `sqrt(n)`, checking divisibility. If a factor is found, return it; if none found, `n` is prime and we return `n`. This handles small inputs exactly and avoids overhead of rho.  
// 2. **Pollard’s rho** for larger `n`: Use a cycle-detection loop with `t` and `h` both starting at 2. At each step, update `t = f(t)`, `h = f(f(h))` where `f(x) = (x*x + c) mod n`. Compute `d = gcd(|t-h|, n)`. If `d == 1`, continue; if `d == n`, retry with `c+1`. Otherwise, `d` is a non-trivial factor. To avoid signed underflow, compute absolute difference as `t>h ? t-h : h-t`. For modular multiplication `(x*x) mod n`, we must avoid 64-bit overflow; use `__int128` for the multiplication and modulo.  
//
// **Edge cases:**  
// - `n <= 1` → return `n` (or 1).  
// - `n` is even → return 2 immediately (common optimization).  
// - `n` is prime → trial division for small numbers returns `n`; for large primes, rho will fail (d == n each time), so after a few retries we return `n` as a safety.  
// - For `n` a perfect square of a prime, rho might find the prime factor correctly.  
//
// **Complexity:**  
// - Trial division: O(√n) worst-case, but only for small n.  
// - Pollard’s rho: expected O(n^(1/4)) time per factor, with O(1) auxiliary space. The `__int128` multiplication is O(1).  
//
// **Implementation notes:**  
// - We include a `mod_mul` helper using `__int128`.  
// - We use a fixed maximum number of rho iterations (e.g., 1000) to avoid infinite loops; after that, return `n`.  
// - The function returns the first non-trivial factor found.
