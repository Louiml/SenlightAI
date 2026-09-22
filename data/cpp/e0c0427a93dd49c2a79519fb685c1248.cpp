/*
Write a C++ function that takes a non-negative integer `n` and returns the result of computing \(2^n\) as an integer. The function should handle inputs from 0 up to and including 30, returning the exact power-of-two value. If `n` is outside this range (i.e., negative or greater than 30), the function should return `-1` to indicate an invalid input. Note that the original snippet uses `pow(2,n)` which may lose precision for large exponents; your implementation should use bit-shifting or iterative multiplication to guarantee integer correctness.
*/
#include <cstdint>

// Compute 2^n for valid n in [0, 30], otherwise return -1.
// Uses left-shift for exact integer arithmetic, avoiding floating-point precision loss.
int powerOfTwo(int n) {
    if (n < 0 || n > 30) {
        return -1;
    }
    // 1 << n computes 2^n exactly; for n=0, result is 1.
    return 1 << n;
}
#include <cassert>

int main() {
    // Basic valid cases
    assert(powerOfTwo(0) == 1);
    assert(powerOfTwo(1) == 2);
    assert(powerOfTwo(2) == 4);
    assert(powerOfTwo(5) == 32);
    assert(powerOfTwo(10) == 1024);
    assert(powerOfTwo(20) == 1048576);
    assert(powerOfTwo(30) == 1073741824);

    // Invalid cases
    assert(powerOfTwo(-1) == -1);
    assert(powerOfTwo(31) == -1);
    assert(powerOfTwo(100) == -1);

    // Additional boundary check
    assert(powerOfTwo(29) == 536870912);
}
// The core operation is computing \(2^n\) exactly. Since the acceptable range is small (`0 ≤ n ≤ 30`), the result fits comfortably within a 32-bit signed integer (max value for `n=30` is `1,073,741,824`, less than `INT_MAX`). The simplest efficient method is to use the left-shift operator `1 << n`, which directly computes \(2^n\) as an integer without floating-point errors. For invalid inputs (`n < 0` or `n > 30`), return `-1`. Edge cases include `n=0` returning `1` (since any number to the power of zero is one), and `n=1` returning `2`. The algorithm runs in constant time \(O(1)\) and uses constant auxiliary space \(O(1)\) because bit-shifting is a single hardware operation.
