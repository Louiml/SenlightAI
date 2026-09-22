Write a C++ function named `swapWithoutTemp` that takes two integer arguments by reference and swaps their values without using any temporary variable, mirroring the logic of the given snippet (multiplication/division swap). The function must work for all valid integer inputs where the product of the two numbers does not overflow and where neither number is zero (since division by zero is undefined). The function should return `void` and modify the arguments in place. Additionally, write a helper function `swapSafe` that uses a temporary variable to perform the swap, and verify both functions produce identical results for a set of test cases. The main task is to implement the multiplication/division swap correctly, handle edge cases (like zero or overflow) by documenting constraints, and ensure const-correctness where applicable.
#include <cassert>

int main() {
    // Test basic positive numbers
    int a = 5, b = 3;
    swapWithoutTemp(a, b);
    assert(a == 3 && b == 5);

    // Test negative numbers (no zero)
    int c = -7, d = 2;
    swapWithoutTemp(c, d);
    assert(c == 2 && d == -7);

    // Test both negative (no zero)
    int e = -4, f = -6;
    swapWithoutTemp(e, f);
    assert(e == -6 && f == -4);

    // Test compare with safe swap for a set of values (excluding zero)
    int x = 123, y = -456;
    int x_copy = x, y_copy = y;
    swapWithoutTemp(x, y);
    swapSafe(x_copy, y_copy);
    assert(x == x_copy && y == y_copy);

    // Test symmetric case (equal numbers)
    int p = 10, q = 10;
    swapWithoutTemp(p, q);
    assert(p == 10 && q == 10);

    // Test large but safe product (e.g., 1000 * 1000 = 1,000,000 fits in int)
    int r = 1000, s = -1000;
    swapWithoutTemp(r, s);
    assert(r == -1000 && s == 1000);

    return 0;
}
#include <utility> // for std::swap (optional, but not used in main solution)

// Swaps two integers using multiplication and division, without a temporary variable.
// Precondition: num1 != 0 and num2 != 0, and num1 * num2 must not overflow int.
void swapWithoutTemp(int& num1, int& num2) {
    num1 = num1 * num2;  // num1 becomes product
    num2 = num1 / num2;  // num2 becomes original num1
    num1 = num1 / num2;  // num1 becomes original num2
}

// Swaps two integers using a temporary variable (safe for all int values).
void swapSafe(int& num1, int& num2) {
    int temp = num1;
    num1 = num2;
    num2 = temp;
}
// The core algorithm is the multiplication/division swap: `a = a * b; b = a / b; a = a / b;`. After the first step, `a` holds the product. The second step computes the original `a` (since `product / b = a`). The third step computes the original `b` (since `product / new_a = product / original_a = b`). However, this method fails when: (1) either `a` or `b` is zero, because division by zero occurs; (2) the product `a * b` overflows the `int` range (undefined behavior in C++). For typical test cases with small integers (e.g., between -1000 and 1000), product fits in 32-bit `int` and no overflow occurs. The boundary condition is to exclude zero values. The algorithm runs in O(1) time and O(1) space. For the reference solution, we implement both the multiplication/division swap and a temporary-variable swap for comparison in tests. The temporary swap is safe for all integers including zero and overflow, but the primary task is to implement the arithmetic swap.
