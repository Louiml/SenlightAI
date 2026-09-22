Write a C++ function named `transformWithTemplate` that takes a single integer parameter `value` and returns an integer. The function must internally call a static helper function `doubleValue` that doubles its input, and then use a template function `applyTransformation` that takes a value of any type `T` and returns the result of calling `doubleValue` with that value (which must be valid for the given type). The outer function should simply return the result of calling `applyTransformation` with the input `value`. The function should work for both positive and negative integers, and must be declared with proper `const` correctness where applicable (though parameters are passed by value, so no `const` on parameters, but the function itself can be non-const). The solution must not use any global variables or external dependencies beyond standard headers.

#include <cassert>

// The solution function is assumed to be declared above.
// Including the solution here for completeness, but in practice it would be in a header or above main.
static int doubleValue(int x) { return x * 2; }
template <typename T>
T applyTransformation(T value) { return doubleValue(value); }
int transformWithTemplate(int value) { return applyTransformation(value); }

int main() {
    assert(transformWithTemplate(1) == 2);
    assert(transformWithTemplate(0) == 0);
    assert(transformWithTemplate(-5) == -10);
    assert(transformWithTemplate(100) == 200);
    assert(transformWithTemplate(-1000) == -2000);
    assert(transformWithTemplate(7) == 14);
    return 0;
}

#include <cstddef>  // For size_t, if needed, but not required here.

// Static helper function that doubles an integer.
static int doubleValue(int x) {
    return x * 2;
}

// Template function that applies the transformation to any type T.
template <typename T>
T applyTransformation(T value) {
    return doubleValue(value);
}

// Outer function that transforms an integer using the template.
// Returns the doubled value of the input.
int transformWithTemplate(int value) {
    return applyTransformation(value);
}

// The core idea is to mimic the structure of the provided snippet, which demonstrates a static free function, a template function that calls the static function, and an outer function that calls the template function with a specific type. The algorithm is straightforward: `applyTransformation` is a template that accepts any type `T`, and inside it calls `doubleValue` with that value. Since `doubleValue` is defined to accept an `int`, the template will only work when `T` is convertible to `int` (or exactly `int`), but for our task we only call it with `int`. The static helper `doubleValue` simply returns `value * 2`. The outer function `transformWithTemplate` takes an `int` and returns the result of `applyTransformation(value)`. Edge cases include negative integers, zero, and large values that might cause overflow; overflow is possible if the doubled value exceeds `INT_MAX` or goes below `INT_MIN`, but for the test cases we choose values that avoid overflow. Time complexity is O(1) and space complexity is O(1), as no additional data structures are used.
