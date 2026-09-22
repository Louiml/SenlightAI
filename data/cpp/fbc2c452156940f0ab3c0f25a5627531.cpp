Write a C++ function named `findLargestAmongThree` that takes three integers as parameters and returns the largest of the three as an integer. The function must correctly handle cases where two or all three numbers are equal, ensuring the largest value (or the common value when all are equal) is always returned. Do not use any standard library containers or algorithms—only basic arithmetic and conditional logic. The function should be const-correct and have no side effects (i.e., it should not print anything or read from standard input). The solution must be self-contained and compilable as part of a larger program.
The core algorithm is a straightforward nested conditional comparison. Initialize the result to the first number, then compare with the second and third numbers, updating the result whenever a larger value is found. This approach naturally handles duplicates: if `a` is the largest and equals `b`, the condition `if (b > result)` is false, so `a` remains the result. If all three are equal, the function returns that common value. Edge cases include negative numbers, zeros, and extreme values like `INT_MAX` and `INT_MIN`, all of which are handled correctly by simple relational comparisons. Time complexity is O(1) (constant number of comparisons, at most 2), and space complexity is O(1) (only a few local integer variables). No auxiliary data structures are used.
// Returns the largest of three integers.
// Correctly handles duplicates and all integer values, including extremes.
int findLargestAmongThree(int a, int b, int c) {
    int largest = a;          // Start with the first number
    if (b > largest) {        // Update if second is larger
        largest = b;
    }
    if (c > largest) {        // Update if third is larger
        largest = c;
    }
    return largest;
}
#include <cassert>
#include <climits>

int findLargestAmongThree(int a, int b, int c); // declaration from solution

int main() {
    // Basic distinct values
    assert(findLargestAmongThree(1, 2, 3) == 3);
    assert(findLargestAmongThree(5, -1, 0) == 5);
    assert(findLargestAmongThree(-10, -20, -5) == -5);

    // Duplicates
    assert(findLargestAmongThree(4, 4, 2) == 4);
    assert(findLargestAmongThree(2, 4, 4) == 4);
    assert(findLargestAmongThree(4, 2, 4) == 4);
    assert(findLargestAmongThree(7, 7, 7) == 7);

    // Extreme values
    assert(findLargestAmongThree(INT_MAX, 0, -1) == INT_MAX);
    assert(findLargestAmongThree(INT_MIN, INT_MIN, 0) == 0);

    return 0;
}
