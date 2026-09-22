// Write a C++ function named `canMakeAllEqual` that takes five integers: `a`, `b`, `c`, `d`, and `increment` (all positive or zero). The function should return `true` if it is possible to add the `increment` value to exactly one of the first four integers (or to none of them) so that all four integers become equal. Adding the increment to more than one integer is not allowed. The function must return `false` otherwise. For example, with `a=1, b=1, c=1, d=2, increment=1`, adding `1` to `d` makes all equal to `1`, so return `true`. But with `a=1, b=2, c=3, d=4, increment=1`, no single addition or no addition can make them all equal, so return `false`. The function should be self-contained, use `const` for parameters where appropriate, and not rely on global state.

#include <cassert>

int main() {
    // Already equal
    assert(canMakeAllEqual(5, 5, 5, 5, 3) == true);
    // Add to d
    assert(canMakeAllEqual(1, 1, 1, 2, 1) == true);
    // Add to a
    assert(canMakeAllEqual(2, 3, 3, 3, 1) == true);
    // Add to b
    assert(canMakeAllEqual(4, 3, 4, 4, 1) == true);
    // Add to c
    assert(canMakeAllEqual(7, 7, 6, 7, 1) == true);
    // No possible
    assert(canMakeAllEqual(1, 2, 3, 4, 1) == false);
    // Increment zero, not equal
    assert(canMakeAllEqual(1, 2, 1, 1, 0) == false);
    // Increment zero, equal
    assert(canMakeAllEqual(2, 2, 2, 2, 0) == true);
    // All zeros, increment positive
    assert(canMakeAllEqual(0, 0, 0, 0, 5) == true);
    // Large numbers
    assert(canMakeAllEqual(1000000, 1000000, 1000000, 999999, 1) == true);
    return 0;
}

#include <cstdbool> // for bool

// Returns true if we can make a, b, c, d all equal by adding 'increment' to at most one of them (or none).
bool canMakeAllEqual(const int a, const int b, const int c, const int d, const int increment) {
    // Case 0: No addition needed
    if (a == b && b == c && c == d) {
        return true;
    }
    // Case 1: Add increment to a
    if (a + increment == b && b == c && c == d) {
        return true;
    }
    // Case 2: Add increment to b
    if (a == b + increment && b + increment == c && c == d) {
        return true;
    }
    // Case 3: Add increment to c
    if (a == b && b == c + increment && c + increment == d) {
        return true;
    }
    // Case 4: Add increment to d
    if (a == b && b == c && c == d + increment) {
        return true;
    }
    return false;
}

// The problem asks whether we can make all four numbers equal by adding the same non-negative value (the `increment`) to exactly one of the four numbers, or to none. We need to check five distinct scenarios: (1) all four are already equal, (2) adding the increment to `a` makes all equal, (3) adding to `b` makes all equal, (4) adding to `c` makes all equal, (5) adding to `d` makes all equal. For each scenario, we must verify that after the addition, all four numbers are identical. A straightforward approach is to check each case explicitly: first test if `a==b && b==c && c==d`; then test if `(a+increment)==b && b==c && c==d`; similarly for `b`, `c`, and `d`. Since the numbers are integers and the increment is non-negative, we don't need to worry about negative sums. Edge cases include when the increment is zero (then the problem reduces to checking if all are initially equal), and when the initial numbers are already equal (returns true without any addition). The time complexity is O(1) because we perform a constant number of comparisons. The space complexity is O(1) as we only use a few constant variables. The solution is robust and handles all possible integer inputs without overflow concerns since the numbers are not large in the context of typical tasks, but even if they were, the addition is done once and we compare directly.
