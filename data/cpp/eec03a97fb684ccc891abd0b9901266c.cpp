/*
Write a C++ function named `addValues` that accepts two values of the same generic type `T` by const reference, computes their sum, and returns the result as a `T`. The function must work for any numeric type that supports `operator+` (e.g., `int`, `double`, `float`). It should not print anything to the console; it must only return the sum. The function must be template-based so that it can be reused with different numeric types, and it must handle the case where both arguments are of the same type. Use appropriate `const` correctness (both parameters passed as `const T&`). The task is to implement only this function; no `main` is required in the solution, but test code will call it with various types and values.
*/
#include <type_traits> // not strictly required but included for completeness

// Adds two values of the same generic type T and returns the sum.
// Parameters are passed by const reference to avoid copying and ensure read-only access.
template <typename T>
T addValues(const T& x, const T& y) {
    return x + y;
}
#include <cassert>
#include <iostream>
// Assume addValues is declared above or in an included header.

int main() {
    // Integer tests
    assert(addValues(2, 3) == 5);
    assert(addValues(-5, 10) == 5);
    assert(addValues(0, 0) == 0);
    assert(addValues(100, 200) == 300);

    // Double tests
    assert(addValues(1.5, 2.5) == 4.0);
    assert(addValues(-0.25, 0.75) == 0.5);
    assert(addValues(3.14, 0.0) == 3.14);

    // Mixed types? No — function requires same type for both arguments.
    // Ensure compile-time error if mismatched types are passed (comment only).
    // assert(addValues(1, 2.0) == 3.0); // This would not compile.

    // Float tests
    assert(addValues(1.1f, 2.2f) > 3.29f && addValues(1.1f, 2.2f) < 3.31f);

    // Long tests
    assert(addValues(1000000000L, 2000000000L) == 3000000000L); // Note: may overflow on 32-bit long, but on 64-bit it's fine.

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The solution uses a function template parameterized by type `T`. The template is declared as `template <typename T>` or `template <class T>`. The function takes two parameters, each of type `const T&`, to avoid unnecessary copying and to respect const-correctness. Inside, it simply returns `x + y`, relying on the built-in `operator+` for the given type. Edge cases: if `T` is an unsigned integer type and the sum overflows, behavior is defined per C++ standard (wraps around), but the function does not guard against this; the caller must ensure values are within range. For floating-point types, the function works directly. The time complexity is O(1) since addition runs in constant time for built-in types, and space complexity is O(1) as no extra storage is used beyond the return value. The template is instantiated at compile time for each distinct type used in the test code.
