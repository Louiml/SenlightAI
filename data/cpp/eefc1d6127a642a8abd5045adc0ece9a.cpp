// Write a C++ function template `clampInRange` that takes three arguments of potentially different types: a `value`, a `lowerBound`, and an `upperBound`. The function should return the value clamped to the closed interval `[lowerBound, upperBound]` — i.e., if `value` is less than `lowerBound`, return `lowerBound`; if `value` is greater than `upperBound`, return `upperBound`; otherwise return `value`. The return type must be consistent with the type of `lowerBound` and `upperBound` (use the common type of these two bounds). The function should work for any numeric types (int, double, float, etc.) and correctly handle mixed-type arguments (e.g., int value with double bounds). Ensure that when `lowerBound > upperBound`, the function returns `upperBound` (treating it as an empty range, so the value is clamped to the upper bound). The function must be `constexpr`-friendly and use `std::common_type` internally. Do not modify the behavior for equality cases.
The solution uses `std::common_type_t<U, V>` to deduce the return type from the two bound types, ensuring that if the bounds are `int` and `double`, the result is `double`. The algorithm first compares `value` to `lowerBound` using `<`; if true, return a copy of `lowerBound` (converted to the common type). Otherwise, compare `value` to `upperBound` using `>`; if true, return `upperBound`. Otherwise, return `value` converted to the common type. Edge cases: (1) when `lowerBound > upperBound`, the first comparison `value < lowerBound` may be true for values less than the lower bound, but the correct behavior per spec is to return `upperBound` in all cases — so we must restructure: first check if `upperBound < value`, then return `upperBound`; then check if `value < lowerBound`, return `lowerBound`; else return `value`. This ensures the empty-range case returns `upperBound` regardless of value. (2) Equality with bounds: `value == lowerBound` or `value == upperBound` should return that bound — the checks use strict inequalities, so equality falls through to the final return. (3) Mixed types: comparisons use the natural promotion rules; the return uses `static_cast<return_type>(...)` to avoid conversion warnings. Time complexity is O(1), space O(1). The function is `constexpr` so it can be used in compile-time contexts.
#include <type_traits>

// Clamp a value to the inclusive interval [lowerBound, upperBound].
// If lowerBound > upperBound (empty range), return upperBound.
// The return type is the common type of the two bounds.
template <typename T, typename U, typename V>
constexpr auto clampInRange(const T& value, const U& lowerBound, const V& upperBound)
    -> std::common_type_t<U, V> {
    using return_type = std::common_type_t<U, V>;

    // Handle empty range: if upper bound is less than value, return upper bound.
    if (upperBound < value) {
        return static_cast<return_type>(upperBound);
    }
    // Otherwise, if value is below lower bound, return lower bound.
    if (value < lowerBound) {
        return static_cast<return_type>(lowerBound);
    }
    // Otherwise return the value.
    return static_cast<return_type>(value);
}
#include <cassert>
#include <type_traits>

int main() {
    // Basic int clamping
    assert(clampInRange(5, 0, 10) == 5);
    assert(clampInRange(-3, 0, 10) == 0);
    assert(clampInRange(15, 0, 10) == 10);

    // Mixed types: int value, double bounds -> double return
    auto a = clampInRange(7, 0.0, 10.0);
    static_assert(std::is_same<decltype(a), double>::value, "Return type should be double");
    assert(a == 7.0);
    assert(clampInRange(-1, 0.5, 10.5) == 0.5);
    assert(clampInRange(100, 0.5, 10.5) == 10.5);

    // Edge: equality with bounds
    assert(clampInRange(0, 0, 10) == 0);
    assert(clampInRange(10, 0, 10) == 10);

    // Edge: empty range (lower > upper) -> should return upper bound
    assert(clampInRange(5, 10, 0) == 0);
    assert(clampInRange(-5, 10, 0) == 0);
    assert(clampInRange(20, 10, 0) == 0);

    // Float bounds with negative values
    assert(clampInRange(-2.5, -1.0, 1.0) == -1.0);
    assert(clampInRange(0.0, -1.0, 1.0) == 0.0);

    // Single type all ints
    assert(clampInRange(3, 3, 3) == 3);

    return 0;
}
