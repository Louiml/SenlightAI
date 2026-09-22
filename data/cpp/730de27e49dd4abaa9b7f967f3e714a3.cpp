/*
Write a C++ function named `findMedian` that takes three integers as parameters and returns the median (middle) value among them. The function must not rely on any standard library sorting algorithms and must handle duplicate values correctly (e.g., if two numbers are equal and the third is larger, the median is the duplicate value). The function should be a free function with a descriptive name, use `const` parameters, and return an integer. The function specification is: `int findMedian(const int a, const int b, const int c);`. You are not required to write a main function in the solution, but you must provide test code separately that verifies the function’s correctness.
*/

#include <algorithm>

// Return the median value among three integers.
// Uses the identity: median = (a + b + c) - min - max.
int findMedian(const int a, const int b, const int c) {
    const int minVal = std::min(a, std::min(b, c));
    const int maxVal = std::max(a, std::max(b, c));
    return a + b + c - minVal - maxVal;
}

#include <cassert>

int findMedian(const int a, const int b, const int c);

int main() {
    // All distinct, various orders
    assert(findMedian(3, 1, 2) == 2);
    assert(findMedian(1, 3, 2) == 2);
    assert(findMedian(2, 1, 3) == 2);
    assert(findMedian(2, 3, 1) == 2);
    assert(findMedian(1, 2, 3) == 2);
    assert(findMedian(3, 2, 1) == 2);
    // Duplicates
    assert(findMedian(5, 5, 10) == 5);
    assert(findMedian(10, 5, 5) == 5);
    assert(findMedian(5, 10, 5) == 5);
    // All equal
    assert(findMedian(7, 7, 7) == 7);
    // Negative numbers
    assert(findMedian(-1, -5, -3) == -3);
    assert(findMedian(-10, 0, -20) == -10);
    // Large values
    assert(findMedian(1000000, 2000000, 1500000) == 1500000);
    return 0;
}

// The median is the value that lies between the other two. A direct comparison approach works by checking for each candidate whether it lies between the other two values. For each input `a`, `b`, `c`, we test conditions based on strict inequalities: for `a` to be the median, either `(b < a && a < c)` or `(c < a && a < b)`. Similarly for `b`. If neither `a` nor `b` satisfies this, then `c` must be the median. Edge cases involving duplicates need careful handling: for example, if two values are equal, the inequality may fail for strict comparisons. A more robust method is to use mathematical properties: the median is the sum of all three minus the minimum and maximum. That is `a + b + c - min(a,b,c) - max(a,b,c)`. This works even with duplicates because subtracting both extremes always leaves the middle value (including duplicates). Regardless of equality, the remaining value is correct. Time complexity is O(1) since we perform a fixed number of comparisons/operations, and space complexity is O(1) as no extra storage is used beyond a few variables.
