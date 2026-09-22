// Write a C++ function named `largestOfTwo` that accepts two arguments of the same type and returns the "larger" one. The function must be implemented as a function template, with two important specializations: (1) an explicit instantiation for `float` arguments, and (2) an explicit specialization for `const char*` arguments, where "larger" is determined lexicographically using `strcmp` (case-sensitive). The generic template should work for any type that supports the `>` operator. The function must not modify its inputs and should handle `nullptr` for `const char*` gracefully (treat `nullptr` as smaller than any non-null string, and if both are `nullptr`, return either). You do not need to provide a `main` function, but your solution must include the template, the explicit instantiation, and the specialization exactly as described.
The core challenge is providing a generic `Max`-like function that works for built-in numeric types and also for C‑style strings, where the default `>` operator compares pointers rather than string contents. The solution uses a function template with a generic implementation that relies on `operator>`. For `const char*`, we provide an explicit specialization that uses `std::strcmp` to compare strings lexicographically. The `nullptr` handling: if one is `nullptr` and the other is not, the non-null is larger; if both are `nullptr`, they are equal (we can return `x` or `y`). For numeric types, the generic template works directly. The explicit instantiation for `float` ensures the template is instantiated for that type even if not used in the test, which is a common requirement to verify understanding. Edge cases: for `const char*`, we must include `<cstring>` for `strcmp`; for generic types, we require the `>` operator, which works for all built-in numeric types and most user-defined types that define it. Time complexity is O(1) for numeric types and O(min(length of strings)) for `const char*` due to `strcmp`. Space complexity is O(1) for all cases.
#include <cstring>  // for strcmp

// Generic template: returns the larger of x and y using operator>.
template<typename T>
T largestOfTwo(T x, T y) {
    return x > y ? x : y;
}

// Explicit instantiation for float (forces code generation for float).
template float largestOfTwo(float x, float y);

// Explicit specialization for const char*: lexicographic comparison, null-safe.
template<>
const char* largestOfTwo<const char*>(const char* x, const char* y) {
    if (x == nullptr) return y;          // null is considered smaller
    if (y == nullptr) return x;
    return std::strcmp(x, y) > 0 ? x : y;
}
#include <cassert>
#include <cstring>
#include <string>

// Declare the template and specialization (must match the solution exactly).
template<typename T>
T largestOfTwo(T x, T y);
template<>
const char* largestOfTwo<const char*>(const char* x, const char* y);

int main() {
    // Numeric generic tests
    assert(largestOfTwo(5, 3) == 5);
    assert(largestOfTwo(2, 9) == 9);
    assert(largestOfTwo(7.5, 7.5) == 7.5);

    // Float explicit instantiation (should work even if not directly used)
    assert(largestOfTwo(1.5f, 2.5f) == 2.5f);
    assert(largestOfTwo(3.14f, 3.14f) == 3.14f);

    // const char* specialization tests
    const char* a = "apple";
    const char* b = "banana";
    assert(std::strcmp(largestOfTwo(a, b), "banana") == 0);
    assert(std::strcmp(largestOfTwo("zebra", "apple"), "zebra") == 0);

    // Case sensitivity: uppercase has lower ASCII values than lowercase
    assert(std::strcmp(largestOfTwo("Apple", "apple"), "apple") == 0);

    // Null handling
    const char* nullPtr = nullptr;
    const char* world = "world";
    assert(largestOfTwo(nullPtr, world) == world);
    assert(largestOfTwo(world, nullPtr) == world);
    assert(largestOfTwo(nullPtr, nullPtr) == nullptr); // both null, returns y (which is null)

    // Duplicate strings
    const char* same1 = "same";
    const char* same2 = "same";
    assert(std::strcmp(largestOfTwo(same1, same2), "same") == 0);
}
