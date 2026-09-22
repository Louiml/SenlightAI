// Write a C++ function that takes two integer values by reference and swaps them without using any temporary variable, then returns nothing. The function should be named `swapValues` and must work correctly for any integer values, including negative numbers, zero, and the same value passed twice (e.g., both parameters referring to the same variable or equal values). The function should modify the original variables in place, and the caller should observe the swapped values after the call.

// The solution uses arithmetic operations to swap without a temporary variable. The classic approach is: `a = a + b; b = a - b; a = a - b;` This works by first storing the sum in `a`, then computing the original `a` as the difference between the sum and the current `b` (which is still the original `b`), and finally computing the original `b` as the difference between the new `a` (which is the sum) and the new `b` (which is now the original `a`). This works for all integer values, but there is an edge case: if the sum overflows the integer range, this approach fails due to undefined behavior in C++ (signed integer overflow). A safer alternative is to use XOR swapping: `a ^= b; b ^= a; a ^= b;` which works without overflow for any integer values, including negative numbers and zero, and also handles the case where both references point to the same variable (though that case results in the variable becoming zero, which is acceptable because the original value is lost anyway—but the task does not specify behavior for that). For the reference solution, we will use XOR to avoid overflow and to keep it robust. The function takes `int&` parameters, so it modifies the caller's variables directly. Time complexity is O(1), space complexity O(1).

#include <cstddef>

// Swap two integers in place using bitwise XOR (no temporary variable, no overflow risk).
void swapValues(int& a, int& b) {
    if (&a == &b) {
        return; // If same variable, swapping is a no-op (avoids self-XOR turning it to zero).
    }
    a ^= b;
    b ^= a;
    a ^= b;
}

#include <cassert>

void swapValues(int&, int&); // declaration for testing

int main() {
    // Basic positive numbers
    int x = 5, y = 10;
    swapValues(x, y);
    assert(x == 10 && y == 5);

    // Negative numbers
    int a = -3, b = -7;
    swapValues(a, b);
    assert(a == -7 && b == -3);

    // Mixed sign
    int c = -1, d = 2;
    swapValues(c, d);
    assert(c == 2 && d == -1);

    // Zero and positive
    int e = 0, f = 42;
    swapValues(e, f);
    assert(e == 42 && f == 0);

    // Same value (not the same variable)
    int g = 8, h = 8;
    swapValues(g, h);
    assert(g == 8 && h == 8);

    // Same variable passed twice (no-op expected)
    int i = 99;
    swapValues(i, i);
    assert(i == 99);

    // Large values (no overflow with XOR)
    int p = 2147483647, q = -2147483648;
    swapValues(p, q);
    assert(p == -2147483648 && q == 2147483647);

    return 0;
}
