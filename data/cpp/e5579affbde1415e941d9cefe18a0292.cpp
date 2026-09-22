// Write a standalone C++ function named `swapIntegersByReference` that takes two `int` parameters by reference, swaps their values, and returns `void`. The function must not use any standard library functions (like `std::swap`) and must implement the swap manually using a temporary variable. Additionally, write a separate test harness that verifies the function works correctly for positive, negative, and equal values, including boundary cases like `INT_MAX` and `INT_MIN`. The function should be declared in a header-style comment and must be const-correct (i.e., no `const` applied to the parameters since they are modified). The main program should not be part of the solution code—only the function definition is required. The test code must call the function directly and use `assert` to verify the swapped values.
The solution is straightforward: accept two `int` references, store the first value in a temporary variable, assign the second value to the first, then assign the temporary to the second. This swaps the values in the caller’s scope. Edge cases include identical values (swap is idempotent), negative numbers (works with standard integer arithmetic), and extreme values like `INT_MAX` and `INT_MIN` (no overflow occurs because we only copy values, never perform arithmetic on them). The time complexity is O(1) constant time, and space complexity is O(1) auxiliary space (only one temporary `int`). No special handling is required for equality or extremes because the copy operations are safe. The function must be declared with `void` return type and take parameters by non-const reference to allow modification.
// Swap the values of two integers using a temporary variable.
// Parameters are passed by reference so changes affect the caller.
void swapIntegersByReference(int& a, int& b) {
    int temp = a;  // Store the first value
    a = b;         // Assign second to first
    b = temp;      // Assign original first to second
}
#include <cassert>
#include <climits>

int main() {
    // Basic swap with positive integers
    int x = 37, y = 36;
    swapIntegersByReference(x, y);
    assert(x == 36 && y == 37);

    // Swap negative integers
    int a = -5, b = -10;
    swapIntegersByReference(a, b);
    assert(a == -10 && b == -5);

    // Swap a negative with a positive
    int c = -1, d = 1;
    swapIntegersByReference(c, d);
    assert(c == 1 && d == -1);

    // Swap equal values (should remain equal)
    int e = 42, f = 42;
    swapIntegersByReference(e, f);
    assert(e == 42 && f == 42);

    // Swap extreme integer values (INT_MAX and INT_MIN)
    int g = INT_MAX, h = INT_MIN;
    swapIntegersByReference(g, h);
    assert(g == INT_MIN && h == INT_MAX);

    // Swap with zero
    int i = 0, j = 7;
    swapIntegersByReference(i, j);
    assert(i == 7 && j == 0);

    return 0;
}
