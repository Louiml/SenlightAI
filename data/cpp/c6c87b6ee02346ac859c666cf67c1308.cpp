Write a C++ function named `decimalToBinary` that accepts a single positive integer `n` (with `1 <= n <= 31`) and returns an integer representing its binary equivalent using only the decimal digits 0 and 1 (e.g., input `5` returns `101`, input `13` returns `1101`). The function must be implemented using recursion (no loops inside the function), and must handle the base case correctly so that no leading zeros appear in the result. You may assume the input is always within the valid range and positive, so no error checking is required inside the function. The return type is `int`, and the result must fit comfortably within a 32-bit integer. The function should be `const`-correct and free of side effects.

The core idea is to recursively compute the binary representation by using the property that the least significant bit of `n` is `n % 2`, and the higher-order bits come from the binary representation of `n / 2`. If we naively compute `10 * result + (n % 2)` after recursing, we build the number from most significant bit to least. The base case is `n <= 0`, which should return 0 (not 1, to avoid leading one incorrectly placed), for the recursive call to `n/2` when `n` becomes 0. Then for each recursion level, we take the current digit `n % 2` and append it: `return 10 * rec(n / 2) + (n % 2)`. This ensures the first call (e.g., for n=5) recurses to 2, then to 1, then to 0 (base returns 0), then builds: rec(1) returns 10*0+1=1, rec(2) returns 10*1+0=10, rec(5) returns 10*10+1=101. Edge cases: n=1 returns 1; n=0 is not valid per spec but if passed should return 0. Time complexity is O(log n) recursive calls, space complexity O(log n) due to call stack. The result is an integer, and for n up to 31, binary fits in 5 bits, so the output is at most 11111, well within int range.

// Compute the binary representation of a positive integer n as an integer
// using only decimal digits 0 and 1. Recursive implementation.
int decimalToBinary(int n) {
    // Base case: n becomes 0 from recursion on n/2, return 0 to avoid leading zeros.
    if (n <= 0) {
        return 0;
    }
    // Recursively get the binary representation of the prefix (n/2),
    // then append the current least significant bit (n % 2) at the end.
    return 10 * decimalToBinary(n / 2) + (n % 2);
}

#include <cassert>

int decimalToBinary(int n); // Declaration, implementation as above

int main() {
    // Base case and small numbers
    assert(decimalToBinary(1) == 1);
    assert(decimalToBinary(2) == 10);
    assert(decimalToBinary(3) == 11);
    assert(decimalToBinary(4) == 100);
    // Typical examples
    assert(decimalToBinary(5) == 101);
    assert(decimalToBinary(6) == 110);
    assert(decimalToBinary(7) == 111);
    // Larger within range (up to 31)
    assert(decimalToBinary(10) == 1010);
    assert(decimalToBinary(13) == 1101);
    assert(decimalToBinary(15) == 1111);
    assert(decimalToBinary(16) == 10000);
    assert(decimalToBinary(31) == 11111);
    return 0;
}
