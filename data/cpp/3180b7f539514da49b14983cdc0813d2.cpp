/*
Write a C++ function named `swapValues` that takes two integer parameters by reference, swaps their values using a temporary variable, and returns `void`. The function must work correctly for any integer values, including negative numbers, zero, and equal values. After calling the function, the original arguments passed in must have their values exchanged. Do not use `std::swap` or any bitwise XOR swapping technique—only a temporary variable approach as shown in the snippet. The function should be `const`-correct where applicable (although passing by non-const reference is required because the values are modified). Provide a standalone implementation with necessary `#include` directives.
*/

#include <utility> // not actually required, but included as a placeholder

// Swap two integer values using a temporary variable.
void swapValues(int& first, int& second) {
    int temp = first; // Save the value of first
    first = second;   // Assign second's value to first
    second = temp;    // Assign the saved value to second
}

#include <cassert>

int main() {
    int a = 1, b = 2;
    swapValues(a, b);
    assert(a == 2 && b == 1);

    int c = -5, d = 10;
    swapValues(c, d);
    assert(c == 10 && d == -5);

    int e = 0, f = 0;
    swapValues(e, f);
    assert(e == 0 && f == 0);

    int g = 7, h = 7;
    swapValues(g, h);
    assert(g == 7 && h == 7);

    int i = -100, j = -200;
    swapValues(i, j);
    assert(i == -200 && j == -100);

    return 0;
}

// The core algorithm directly mirrors the given snippet: store the value of the first parameter in a temporary variable, assign the second parameter's value to the first parameter, then assign the temporary value to the second parameter. This ensures both variables are swapped correctly. Edge cases include: both values equal (works because temp holds the original, and the assignments are symmetric), negative values (no special handling needed), and zero (works fine). The time complexity is O(1) constant time, and space complexity is O(1) since only one temporary integer is used. The function must take parameters by non-const reference to modify the caller's variables; no `const` can be applied to the parameters themselves, but the temporary can be `const` after initialization if desired (though not necessary).
