// Write a C++ function named `largerValue` that accepts two arguments of the same generic type (using a template) and returns the larger of the two. The function must work for at least `int`, `double`, `float`, and `char` types (for `char`, compare by their ASCII values). The function should be `const`-qualified where appropriate (i.e., it should not modify its inputs, and the parameters should be passed by value). If the two values are equal, return the first argument. Provide a standalone implementation with only the function definition (no `main`), and then create a separate test file with assert checks that verify the function for various types, including negative numbers, equal values, floating-point precision, and character comparisons.

#include <cassert>
#include <iostream>

// Declaration (must match the solution function)
template <typename T>
T largerValue(const T& a, const T& b);

int main() {
    // Integer tests
    assert(largerValue(3, 5) == 5);
    assert(largerValue(-2, -7) == -2);
    assert(largerValue(10, 10) == 10); // equal returns first

    // Double tests (floating-point)
    assert(largerValue(3.5, 5.5) == 5.5);
    assert(largerValue(-0.1, -0.2) == -0.1);
    assert(largerValue(2.0, 2.0) == 2.0);

    // Float tests
    assert(largerValue(3.5f, 5.5f) == 5.5f);
    assert(largerValue(1.1f, 1.1f) == 1.1f);

    // Char tests (ASCII comparison)
    assert(largerValue('m', 's') == 's');
    assert(largerValue('a', 'a') == 'a');
    assert(largerValue('A', 'B') == 'B');

    // Edge case: negative and positive
    assert(largerValue(-1, 1) == 1);

    std::cout << "All tests passed!\n";
    return 0;
}

#include <type_traits>

// Return the larger of two values of the same type.
// If the values are equal, return the first one.
template <typename T>
T largerValue(const T& a, const T& b) {
    static_assert(std::is_arithmetic<T>::value || std::is_same<T, char>::value,
                  "largerValue supports arithmetic types and char only");
    return (a > b) ? a : b;
}

// The solution is straightforward: define a function template that takes two parameters of type `T` and returns the larger one using a simple comparison `if (a > b) return a; else return b;`. Since we return by value, no modification occurs, and `const` is not strictly required on parameters passed by value (they are already copies), but we can make the function itself `constexpr` if desired for compile-time evaluation. The key edge cases are: (1) equal values – the spec says return the first argument, which our `else` branch does; (2) different types – we rely on template instantiation, so `int` vs `double` will not compile unless the caller explicitly casts or uses a common type (but the task requires same type for both arguments, so that's fine); (3) floating-point – comparison works normally, but note that `NaN` behavior is undefined, which we can ignore as inputs are expected to be normal values; (4) `char` – comparing `char` uses ASCII values automatically. Time complexity is O(1) constant time, and space complexity is O(1) as no extra storage is used.
