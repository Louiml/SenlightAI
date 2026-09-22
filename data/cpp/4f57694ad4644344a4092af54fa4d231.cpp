Given two non-negative integers x and y where x ≤ y, write a C++ function that returns the maximum possible value of (a XOR b) for any two integers a and b satisfying x ≤ a ≤ b ≤ y. The function should take two `int` parameters and return an `int` representing the maximum XOR value. This is a standalone task—do not read from standard input or write to standard output inside the function.
The key insight is that the maximum XOR between any two numbers in the range [x, y] is determined by the most significant bit position where x and y differ. Let `diff = x ^ y`. The highest set bit in `diff` indicates the highest bit position where x and y can differ. To maximize XOR, we want to set all bits from that position down to 0 to 1. This is achieved by taking `(1 << (bit_length_of_diff)) - 1`, which is a number consisting entirely of ones from that highest differing bit down to bit 0. Why does this work? Because we can always find two numbers in the range, one with 0 and one with 1 at that highest differing bit, and then choose the remaining lower bits to maximize the XOR to all ones. The algorithm: compute `x ^ y`, find the position of its most significant set bit (by repeated right-shifting until zero), then return `(1 << position) - 1`. Edge cases: if x == y, then `diff` is 0, the loop never executes, and the result is 0 (correct, since a == b == x, XOR is 0). The function works for all non-negative ints including when x and y are close in value. Time complexity is O(log(max(x,y))) due to the bit loop; space complexity is O(1).
#include <bits/stdc++.h>

// Returns the maximum possible value of (a XOR b) for x <= a <= b <= y.
int maxXorInRange(int x, int y) {
    int diff = x ^ y;
    int bit_count = 0;
    while (diff > 0) {
        ++bit_count;
        diff >>= 1;
    }
    // (1 << bit_count) - 1 is a number with all bits set up to the highest differing bit.
    return (1 << bit_count) - 1;
}
#include <cassert>
#include <bits/stdc++.h>

// Include the solution function here (or link it).

int maxXorInRange(int x, int y);

int main() {
    // Basic examples
    assert(maxXorInRange(5, 6) == 3);       // 5^6 = 3
    assert(maxXorInRange(0, 0) == 0);       // single value
    assert(maxXorInRange(1, 1) == 0);       // equal bounds
    assert(maxXorInRange(8, 15) == 7);      // 8^15 = 7
    // Range spanning bit boundary
    assert(maxXorInRange(0, 1) == 1);       // 0^1 = 1
    assert(maxXorInRange(4, 7) == 3);       // 4^7 = 3
    // Larger range
    assert(maxXorInRange(10, 20) == 15);    // e.g. 10^20 = 30 not max, but 31? Check: x=10,y=20 diff=30 -> highest bit at 4 -> 15
    // Edge with large numbers
    assert(maxXorInRange(100000, 200000) == 131071); // diff=131072 -> highest bit at 17 -> (1<<17)-1
    // Consecutive numbers
    assert(maxXorInRange(3, 4) == 7);       // 3^4=7
    // Range includes both sides of bit boundary
    assert(maxXorInRange(2, 5) == 7);       // 2^5=7
    // Another equal bounds
    assert(maxXorInRange(42, 42) == 0);
    return 0;
}
