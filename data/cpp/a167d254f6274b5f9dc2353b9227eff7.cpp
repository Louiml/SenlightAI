/*
Write a C++ function named `swapValues` that takes two integer variables by reference and swaps their values. The function must not return anything (`void`), and the swapping must be visible to the caller (i.e., after calling the function, the caller's variables should hold the swapped values). The function should handle edge cases such as swapping a variable with itself (no change), and it must work correctly for any integer values, including negative numbers and zero. The solution should demonstrate proper use of pass-by-reference, a temporary variable for swapping, and avoid any unnecessary copying.
*/
#include <utility> // not needed, but included for completeness if needed

// Swaps the values of two integers passed by reference.
// The change is visible to the caller.
void swapValues(int& a, int& b) {
    int temp = a; // store original a
    a = b;       // overwrite a with b's value
    b = temp;    // assign b the original a's value
}
#include <cassert>

// Function declaration (or include header containing the function)
void swapValues(int& a, int& b);

int main() {
    // Basic swap
    int x = 5, y = 10;
    swapValues(x, y);
    assert(x == 10 && y == 5);

    // Swap with negative numbers
    int p = -3, q = -7;
    swapValues(p, q);
    assert(p == -7 && q == -3);

    // Swap with zero
    int m = 0, n = 42;
    swapValues(m, n);
    assert(m == 42 && n == 0);

    // Swap with equal values (still works)
    int r = 8, s = 8;
    swapValues(r, s);
    assert(r == 8 && s == 8);

    // Swap a variable with itself (same reference) — should not corrupt value
    int t = 99;
    swapValues(t, t);
    assert(t == 99);

    // Swap multiple times to ensure consistency
    int a = 1, b = 2, c = 3;
    swapValues(a, b); // a=2, b=1
    swapValues(b, c); // b=3, c=1
    assert(a == 2 && b == 3 && c == 1);

    // Edge: large values
    int big1 = 2147483647, big2 = -2147483648;
    swapValues(big1, big2);
    assert(big1 == -2147483648 && big2 == 2147483647);
}
// The core idea is to use pass-by-reference (`int&`) so that the function operates directly on the caller's variables rather than copies. Inside the function, a temporary integer variable stores the value of `x` before overwriting `x` with `y`'s value, then `y` is assigned the temporary's value (which is the original `x`). This three-step swap is the standard algorithm. Edge cases: if `x` and `y` refer to the same variable (caller passes the same variable twice), the temporary still holds the original value, so after `x = y` (no change since they are same), then `y = temp` (also same), so no corruption occurs—the value remains unchanged. No other edge cases exist because integers can be swapped freely. Time complexity is O(1) since only a constant number of operations are performed; space complexity is O(1) as only one extra temporary variable is used (no dynamic memory).
