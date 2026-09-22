Write a C++ function that takes three integers `a`, `b`, and `c` and returns the sum of the two smallest values among them. For example, given `(5, 3, 8)`, the two smallest are `3` and `5`, so the function returns `8`. Your function must be named `sumOfTwoSmallest` and accept three `int` parameters by value. The solution must not rely on sorting or library algorithms beyond basic arithmetic; you may use `std::max` and `std::min` if desired. Ensure the function works correctly for negative numbers, zero, and duplicate values. The function should be `const`-correct (parameters are passed by value, so no `const` reference is needed, but the function itself should be `noexcept` if applicable). Provide a standalone implementation—no `main` function in the solution section—and then test it with assertions.

#include <cassert>

int sumOfTwoSmallest(int, int, int); // declaration for clarity

int main() {
    // Basic case
    assert(sumOfTwoSmallest(5, 3, 8) == 8);
    // Negative numbers
    assert(sumOfTwoSmallest(-5, -2, -10) == -15);
    // Duplicate values
    assert(sumOfTwoSmallest(4, 4, 1) == 5);
    // All equal
    assert(sumOfTwoSmallest(3, 3, 3) == 6);
    // Zero included
    assert(sumOfTwoSmallest(0, 7, 2) == 2);
    // Another negative with zero
    assert(sumOfTwoSmallest(-1, 0, 5) == -1);
    // Large values (within int range)
    assert(sumOfTwoSmallest(1000, 2000, 3000) == 3000);
    // Descending order
    assert(sumOfTwoSmallest(9, 8, 7) == 15);
    // Ascending order
    assert(sumOfTwoSmallest(1, 2, 3) == 3);
    // Mixed signs
    assert(sumOfTwoSmallest(10, -10, 0) == -10);
}

#include <algorithm>

// Return the sum of the two smallest among three integers.
int sumOfTwoSmallest(int a, int b, int c) noexcept {
    int largest = std::max(a, std::max(b, c));
    return (a + b + c) - largest;
}

// The core insight is that for any three integers, the sum of the two smallest equals the total sum minus the largest value. This is because \(\text{sum} - \text{maximum} = \text{first smallest} + \text{second smallest}\). The algorithm first computes the maximum of the three inputs using `std::max`, then returns `(a + b + c) - maximum`. Edge cases include:  
// - **Negative numbers**: The arithmetic works identically; e.g., `(-5, -2, -10)` → sum = `-17`, max = `-2`, result = `-15` (which is `-5 + -10`).  
// - **Duplicates**: If two values are equal and are the largest, e.g., `(4, 4, 1)`, max = `4`, sum = `9`, result = `5` (which is `4+1`).  
// - **All equal**: `(3,3,3)` → max = `3`, sum = `9`, result = `6`.  
// - **Large magnitudes**: Since we only use `int` arithmetic, overflow is possible if inputs are near `INT_MAX`, but the problem assumes typical small integers.  
// Time complexity is \(O(1)\) (constant number of operations), and space complexity is \(O(1)\).
