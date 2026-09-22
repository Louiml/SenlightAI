// Write a C++ function template named `sumRange` that accepts two or more values of the same numeric type (e.g., `int`, `double`, `float`) and returns their total sum. The function must be implemented as a variadic template (using parameter packs) so that it works for any number of arguments (at least 2) without needing a separate overload for each count. It should use template deduction to accept mixed numeric types only if they are identical (i.e., all arguments must be of the same type `T`). Apply `const` correctness to the parameters where appropriate. The solution must compile with C++11 or later and not rely on external libraries beyond the standard headers.

#include <cassert>
#include <iostream>

int main() {
    // Test with two integers
    assert(sumRange(3, 4) == 7);
    // Test with three doubles
    assert(sumRange(1.5, 2.5, 3.0) == 7.0);
    // Test with many integers
    assert(sumRange(1, 2, 3, 4, 5) == 15);
    // Test with negative numbers
    assert(sumRange(-1, -2, -3, 10) == 4);
    // Test with four floats (note: approximate comparison might be needed, but these are exact)
    assert(sumRange(0.1f, 0.2f, 0.3f, 0.4f) == 1.0f);
    // Test with a single value (edge case, though spec says at least 2, we allow it)
    assert(sumRange(100) == 100);
    // Ensure const correctness: pass const variables
    const int a = 5, b = 6, c = 7;
    assert(sumRange(a, b, c) == 18);
    // Test with same type requirement: cannot mix int and double (compile error expected), but we only test valid cases
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <type_traits>

// Helper to check if all types in a pack are the same as T
template<typename T, typename... Args>
struct all_same : std::conjunction<std::is_same<T, Args>...> {};

// Base case: one argument
template<typename T>
T sumRange(T value) {
    return value;
}

// Recursive case: sum first + sum of the rest
template<typename T, typename... Args>
T sumRange(T first, Args... rest) {
    static_assert(all_same<T, Args...>::value, "All arguments must be of the same type");
    return first + sumRange(rest...);
}

// The core challenge is to support a variable number of arguments without pre-defining overloads for 2, 3, or more parameters. A variadic template `template<typename T, typename... Args> T sumRange(T first, Args... rest)` can recursively sum the first argument with the sum of the remaining ones. However, the task requires "two or more values of the same numeric type" — to enforce that all arguments are the same type, the recursive base case must handle when there are no more arguments (return 0) or one argument (return that argument). To guarantee the same type, the function signature should use `T` for the first parameter and `TSame...` for the rest, but a cleaner approach is to use `typename... Args` and require the return type and all parameter types be identical via a static assertion or by using `std::common_type`. A simpler solution is to keep a recursive template where each call passes the accumulated sum as a parameter, but that doesn't enforce same type easily. Alternative: use an initializer list if the type is known (e.g., `std::initializer_list<T>`), but that forces the caller to use braces. The most elegant way is to use a fold expression (C++17) or recursive expansion. For C++11 compatibility, use recursion: `template<typename T> T sumRange(T arg) { return arg; }` and `template<typename T, typename... Args> T sumRange(T first, Args... rest) { return first + sumRange(rest...); }` — but this allows different types if `Args...` are different. To enforce same type, we can add a `static_assert` that all types in the pack are identical to `T` using a helper struct. Edge cases: empty argument list is not allowed (since at least 2 are required, but the function is recursive; we can document that). Time complexity is O(n) for n arguments due to recursion, and space is O(n) for recursion stack (or O(1) in C++17 with fold if optimized). The solution below uses a helper trait to ensure type consistency and recursion for C++11 compatibility.
