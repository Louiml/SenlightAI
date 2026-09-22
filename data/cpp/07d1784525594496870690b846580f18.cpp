// Write a C++ function that, given a non-negative integer `n` (which can be extremely large, up to \(10^{18}\)), returns the count of "good numbers" of length `n` modulo \(10^9+7\). A good number is defined as an `n`-digit positive integer (no leading zeros) whose digits at even indices (0-based, from the left) are even, and digits at odd indices are prime (specifically, must be one of {2, 3, 5, 7}). The function must be efficient for large `n` and must handle both odd and even lengths correctly. Return the result as a `long long` modulo \(10^9+7\).

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Assume solution function is declared above
long long countGoodNumbers(long long n);

int main() {
    // n=1: only one even digit (2,4,6,8) -> 4
    assert(countGoodNumbers(1) == 4);
    // n=2: first digit even (4 choices), second digit prime (4 choices) -> 16
    assert(countGoodNumbers(2) == 16);
    // n=3: indices: 0 even (4), 1 prime (4), 2 even (5) -> 4*4*5=80
    assert(countGoodNumbers(3) == 80);
    // n=4: 4*5*4*4 = 320
    assert(countGoodNumbers(4) == 320);
    // n=5: 4*5*4*5*4? Actually: index0:4, idx1:4, idx2:5, idx3:4, idx4:5 -> 4*4*5*4*5 = 1600
    assert(countGoodNumbers(5) == 1600);
    // Large n check modulo: n=6: pairs = 2 -> 16*20^2 = 16*400=6400
    assert(countGoodNumbers(6) == 6400);
    // Very large n: ensure it runs fast and returns a plausible modulo
    long long result = countGoodNumbers(1000000000000000000LL);
    assert(result >= 0 && result < MOD);
    // Test n=2 and n=1 edge
    // Also test that n=0 (if allowed) returns 1
    assert(countGoodNumbers(0) == 1);
    return 0;
}

#include <bits/stdc++.h>
using namespace std;

const long long MOD = 1000000007LL;

// Fast modular exponentiation: base^exp % MOD
long long mod_pow(long long base, long long exp) {
    long long result = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp & 1) result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}

// Returns the number of good numbers of length n modulo MOD.
// Good number: n-digit positive integer, even digits at even indices (0-based),
// prime digits (2,3,5,7) at odd indices, first digit cannot be zero.
long long countGoodNumbers(long long n) {
    if (n % 2 == 1) {
        // odd n: (n-1)/2 pairs of (even, prime) plus one leading even
        long long pairs = (n - 1) / 2;
        return (4LL * mod_pow(20, pairs)) % MOD;
    } else {
        // even n: n/2 even positions (first 4 choices, others 5) and n/2 prime positions
        if (n == 0) return 1; // edge case, not typically used
        long long pairs_after_leading = n / 2 - 1; // number of (even, prime) pairs after the first digit
        long long base = 20;
        return (16LL * mod_pow(base, pairs_after_leading)) % MOD;
    }
}

// We need to count all strings of length `n` where:
// - Position 0 (most significant digit) must be even (since it's an even index), and cannot be 0 because the number has exactly `n` digits. So it has 4 choices: {2,4,6,8}.
// - Every other even index (2,4,6,...) has 5 choices: {0,2,4,6,8}.
// - Every odd index (1,3,5,...) has 4 choices: {2,3,5,7}.
//
// Thus, the total count is:  
// For even positions (excluding leading position if index 0), count of positions = 
// - If `n` is even: even indices are 0,2,...,n-2 → count = n/2. Among these, position 0 has 4 choices, others (n/2 - 1) positions have 5 choices. Odd positions: n/2 positions, each 4 choices.
//   Total = \(4 \cdot 5^{n/2 - 1} \cdot 4^{n/2} = 4 \cdot (5^{n/2 - 1}) \cdot 4^{n/2} = 4 \cdot 20^{n/2 - 1} \cdot 4\)? Let's compute more carefully:
//   Better: For even `n`: even positions count = n/2, among them one leading (4 choices) and (n/2 -1) others (5 each). Odd positions count = n/2, each 4 choices. So total = \(4 \cdot 5^{n/2 -1} \cdot 4^{n/2}\). That equals \(4 \cdot (5^{n/2 - 1}) \cdot (4^{n/2}) = 4 \cdot 5^{n/2 -1} \cdot 4^{n/2} = 4 \cdot (5^{n/2 -1}) \cdot 4^{n/2} \). That's not simply 20^{n/2}. But note that \(5^{n/2 - 1} \cdot 4^{n/2} = 5^{n/2 - 1} \cdot 4 \cdot 4^{n/2 -1} = 4 \cdot (5 \cdot 4)^{n/2 - 1} = 4 \cdot 20^{n/2 -1}\). Multiply by leading 4: total = \(4 \cdot 4 \cdot 20^{n/2-1} = 16 \cdot 20^{n/2-1} = (16/20) \cdot 20^{n/2} = (4/5) \cdot 20^{n/2}\). Hmm, that's not integer nicely. Let's recalc directly:  
//   For even n:  
//   - Even indices: indices 0,2,...,n-2 → count = n/2.  
//   - Among them: index 0 has 4 choices, indices 2,4,...,n-2 (count = n/2 -1) each 5 choices.  
//   - Odd indices: 1,3,...,n-1 → count = n/2, each 4 choices.  
//   Total = \(4 \cdot 5^{n/2 - 1} \cdot 4^{n/2}\). Simplify: \(= 4 \cdot (5^{n/2 - 1}) \cdot (4 \cdot 4^{n/2 - 1}) = 4 \cdot 4 \cdot 5^{n/2 - 1} \cdot 4^{n/2 -1} = 16 \cdot (20)^{n/2 -1}\).  
//   So for even n, total = \(16 \cdot 20^{n/2 -1}\) modulo MOD.
//
// - If `n` is odd: even indices: 0,2,...,n-1 → count = (n+1)/2. Among them index 0 has 4 choices, the other (n+1)/2 -1 = (n-1)/2 positions have 5 each. Odd indices: count = (n-1)/2, each 4 choices.  
//   Total = \(4 \cdot 5^{(n-1)/2} \cdot 4^{(n-1)/2} = 4 \cdot (20)^{(n-1)/2}\).  
//   Since n odd, let m = (n-1)/2. Then total = \(4 \cdot 20^m\). This matches the given snippet's odd case: `power(20, (n-1)/2) * 5`? Wait, snippet uses `power(20, t/2) * 5` with t = n-1, so t/2 = (n-1)/2. That gives `power(20, (n-1)/2) * 5`. But our derivation says `4 * 20^{(n-1)/2}`. So snippet uses 5, but correct is 4? Let's check with n=1: even index 0 only, should be 4 choices (2,4,6,8). Snippet gives power(20,0)*5 = 5, which is wrong. So snippet has a bug! But we must create a correct task. The snippet's logic is incorrect for the "good numbers" definition as stated. However, the snippet's code comments suggest they intended something else: possibly "even digits at even positions and prime digits at odd positions" but they used 5 for leading? Actually 5 would be all even digits including 0, but leading zero not allowed. So we must fix it. Our task will define the correct constraints, and we'll write a correct solution.  
//   For even n: total = \(16 \cdot 20^{n/2 -1}\) for n>=2. For n=0? Not needed; n is positive. For n=2: even indices 0,2? Wait n=2, indices 0,1. So even index 0 only (index 2 doesn't exist). Actually for length 2, indices are 0 and 1. Even index count = 1 (index 0), odd index count = 1 (index 1). So even positions: index 0 has 4 choices; odd positions: index 1 has 4 choices. Total = 4*4 = 16. Our formula: n=2 even → n/2=1, n/2-1=0, so 16 * 20^0 = 16. Good. For n=4: indices 0,1,2,3. Even indices 0,2: index0 4 choices, index2 5 choices; odd indices 1,3: 4*4 =16. Total = 4*5*16=320. Formula: n=4, n/2=2, n/2-1=1 → 16*20^1 = 320. Good.  
//   For odd n=1: total = 4 * 20^0 =4. For n=3: indices 0,1,2. Even indices 0,2: index0 4, index2 5 → 20; odd index 1: 4 → total 80. Formula: (n-1)/2 =1 → 4*20^1 =80. Good.
//
// Thus a correct unified formula:  
// - If n is odd: answer = \(4 \cdot power(20, (n-1)/2) \mod MOD\).  
// - If n is even: if n==1? Actually n even and n>=2: answer = \(16 \cdot power(20, n/2 -1) \mod MOD\).  
// Alternatively, we can treat separately: For n even, let k = n/2 -1. Then answer = 16 * 20^k. For n odd, let k = (n-1)/2. Then answer = 4 * 20^k.  
// Edge cases: n=0? Probably not needed, but if given, there are no digits, count 1? Usually not. We'll assume n>=1. Time complexity: O(log n) for fast exponentiation. Space O(1).
