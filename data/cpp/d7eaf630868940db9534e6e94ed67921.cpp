Write a standalone C++ function `int minimumTotalDistance(int a, int b, int c)` that, given three integers, determines the minimum possible sum of absolute differences when all three are moved to a single common integer value. Specifically, find `min_{x} ( |a - x| + |b - x| + |c - x| )` over all integers `x`. The inputs may be any 32-bit integers (positive, negative, or zero). You may assume the inputs fit in a standard `int`. Return the minimum sum as an `int`. The function must be self-contained and not depend on any global variables.
// The problem is to minimize the sum of absolute deviations from a central point for three points on a number line. The optimal `x` that minimizes the sum of absolute deviations for an odd number of points is any median of the three numbers. For three values, the median is the value that is neither the minimum nor the maximum. The minimum sum is achieved at that median (or any value between the two middle numbers, but here with exactly three numbers, the median is unique and gives the minimum). So the algorithm is: sort the three values, pick the middle one, and compute `abs(a - median) + abs(b - median) + abs(c - median)`. Since sorting three fixed elements is trivial, this is constant time. Edge cases: duplicates are handled naturally (if two or three numbers are equal, the median is the repeated value, and the sum is correct). Negative numbers work fine because absolute differences are always non-negative. Time complexity: O(1) because we only compare and swap a fixed number of elements. Space complexity: O(1) besides input.
#include <algorithm>
#include <cstdlib>

// Given three integers, return the minimum possible sum of absolute differences
// when all are shifted to a common integer value.
int minimumTotalDistance(int a, int b, int c) {
    // Arrange a, b, c in non-decreasing order.
    if (a > b) std::swap(a, b);
    if (b > c) std::swap(b, c);
    if (a > b) std::swap(a, b);
    // The optimal meeting point is the median (now b).
    int median = b;
    // Compute the sum of absolute differences from the median.
    return std::abs(a - median) + std::abs(b - median) + std::abs(c - median);
}
#include <cassert>

int main() {
    // All distinct positive numbers.
    assert(minimumTotalDistance(1, 2, 3) == 2);
    // Distinct with negatives.
    assert(minimumTotalDistance(-10, 0, 10) == 20);
    // Two equal, one different.
    assert(minimumTotalDistance(5, 5, 9) == 4);
    // All equal.
    assert(minimumTotalDistance(7, 7, 7) == 0);
    // Large spread.
    assert(minimumTotalDistance(-1000, 0, 1000) == 2000);
    // Unsorted input.
    assert(minimumTotalDistance(3, 1, 2) == 2);
    // Negative and positive with median at zero.
    assert(minimumTotalDistance(-5, 0, 5) == 10);
    // Duplicate median.
    assert(minimumTotalDistance(4, 4, -100) == 104);
}
