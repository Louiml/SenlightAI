Write a C++ function `int countOnes(int n)` that takes a 32-bit signed integer and returns the number of bits set to 1 in its binary representation (two's complement for negative numbers). The function must handle all possible integer inputs, including negative numbers (e.g., `-1` has all 32 bits set, so the result is 32), zero (result is 0), and positive values. Do not use loops that count bits by converting to a string or by precomputed tables; instead, implement an efficient bitwise algorithm. The function must be `const`-correct and should be placed in a self-contained file with no `main` function. Provide a separate test harness in a `main` function that uses `assert` to validate the function against known values, including edge cases like `0`, `1`, `-1`, `INT_MAX`, `INT_MIN`, and a variety of positive and negative numbers.

#include <cassert>
#include <climits>
#include <iostream>

// Function under test (declaration for linking; defined elsewhere in the solution)
int countOnes(int n);

int main() {
    // Basic positive numbers
    assert(countOnes(0) == 0);
    assert(countOnes(1) == 1);
    assert(countOnes(2) == 1);   // 10
    assert(countOnes(3) == 2);   // 11
    assert(countOnes(9) == 2);   // 1001
    assert(countOnes(15) == 4);  // 1111
    assert(countOnes(255) == 8);

    // Negative numbers (two's complement)
    assert(countOnes(-1) == 32);          // all bits set
    assert(countOnes(-2) == 31);          // ...1110
    assert(countOnes(-3) == 31);          // ...1101
    assert(countOnes(-8) == 29);          // ...11111000
    assert(countOnes(-2147483648) == 1);  // INT_MIN, only sign bit set

    // Boundary values
    assert(countOnes(INT_MAX) == 31);     // 0x7FFFFFFF
    assert(countOnes(INT_MIN) == 1);      // 0x80000000

    // Random checks
    assert(countOnes(1024) == 1);         // 2^10
    assert(countOnes(1023) == 10);        // 2^10 - 1
    assert(countOnes(-1024) == 22);       // ...1111110000000000

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <cstdint>

// Returns the number of 1 bits in the binary representation of a 32-bit signed integer.
// Uses the n & (n - 1) trick to clear the lowest set bit each iteration.
int countOnes(int n) {
    int count = 0;
    while (n != 0) {
        ++count;
        n = n & (n - 1);
    }
    return count;
}

// The most elegant and efficient approach is the bit-manipulation trick `n = n & (n - 1)`, which clears the lowest set bit in `n` each time. Counting how many times we can apply this operation until `n` becomes zero gives the number of 1 bits. This works for both positive and negative numbers because in two's complement, subtracting 1 from a number flips all trailing 0 bits to 1 and flips the lowest 1 bit to 0, and the bitwise AND clears that lowest 1 bit. For example, for `n = 9 (1001)`: `9 & 8 = 8`, then `8 & 7 = 0`, two iterations, so count = 2. For `n = -1` (all 32 bits set), the loop will run 32 times because each `n & (n-1)` clears one set bit. For `n = 0`, the loop never runs, returning 0. The time complexity is O(k), where k is the number of set bits (at most 32 for a 32-bit int), so worst-case O(1) in practical terms. The space complexity is O(1). Edge cases include negative numbers, zero, and the minimum integer `INT_MIN` (which has only the sign bit set, so count = 1). This method is optimal and avoids any undefined behavior from shifting signed negative numbers (unlike left-shifting which can cause issues).
