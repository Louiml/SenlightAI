Write a C++ function `int findUniqueValue(int x, int y, int z)` that takes three integers, where exactly two of them are guaranteed to be equal and the third is different, and returns the value that appears only once. For example, given `(5, 5, 3)` the function must return `3`. The input values can be any integers (including negative and zero) and the order of the three arguments is arbitrary. Do not assume any particular position for the duplicate pair.
The task is trivial because with exactly three integer inputs and the guarantee that two are equal, we can compare the three values pairwise. The main idea: if the first and second values are equal, then the unique value must be the third; otherwise, if the first and third are equal, the unique value is the second; otherwise, the first is the unique value (because if neither of the first two comparisons hold, the first must be different from both, and the second and third must be equal). This covers all possible permutations. The edge case of negative numbers or zeros works identically. Time complexity is O(1) with constant space O(1), as only three integer comparisons are performed. No additional data structures or loops are required.
#include <algorithm> // not needed, but kept for clarity (optional)

// Given three integers where exactly two are equal and one differs,
// return the integer that appears only once.
int findUniqueValue(int x, int intY, int z) {
    if (x == intY) {
        // First two are equal, so the unique value is the third.
        return z;
    }
    if (x == z) {
        // First and third are equal, so the unique value is the second.
        return intY;
    }
    // The second and third must be equal, so the first is unique.
    return x;
}
#include <cassert>

int main() {
    // Basic cases with positive numbers
    assert(findUniqueValue(5, 5, 3) == 3);
    assert(findUniqueValue(5, 3, 5) == 3);
    assert(findUniqueValue(3, 5, 5) == 3);

    // Cases with negative numbers
    assert(findUniqueValue(-1, -1, -7) == -7);
    assert(findUniqueValue(-7, -1, -7) == -1);
    assert(findUniqueValue(-7, -1, -1) == -7);

    // Cases with zero and mixed signs
    assert(findUniqueValue(0, 0, -2) == -2);
    assert(findUniqueValue(0, 4, 4) == 0);
    assert(findUniqueValue(2, 0, 0) == 2);

    // Large magnitude values
    assert(findUniqueValue(1000000000, 1000000000, -1000000000) == -1000000000);
    assert(findUniqueValue(-1000000000, 1000000000, 1000000000) == -1000000000);

    // Duplicate pair at various positions
    assert(findUniqueValue(42, 42, 7) == 7);
    assert(findUniqueValue(42, 7, 42) == 7);
    assert(findUniqueValue(7, 42, 42) == 7);

    return 0;
}
