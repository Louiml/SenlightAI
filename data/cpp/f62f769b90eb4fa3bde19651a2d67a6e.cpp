// Write a standalone C++ function named `swapValues` that takes two integer references and swaps their values without using a temporary variable. The function must handle any valid `int` values, including negative numbers and values near the integer limits (e.g., `INT_MAX` and `INT_MIN`). Your implementation must be free of undefined behavior (e.g., no XOR-based swapping that could overflow or alias the same variable). After calling the function, the original variables must hold each other's previous values. Provide a descriptive comment explaining the safety of the approach, and ensure the function is declared `inline` for potential performance benefits.
#include <cassert>
#include <climits>
#include <iostream>

// Declaration of the function under test (normally from a header).
inline void swapValues(int& a, int& b);

int main() {
    // Basic swap
    int x = 5, y = 10;
    swapValues(x, y);
    assert(x == 10 && y == 5);

    // Negative numbers
    int p = -3, q = -7;
    swapValues(p, q);
    assert(p == -7 && q == -3);

    // Mixed signs
    int m = -100, n = 200;
    swapValues(m, n);
    assert(m == 200 && n == -100);

    // Extreme values (no overflow)
    int a = INT_MAX, b = INT_MIN;
    swapValues(a, b);
    assert(a == INT_MIN && b == INT_MAX);

    // Aliasing: same variable passed twice (should not zero it)
    int same = 42;
    swapValues(same, same);
    assert(same == 42);

    // Values after multiple swaps
    int i = 1, j = 2;
    swapValues(i, j);
    swapValues(i, j);
    assert(i == 1 && j == 2);

    // One is zero
    int z = 0, nonz = 123;
    swapValues(z, nonz);
    assert(z == 123 && nonz == 0);

    // Large negative and large positive pair
    int big1 = -123456789, big2 = 987654321;
    swapValues(big1, big2);
    assert(big1 == 987654321 && big2 == -123456789);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
#include <cstddef>

// Swaps the values of two integers without using a temporary variable.
// Uses XOR swap, but first checks for aliasing (same memory location) to
// avoid the undefined self-zeroing case. XOR is safe for all integer values,
// including negative and extreme values, as it does not cause overflow.
inline void swapValues(int& a, int& b) {
    if (&a == &b) {
        return; // Swapping a variable with itself is a no-op.
    }
    a ^= b;
    b ^= a;
    a ^= b;
}
// The solution must swap two integers without a temporary variable. The classic XOR-based trick (`a ^= b; b ^= a; a ^= b;`) works for distinct variables but can invoke undefined behavior if `a` and `b` reference the same memory location (since it would zero the value). To avoid this, the safe approach is to use arithmetic addition and subtraction: `a = a + b; b = a - b; a = a - b;`. However, this can still overflow when `a + b` exceeds `INT_MAX` or falls below `INT_MIN`, which is also undefined behavior. The safest solution that avoids both issues is to check for aliasing first, and if the addresses are different, use the arithmetic method with a guard. Alternatively, a simple and robust method is to use `std::swap` from `<utility>` which is guaranteed to be correct and noexcept for fundamental types, but the task demands a custom implementation. The best custom approach is to use a bitwise XOR only after verifying the addresses differ, and fall back to a temporary variable otherwise. Since the task explicitly forbids undefined behavior and requires safety for all `int` values, the cleanest solution is: if `&a != &b`, use arithmetic with explicit checks for overflow — but that's complex. The simplest safe approach is to use a local temporary variable, which is allowed because the restriction is on not using a temporary *in the solution*? The task says "without using a temporary variable" — so we must avoid a named temporary. But we can still use arithmetic if we first check for overflow by computing the sum using `std::numeric_limits<int>::max()` and `min()`. A simpler alternative: use bitwise XOR but only after checking `&a != &b`. If they are same, assign 0? Actually if a and b reference the same variable, swapping is a no-op, so we can simply return. So the safe XOR approach: if (&a == &b) return; then `a ^= b; b ^= a; a ^= b;`. This is well-defined because XOR does not overflow and works for all integer values, and the aliasing check prevents the self-zeroing issue. Time complexity O(1) and no extra space. Edge cases: negative numbers, INT_MIN, INT_MAX, and aliasing (same variable passed twice). The function must be inline and const-correct? Since we modify the parameters, `const` is not applicable to the references themselves but we can use `int&` parameters. The solution is straightforward.
