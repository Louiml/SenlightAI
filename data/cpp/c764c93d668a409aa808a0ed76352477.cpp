/*
Write a C++ function `fastPowerOfTwo(int k)` that computes \(2^k\) for any non-negative integer \(k\) using the divide-and-conquer binary exponentiation technique, but with a twist: the function must not use any loop or the `pow` function, and it must reduce the exponent by halving it recursively rather than by decrementing it. The result must fit within a 32-bit signed integer, so the input \(k\) will be between 0 and 30 inclusive. For \(k=0\), return 1; for larger values, use the recurrence: if \(k\) is even, \(2^k = (2^{k/2})^2\); if \(k\) is odd, \(2^k = 2 \cdot (2^{k/2})^2\). Ensure the function handles \(k=0\) correctly as the base case, and that odd numbers are correctly identified by checking `(k & 1) == 1` (note that the original snippet's buggy `k&1==1` has precedence issues—fix that in your implementation). The function must be recursive and use only integer arithmetic.
*/

#include <cstdint>

// Compute 2^k for non-negative integer k using recursive binary exponentiation.
// Assumes k is in [0, 30] so the result fits in a 32-bit signed int.
int fastPowerOfTwo(int k) {
    // Base case: 2^0 = 1
    if (k == 0) {
        return 1;
    }
    
    // Recursive step: compute 2^(k/2) first
    int half = fastPowerOfTwo(k / 2);
    int squared = half * half; // 2^(2 * (k/2))
    
    // If k is odd, we need an extra factor of 2
    if ((k & 1) == 1) {
        return 2 * squared;
    }
    
    return squared;
}

#include <cassert>

int main() {
    // Base case
    assert(fastPowerOfTwo(0) == 1);
    
    // Small even and odd exponents
    assert(fastPowerOfTwo(1) == 2);
    assert(fastPowerOfTwo(2) == 4);
    assert(fastPowerOfTwo(3) == 8);
    assert(fastPowerOfTwo(4) == 16);
    assert(fastPowerOfTwo(5) == 32);
    
    // Powers at boundaries of guaranteed 32-bit range
    assert(fastPowerOfTwo(10) == 1024);
    assert(fastPowerOfTwo(20) == 1048576);
    assert(fastPowerOfTwo(30) == 1073741824);
    
    // Verify against known powers of two for a sequence
    for (int k = 0; k <= 30; ++k) {
        int expected = 1;
        for (int i = 0; i < k; ++i) {
            expected *= 2;
        }
        assert(fastPowerOfTwo(k) == expected);
    }
    
    return 0;
}

// The main algorithm is a recursive binary exponentiation. For a given exponent \(k\), the base case is \(k=0\), where \(2^0 = 1\). For \(k > 0\), compute the half exponent \(h = k/2\) (integer division). Recursively compute \(2^h\), then square it to get \(2^{2h}\). If \(k\) is even, that's the result. If \(k\) is odd, \(k = 2h + 1\), so multiply the squared result by 2 to account for the extra factor of 2. The key correctness issue is to properly test for oddness using `(k & 1) == 1` (in C++, `&` has lower precedence than `==`, so the original `k&1==1` is interpreted as `k & (1==1)` which is just `k & 1`—actually that works, but it's better to write it explicitly with parentheses). Edge cases: \(k=0\) must return 1 without recursion. For \(k=1\), \(h=0\), recursion returns 1, squared is 1, then because odd, multiply by 2 to get 2. Time complexity: the recursion reduces \(k\) by half each step, so there are \(O(\log k)\) recursive calls, each doing constant work, giving \(O(\log k)\) time. Space complexity: \(O(\log k)\) due to the recursion stack depth. Since \(k \le 30\), this is at most 5 levels deep.
