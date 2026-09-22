/*
Write a C++ function named `findGreatest` that accepts three integers by value and returns the greatest of the three as an integer. The function must handle cases where two or three numbers are equal, in which case the function should still return the correct maximum value (e.g., if the three numbers are 5, 5, 2, it must return 5). The function must be declared with appropriate `const` correctness, meaning all parameters are passed by value and no object is modified inside. Do not write a `main` function—only provide the free function definition. The function should be robust for any integer values, including negative numbers, zero, and large values within the `int` range.
*/
// Returns the greatest of three integers.
// Works for all int values, handles ties correctly.
int findGreatest(int a, int b, int c) {
    if (a >= b && a >= c) {
        return a;
    }
    if (b >= a && b >= c) {
        return b;
    }
    return c;
}
int main() {
    // Basic distinct values
    assert(findGreatest(1, 2, 3) == 3);
    assert(findGreatest(3, 2, 1) == 3);
    assert(findGreatest(2, 3, 1) == 3);
    // Negative numbers
    assert(findGreatest(-1, -2, -3) == -1);
    assert(findGreatest(-3, -2, -1) == -1);
    // Zero and negative
    assert(findGreatest(0, -1, -2) == 0);
    // Ties (two equal maxima)
    assert(findGreatest(5, 5, 2) == 5);
    assert(findGreatest(5, 2, 5) == 5);
    assert(findGreatest(2, 5, 5) == 5);
    // All equal
    assert(findGreatest(7, 7, 7) == 7);
    // Large values
    assert(findGreatest(1000000, 999999, 1000001) == 1000001);
    // Mixed signs
    assert(findGreatest(-10, 0, 10) == 10);
}
// The solution is straightforward: compare three integers and return the largest. The main algorithm uses pairwise comparisons: first compare `a` and `b` to find a temporary maximum, then compare that temporary maximum with `c` to determine the overall greatest. Alternatively, use the ternary operator or `std::max` from `<algorithm>` (but the task expects hand-rolled logic to be educational). Edge cases include all equal values, two equal maxima, and negative numbers—these are naturally handled because using `>` and `<` correctly works for equality (if `a == b == c`, the condition `a > b` is false, but we still end up returning the correct maximum because we check all possibilities). Time complexity is O(1) since only a fixed number of constant-time comparisons are made. Space complexity is O(1) as no auxiliary data structures are used, only a few local variable slots for the temporary maximum if needed. The function should be `const`-qualified where applicable (though it's not a member function, we can still ensure parameters are passed by value and we don't modify them). The implementation must be self-contained, including necessary headers like `<algorithm>` if using `std::max`, but provided solution will use plain comparisons to be educational and dependency-free except for no includes needed (or only `<algorithm>` if desired). For clarity, a simple nested conditional approach is recommended.
