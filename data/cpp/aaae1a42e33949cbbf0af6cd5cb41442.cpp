Write a C++ function named `maximumOfThree` that takes three integer arguments and returns the largest of the three values. The function must not use any standard library functions like `std::max` or `std::min`, and must be implemented using only comparison operators and simple variable assignments. The function should be const-correct (i.e., parameters passed by value are naturally const in the parameter list, but ensure no accidental modification) and be placed in a header-ready style with necessary includes. Your solution should handle all possible integer inputs, including negative numbers, zeros, and duplicates, and return the correct maximum in every case. The function signature must be `int maximumOfThree(int a, int b, int c);` and it must be declared with `const` correctness where applicable (e.g., parameters can be marked `const` if desired, but by value semantics are fine). The task is standalone: no `main` function is required in the solution—only the free function.

int main() {
    // Basic positive values
    assert(maximumOfThree(1, 2, 3) == 3);
    assert(maximumOfThree(5, 5, 5) == 5);
    // Negative values
    assert(maximumOfThree(-1, -2, -3) == -1);
    assert(maximumOfThree(-10, -5, -7) == -5);
    // Mixed signs and zero
    assert(maximumOfThree(0, -1, 2) == 2);
    assert(maximumOfThree(4, -3, 0) == 4);
    // Duplicate maximum values
    assert(maximumOfThree(9, 9, 8) == 9);
    assert(maximumOfThree(0, 0, -1) == 0);
    // Large and small extremes
    assert(maximumOfThree(2147483647, -2147483647, 100) == 2147483647);
    assert(maximumOfThree(-2147483647, -2147483648, 0) == 0);
}

// Returns the maximum of three integers a, b, and c.
// Uses only comparison operators; no standard library helpers.
int maximumOfThree(const int a, const int b, const int c) {
    int max = a;
    if (max < b) {
        max = b;
    }
    if (max < c) {
        max = c;
    }
    return max;
}

// The solution uses a simple comparison-based algorithm: initialize a local variable `max` with the value of the first parameter `a`. Then compare this `max` with the second parameter `b`; if `b` is greater, assign `b` to `max`. Similarly, compare the current `max` with the third parameter `c`; if `c` is greater, assign `c` to `max`. After these two conditional updates, `max` holds the largest of the three input values. This works for all edge cases: negative numbers are compared normally, zeros are handled, and duplicate values do not affect correctness because the condition uses strict greater-than, so if a duplicate is the maximum, the first occurrence remains. The algorithm is straightforward and requires no extra memory. Time complexity is constant, \(O(1)\), because the number of comparisons is fixed (exactly two comparisons), regardless of input values. Space complexity is also \(O(1)\), as only a single local variable is used. The function is pure and has no side effects.
