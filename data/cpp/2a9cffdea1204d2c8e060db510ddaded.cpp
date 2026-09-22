// Write a C++ function `int largestOfThree(int a, int b, int c)` that returns the largest of three integers using only the formula `(x + y + abs(x - y)) / 2` (the standard max-of-two expression) applied pairwise, without directly using `std::max`, comparison operators (`>` or `<`), or conditional statements inside the function. The function must handle all integer values within the range of a 32-bit `int`, including negative numbers, zero, and duplicates. The caller will provide three integers, and the function must return the maximum value. The formula must be used exactly as given for each pairwise combination, and the solution must not rely on overflow-prone arithmetic beyond what the formula requires (assume inputs are such that `a + b + abs(a - b)` does not overflow, i.e., inputs are within a reasonable range). No `main` function should be included in the solution; only the function definition.

#include <cassert>

int largestOfThree(int, int, int); // Declaration for testing

int main() {
    // Basic positive numbers
    assert(largestOfThree(1, 2, 3) == 3);
    assert(largestOfThree(3, 1, 2) == 3);
    assert(largestOfThree(2, 3, 1) == 3);

    // Negative numbers
    assert(largestOfThree(-1, -2, -3) == -1);
    assert(largestOfThree(-3, -1, -2) == -1);
    assert(largestOfThree(-5, -10, -7) == -5);

    // Mixed signs
    assert(largestOfThree(-1, 0, 5) == 5);
    assert(largestOfThree(-10, 0, 3) == 3);
    assert(largestOfThree(-2, -1, 0) == 0);

    // Duplicates
    assert(largestOfThree(5, 5, 5) == 5);
    assert(largestOfThree(2, 2, 1) == 2);
    assert(largestOfThree(1, 2, 2) == 2);

    // Zero and large values
    assert(largestOfThree(0, 0, 0) == 0);
    assert(largestOfThree(1000000, 999999, 999998) == 1000000);
    assert(largestOfThree(-1000000, -999999, -999998) == -999998);

    // Order variations
    assert(largestOfThree(5, 3, 4) == 5);
    assert(largestOfThree(4, 5, 3) == 5);
    assert(largestOfThree(3, 4, 5) == 5);

    return 0;
}

#include <cstdlib> // for std::abs

// Returns the largest of three integers using only the pairwise max formula.
int largestOfThree(int a, int b, int c) {
    // Maximum of a and b using the given formula.
    const int maxAB = (a + b + std::abs(a - b)) / 2;
    // Maximum of maxAB and c.
    const int largest = (maxAB + c + std::abs(maxAB - c)) / 2;
    return largest;
}

// The core idea is to repeatedly apply the pairwise maximum formula `(x + y + abs(x - y)) / 2`, which correctly yields the larger of `x` and `y` without direct comparison. First compute `maxAB` as the maximum of `a` and `b` using the formula. Then compute the final maximum by applying the same formula to `maxAB` and `c`. This two-step process works for any three integers, including negatives and duplicates. Edge cases: if all values are equal, the formula returns the same value; if one value is negative and another positive, the absolute value difference handles it correctly; if `a` and `b` are equal, the formula returns that value. Time complexity is O(1) with a fixed number of operations. Space complexity is O(1). No overflow occurs under the given assumption, but to be safe we could use `long long` for intermediate computation, but the task specifies `int` and reasonable inputs, so the standard `int` arithmetic is acceptable.
