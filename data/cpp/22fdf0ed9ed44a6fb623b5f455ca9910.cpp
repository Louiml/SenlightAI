Write a C++ function named `swapValues` that takes two integer reference parameters and swaps their values in place. The function must not use any standard library algorithms or temporary storage beyond a single local variable. It should handle all integer values, including negative numbers, zero, and duplicate values, and must work correctly when the same variable is passed for both parameters (self-swap). No return value is needed. The function should be declared in a header guard and include only necessary headers.
#include <cassert>

// Declare the function (normally included from header)
void swapValues(int& a, int& b);

int main() {
    int x = 5, y = 10;
    swapValues(x, y);
    assert(x == 10 && y == 5);

    int a = -3, b = 7;
    swapValues(a, b);
    assert(a == 7 && b == -3);

    int p = 0, q = 0;
    swapValues(p, q);
    assert(p == 0 && q == 0);

    int same = 42;
    swapValues(same, same);
    assert(same == 42);

    int large1 = 2147483647, large2 = -2147483647;
    swapValues(large1, large2);
    assert(large1 == -2147483647 && large2 == 2147483647);

    int m = 1, n = 1;
    swapValues(m, n);
    assert(m == 1 && n == 1);

    return 0;
}
#ifndef SWAP_VALUES_HPP
#define SWAP_VALUES_HPP

// Swap the values of two integer references in place.
// Uses a single temporary variable. Self-swap is safe.
void swapValues(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

#endif // SWAP_VALUES_HPP
// The solution is straightforward: store the value of the first parameter in a temporary local variable, assign the second parameter’s value to the first parameter, and then assign the temporary value to the second parameter. This requires exactly one extra variable of type `int`. Edge cases: if both references refer to the same variable (e.g., `swapValues(x, x)`), the temporary holds the original value, then the first assignment overwrites x with itself, and the second assignment also sets x to the same value—so no corruption occurs. Negative numbers and duplicates are handled because values are copied, not assumed to have any ordering constraints. Time complexity is O(1) with O(1) auxiliary space.
