Write a C++ function named `swapWithoutTemp` that takes two integer references, `a` and `b`, and swaps their values **without using any temporary variable** (e.g., no `int temp = a;`). The function must modify the original variables passed by the caller. Your implementation should handle edge cases where both integers are the same (e.g., `a == b`), negative numbers, and large values that approach the limits of `int`. You must use only arithmetic operations (addition and subtraction) or bitwise XOR (exclusive or) to perform the swap. The function should have a `void` return type and must be declared in a way that guarantees the caller's variables are updated.
#include <cassert>
#include <climits>

void swapWithoutTemp(int& a, int& b);

int main() {
    // Basic swap
    int x = 10, y = 20;
    swapWithoutTemp(x, y);
    assert(x == 20 && y == 10);

    // Negative values
    int m = -5, n = -7;
    swapWithoutTemp(m, n);
    assert(m == -7 && n == -5);

    // Mixed sign
    int p = 100, q = -3;
    swapWithoutTemp(p, q);
    assert(p == -3 && q == 100);

    // Same values (no change expected)
    int r = 42, s = 42;
    swapWithoutTemp(r, s);
    assert(r == 42 && s == 42);

    // Extreme values to test overflow safety with XOR
    int big = INT_MAX, small = INT_MIN;
    swapWithoutTemp(big, small);
    assert(big == INT_MIN && small == INT_MAX);

    // Self-swap (passing the same variable to both references)
    int single = 7;
    swapWithoutTemp(single, single);
    assert(single == 7);

    return 0;
}
#include <cstdint>

// Swaps the values of two integers without using a temporary variable.
// Uses bitwise XOR to avoid any risk of integer overflow.
void swapWithoutTemp(int& a, int& b) {
    if (&a != &b) {  // Guard against self-swap when both references point to the same variable
        a = a ^ b;
        b = a ^ b;
        a = a ^ b;
    }
}
// The core idea is to swap two integers without a temporary variable using arithmetic (addition/subtraction) or bitwise XOR. The arithmetic method works as follows:  
// 1. `a = a + b` (now `a` holds the sum of both original values).  
// 2. `b = a - b` (since `a` is the sum, subtracting the original `b` yields the original `a`, which is assigned to `b`).  
// 3. `a = a - b` (now `a` is the sum minus the new `b` (which is the original `a`), giving the original `b`).  
//
// This works in all cases, including when `a == b` (the steps produce no change, which is correct). However, a critical edge case is integer overflow: if `a + b` exceeds the range of `int` (e.g., `a = INT_MAX`, `b = 1`), the operation causes undefined behavior. To avoid this, the more robust approach is to use bitwise XOR:  
// 1. `a = a ^ b`  
// 2. `b = a ^ b`  
// 3. `a = a ^ b`  
//
// XOR is safe for all integer values because it operates on bits and never overflows. For identical values (`a == b`), the XOR swap correctly leaves them unchanged (since `x ^ x = 0`). The time complexity is constant, \(O(1)\), as only three arithmetic/bitwise operations are performed. The space complexity is also \(O(1)\), requiring no extra storage beyond the two references. The solution must take references to `int` to modify the caller's variables directly.
