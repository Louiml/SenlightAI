// Write a C++ function that takes three integers as input parameters and returns the minimum total distance required to move all three integers to the same value, where the distance contributed by each integer is the absolute difference between that integer and the chosen common value. The common value can be any integer, not necessarily one of the three inputs. The function should be named `minTotalDistance` and accept three `int` arguments in any order. For example, given inputs 1, 3, and 5, the optimal common value is 3, yielding distances |1-3| + |3-3| + |5-3| = 2+0+2 = 4. The function should return an `int` representing this minimum total distance. Handle duplicate values naturally, and note that the function must work for all integer values including negative numbers and zero.

#include <cassert>

int main() {
    // All equal numbers: distance 0.
    assert(minTotalDistance(5, 5, 5) == 0);

    // Simple increasing order, median is 2.
    assert(minTotalDistance(1, 2, 3) == 2); // |1-2| + |2-2| + |3-2| = 1+0+1

    // Unsorted input.
    assert(minTotalDistance(3, 1, 2) == 2);

    // Negative numbers.
    assert(minTotalDistance(-5, -1, -10) == 9); // median -5: 0+4+5

    // Mixed negative and positive.
    assert(minTotalDistance(-100, 0, 100) == 200); // median 0: 100+0+100

    // Large range, median 1000.
    assert(minTotalDistance(-1000, 1000, 0) == 2000); // median 0: 1000+1000+0

    // Duplicate values with one different.
    assert(minTotalDistance(4, 4, 10) == 6); // median 4: 0+0+6

    // Two equal negatives and a positive.
    assert(minTotalDistance(-2, -2, 8) == 10); // median -2: 0+0+10

    // Random test.
    assert(minTotalDistance(7, 3, 9) == 6); // median 7: 4+0+2

    // All negative equal.
    assert(minTotalDistance(-3, -3, -3) == 0);
}

#include <algorithm>
#include <cstdlib>

// Returns the minimum sum of absolute distances to a common point for three integers.
// The optimal point is the median of the three values.
int minTotalDistance(int a, int b, int c) {
    // Store the three values in a small array and sort them.
    int values[3] = {a, b, c};
    std::sort(values, values + 3);
    int median = values[1];  // middle element after sorting

    // Sum of absolute differences from the median.
    int total = 0;
    for (int i = 0; i < 3; ++i) {
        total += std::abs(values[i] - median);
    }
    return total;
}

// The optimal meeting point for three integers on a number line is always the median of the three values. Sorting the three numbers places the median at the middle index (index 1 after sorting). The total distance is then the sum of absolute differences between the median and each of the three numbers. This works because for any set of points on a line, the sum of absolute deviations is minimized at any median; for an odd count of points, the median is unique and optimal. Duplicate values do not change correctness because sorting handles them naturally. Edge cases include all equal numbers (distance 0) and negative numbers (absolute differences handle sign). Time complexity is O(1) because sorting three elements is constant-time (or we can use comparisons/manual sort). Space complexity is O(1). No input validation is needed because the function contract assumes exactly three integers.
