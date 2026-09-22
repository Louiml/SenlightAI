// Create a standalone C++ function that takes two integers and returns the larger one, but the function must be declared in a header file named `max.h` and defined in a separate implementation file `max.cpp`. The function should be named `max` (matching the usage in the provided snippet), accept two `const int` parameters, and return an `int` representing the maximum value. Handle edge cases where both integers are equal (in which case either value is correct). The task requires you to write both the header declaration and the implementation definition, but for the purposes of this exercise, provide only the function definition in a self-contained implementation (without a header guard, since it's just a definition). Ensure the function works for all possible `int` values, including negative numbers, zero, and `INT_MAX`/`INT_MIN`. The function should not use any standard library functions like `std::max` — implement the comparison manually using the ternary operator or `if-else`.

The solution approach is straightforward: compare two integers and return the larger. Since we are to implement this manually, we can use a simple conditional expression: `return (a > b) ? a : b;`. This handles all integer values correctly, including equal values (returns either), and works with negative numbers. There are no significant edge cases beyond the basic comparison — the main concern is ensuring that the function signature matches exactly (two `const int` parameters and returning `int`). The time complexity is O(1) constant time, and space complexity is O(1) as no additional storage is used. The function is `const`-correct because parameters are passed by value but marked `const` to prevent modification inside the function (though it's not strictly necessary, it matches the task's instruction to apply `const` correctness). No headers are needed beyond potentially `<climits>` if testing extreme values, but the solution itself is self-contained.

// Return the larger of two integers.
// Parameters are passed by value, but declared const to prevent modification.
int max(const int a, const int b) {
    return (a > b) ? a : b;
}

#include <cassert>
#include <climits>

// Declare the function (it would normally be in max.h)
int max(const int a, const int b);

int main() {
    assert(max(10, 20) == 20);
    assert(max(20, 10) == 20);
    assert(max(-5, -10) == -5);
    assert(max(0, 0) == 0);
    assert(max(INT_MAX, INT_MIN) == INT_MAX);
    assert(max(-3, -3) == -3);
    assert(max(100, 99) == 100);
    assert(max(-100, -99) == -99);
    assert(max(7, 7) == 7);
    assert(max(1, -1) == 1);
    return 0;
}
