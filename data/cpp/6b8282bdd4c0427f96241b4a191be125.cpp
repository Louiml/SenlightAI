// Write a C++ function `int lastDigits(int a, int b, long long n, int m)` that computes the value of the Fibonacci-like sequence where `f[0] = a`, `f[1] = b`, and `f[i] = (f[i-1] + f[i-2]) % 10000` for all `i >= 2`. The function must return the last `m` decimal digits of `f[n]` (i.e., `f[n] % 10^m`). The input values satisfy: `a` and `b` are integers between 0 and 9999, `n` is a non-negative long long (can be up to 10^18), and `m` is an integer between 1 and 4. Note: because the sequence is computed modulo 10000, it has a periodic behavior with a period that divides 15000 (since there are at most 10000*10000 = 10^8 possible pairs, but for this specific linear recurrence the period is exactly 15000 when starting from any pair). Thus you can reduce `n` modulo 15000. The function must handle the case where `n` is huge and where `m` may be less than 4, meaning you only need the last `m` digits, so if `f[n]` is, e.g., 5 and `m=2`, the result is 5 (not 05, as you return an integer). The function should be efficient even for very large `n`.

// The key observation is that the sequence `f[i]` is computed modulo 10000, and the recurrence `f[i] = f[i-1] + f[i-2]` with fixed modulus creates a periodic sequence that repeats exactly every 15000 positions for any starting values. Why 15000? Because the pair `(f[i], f[i-1])` can have at most 10000*10000 = 100,000,000 distinct states, but for this linear recurrence the period is known to be at most 15000 (the Pisano period for modulus 10000 is 15000, which is the least common multiple of the periods for 2^4=16 and 5^4=625). Therefore, to compute `f[n]`, we only need to compute the first 15000 terms once (starting with the given `a`, `b`) and then index `n % 15000`. After computing `f[n]` (which is already the value modulo 10000), we need to extract the last `m` decimal digits. That is simply `f[n] % (10^m)`, where `10^m` is computed by multiplying 10 repeatedly `m` times. Edge cases: if `n` is 0 or 1, we directly use `a` or `b`. If `n` is very large, the modulo reduction handles it. If `m=4`, we get the full modulo 10000 value. The time complexity is O(15000) to precompute the array, and O(1) for the lookup, regardless of how large `n` is. Space complexity is O(15000) for the array, or O(1) if we reduce to just computing the needed index after reduction (but we still need the array). This is extremely efficient and handles all edge cases.

#include <vector>
#include <cstdint>

// Returns the last m decimal digits of the Fibonacci-like sequence f[n] where
// f[0]=a, f[1]=b, and f[i]=(f[i-1]+f[i-2]) % 10000 for all i>=2.
// n can be any non-negative long long, and m is between 1 and 4.
int lastDigits(int a, int b, long long n, int m) {
    const int MOD = 10000;
    const int PERIOD = 15000;
    
    // Precompute the first PERIOD terms of the sequence.
    std::vector<int> f(PERIOD);
    f[0] = a;
    f[1] = b;
    for (int i = 2; i < PERIOD; ++i) {
        f[i] = (f[i-1] + f[i-2]) % MOD;
    }
    
    // Reduce n modulo the period.
    int index = static_cast<int>(n % PERIOD);
    int value = f[index];
    
    // Compute 10^m.
    int power = 1;
    for (int i = 0; i < m; ++i) {
        power *= 10;
    }
    
    return value % power;
}

#include <cassert>

int main() {
    // Basic cases.
    assert(lastDigits(0, 1, 0, 4) == 0);   // f[0] = 0
    assert(lastDigits(0, 1, 1, 4) == 1);   // f[1] = 1
    assert(lastDigits(0, 1, 2, 4) == 1);   // f[2] = 1
    assert(lastDigits(0, 1, 10, 4) == 55); // Fibonacci 55
    
    // Large n is reduced modulo 15000.
    assert(lastDigits(0, 1, 15000, 4) == 0);  // period resets to f[0]
    assert(lastDigits(0, 1, 15001, 4) == 1);  // f[1]
    assert(lastDigits(0, 1, 1000000000000000000LL, 4) == lastDigits(0, 1, 1000000000000000000LL % 15000, 4));
    
    // Handle m < 4 (only last digits).
    assert(lastDigits(0, 1, 10, 1) == 5);   // 55 % 10 = 5
    assert(lastDigits(0, 1, 10, 2) == 55);  // 55 % 100 = 55
    assert(lastDigits(0, 1, 10, 3) == 55);  // 55 % 1000 = 55
    
    // Non-standard starting values.
    assert(lastDigits(3, 7, 0, 4) == 3);
    assert(lastDigits(3, 7, 1, 4) == 7);
    assert(lastDigits(3, 7, 2, 4) == 10);   // (7+3) % 10000 = 10
    assert(lastDigits(3, 7, 3, 4) == 17);   // (10+7) = 17
    
    // Values that wrap around mod 10000.
    assert(lastDigits(9999, 1, 2, 4) == 0);  // (1+9999)%10000 = 0
    assert(lastDigits(9999, 1, 3, 4) == 1);  // (0+1) = 1
    
    // Edge case: n=0, m=1.
    assert(lastDigits(5, 9, 0, 1) == 5);
    // Edge case: n=1, m=4.
    assert(lastDigits(1234, 5678, 1, 4) == 5678);
    
    return 0;
}
