Write a C++ function named `findMaximum` that takes two integer values as input and returns the larger of the two. The function must handle any integer values, including negative numbers, zeros, and equal values. For equal values, it should return either one (as they are both the maximum). Additionally, write a second function `findMinimum` that takes two integers and returns the smaller value, again handling all integer cases including negatives and ties. The solution should be self-contained, avoid global variables, and use `const` correctness for the parameters. The task is to demonstrate clean conditional logic without relying on library `std::max` or `std::min` functions.
// The problem is a straightforward comparison of two integers. The main algorithm involves reading two integer values (which could be negative, zero, positive, or equal) and selecting the maximum or minimum based on a simple `if`-`else` comparison. For `findMaximum`, if the first value is greater than the second, return the first; otherwise, return the second (this correctly handles ties because either value is acceptable, but returning the second is fine). For `findMinimum`, the logic is reversed. No special edge cases exist beyond ties, which are harmless. Both functions run in constant time O(1) and use constant space O(1), as no additional data structures are created. The original snippet used global variables and mixed input/output with logic, but the task requires a clean, reusable free function without side effects.
#include <iostream>

// Returns the larger of two integer values.
// Handles negative, zero, and equal values correctly.
int findMaximum(const int a, const int b) {
    if (a > b) {
        return a;
    }
    return b;
}

// Returns the smaller of two integer values.
// Handles negative, zero, and equal values correctly.
int findMinimum(const int a, const int b) {
    if (a < b) {
        return a;
    }
    return b;
}
#include <cassert>

int main() {
    assert(findMaximum(10, 5) == 10);
    assert(findMaximum(-3, -7) == -3);
    assert(findMaximum(0, 0) == 0);
    assert(findMaximum(100, -100) == 100);
    assert(findMaximum(-1, 2) == 2);

    assert(findMinimum(10, 5) == 5);
    assert(findMinimum(-3, -7) == -7);
    assert(findMinimum(0, 0) == 0);
    assert(findMinimum(100, -100) == -100);
    assert(findMinimum(-1, 2) == -1);

    assert(findMaximum(7, 7) == 7);
    assert(findMinimum(7, 7) == 7);

    return 0;
}
