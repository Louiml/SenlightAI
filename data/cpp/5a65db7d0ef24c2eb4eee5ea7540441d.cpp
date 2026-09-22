// Write a C++ function named `swapIntegers` that takes two integer pointers as parameters and swaps the values stored at those memory locations. The function must work for any two distinct memory locations and must not return a value. After the function completes, the value originally pointed to by the first pointer should be stored at the location pointed to by the second pointer, and vice versa. You must also demonstrate the function's correctness by testing it with both positive and negative values, and ensure that the function uses a temporary local variable for the swap and does not rely on any global state. The function should be declared in a header-style comment and should be usable directly in a program that calls it with addresses of integer variables.

The solution approach is straightforward: the function receives two pointers to integers, dereferences them to access the actual values, and swaps those values using a temporary integer variable. The main algorithm is: (1) store the value at the first pointer in a temporary variable `temp`, (2) assign the value at the second pointer to the location of the first pointer, (3) assign the stored temporary value to the location of the second pointer. Edge cases include when both pointers point to the same memory location—in that case the function should do nothing harmful (the swap is effectively a no-op, though the temporary variable still works correctly). Another edge case is when pointers are null; since the problem assumes valid pointers, we do not need to handle null pointers, but in a production implementation one could add a check. Time complexity is O(1) since only a constant number of operations are performed regardless of input size. Space complexity is also O(1) because only a single integer temporary variable is used. There are no loops or recursion, so the complexity is constant.

#include <cstddef>  // for nullptr (not required but good practice)

// Swap the integer values pointed to by 'a' and 'b'.
// Precondition: 'a' and 'b' point to valid, mutable int objects.
void swapIntegers(int* a, int* b) {
    int temp = *a;  // Save the value at 'a'
    *a = *b;        // Copy value from 'b' to 'a'
    *b = temp;      // Copy saved value to 'b'
}

#include <cassert>

// Global main function to test the swapIntegers function.
int main() {
    // Test 1: basic swap with positive numbers
    int a = 10, b = 20;
    swapIntegers(&a, &b);
    assert(a == 20 && b == 10);

    // Test 2: swap with negative numbers
    int c = -5, d = -15;
    swapIntegers(&c, &d);
    assert(c == -15 && d == -5);

    // Test 3: swap with mixed signs
    int e = 7, f = -3;
    swapIntegers(&e, &f);
    assert(e == -3 && f == 7);

    // Test 4: swap where values are equal
    int g = 42, h = 42;
    swapIntegers(&g, &h);
    assert(g == 42 && h == 42);

    // Test 5: swap where one is zero
    int i = 0, j = 100;
    swapIntegers(&i, &j);
    assert(i == 100 && j == 0);

    // Test 6: swap same pointer (no-op correctness)
    int k = 99;
    swapIntegers(&k, &k);
    assert(k == 99);

    // Test 7: swap with minimum and maximum int values
    int m = 2147483647, n = -2147483647 - 1;  // INT_MAX and INT_MIN
    swapIntegers(&m, &n);
    assert(m == -2147483647 - 1 && n == 2147483647);

    return 0;
}
