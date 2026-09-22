Write a C++ function that takes a non-empty vector of integers representing the heights of candles on a birthday cake and returns the number of candles that are the tallest. The candle heights may include negative numbers, duplicates, and the vector can be very large. The function must determine the maximum height efficiently and count how many times that maximum appears. For example, given heights `[4, 4, 1, 3]`, the tallest candles have height `4` and appear twice, so the result is `2`. If all candles have the same height, the result equals the size of the vector. The function should not modify the input vector and should handle edge cases such as a single candle or all negative values.

#include <cassert>
#include <vector>

int main() {
    // Standard case with duplicates
    assert(countTallestCandles({4, 4, 1, 3}) == 2);

    // All identical heights
    assert(countTallestCandles({5, 5, 5, 5}) == 4);

    // Single candle
    assert(countTallestCandles({7}) == 1);

    // Negative values, maximum is positive
    assert(countTallestCandles({-1, -5, -3}) == 1);

    // Negative values, maximum is negative
    assert(countTallestCandles({-2, -2, -10, -1}) == 1);

    // Large values and multiple occurrences of maximum
    assert(countTallestCandles({1000000, 999999, 1000000, 1}) == 2);

    // Unsorted vector with maximum appearing once
    assert(countTallestCandles({3, 1, 9, 4}) == 1);

    // Vector with maximum appearing at both ends
    assert(countTallestCandles({10, 0, -1, 10, 2}) == 2);

    // Empty vector (though task says non-empty, guard works)
    assert(countTallestCandles({}) == 0);

    // All negative identical
    assert(countTallestCandles({-4, -4, -4}) == 3);
}

#include <vector>
#include <climits>
#include <algorithm>

// Count how many candles have the tallest height.
// Returns the number of occurrences of the maximum value in the input vector.
int countTallestCandles(const std::vector<int>& candleHeights) {
    if (candleHeights.empty()) {
        return 0;
    }

    int maxHeight = INT_MIN;
    for (int height : candleHeights) {
        maxHeight = std::max(maxHeight, height);
    }

    int tallestCount = 0;
    for (int height : candleHeights) {
        if (height == maxHeight) {
            ++tallestCount;
        }
    }

    return tallestCount;
}

// The solution involves two linear passes over the input vector. In the first pass, we find the maximum element by iterating through the vector and updating a running maximum (initialized to the smallest possible integer value, e.g., `INT_MIN` from `<climits>`). In the second pass, we count how many elements equal that maximum. Since we only need the maximum and its frequency, no sorting or extra data structures are required. Edge cases include an empty vector (though the task states non-empty, we can still guard by returning 0), a vector with all identical values (the count equals the vector size), negative values (the maximum is still correctly identified using `INT_MIN` as initial guess), and single-element vectors (the count is 1). Time complexity is O(n) for the two passes, and space complexity is O(1) auxiliary, not counting the input vector itself. The function should take the vector by const reference to avoid copying and ensure const correctness.
