Write a C++ function named `mostFrequentElement` that takes a vector of integers as input and returns a `std::pair<int, int>` where the first element is the most frequently occurring value and the second element is its frequency. In case of a tie (multiple values with the same highest frequency), return the smallest value among them. The function should work for empty input by returning `{0, 0}` (indicating no elements). The algorithm must sort the input internally (not modifying the original vector) and count consecutive duplicates. You may assume all integers are within the `int` range.
// The solution sorts a copy of the input vector in ascending order (using `std::sort`). After sorting, identical values become adjacent. We then iterate through the sorted array, maintaining a `currentValue` and `currentCount`. Whenever the value changes, we compare the current count to the best (most frequent) count found so far. If the current count is greater than the best count, we update the best value and count. If it's equal, we keep the smaller value (since we process in ascending order, the first time we see a count equal to the best, we would already have the smaller value, so we only update when strictly greater). At the end, if the best count remains 0 (empty input), return `{0, 0}`. Edge cases include a single element, all identical elements, and ties. Time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(n)\) because we copy the input.
#include <vector>
#include <algorithm>
#include <utility>

// Return the most frequent element and its frequency.
// On ties, return the smallest such element. Empty input returns {0,0}.
std::pair<int, int> mostFrequentElement(const std::vector<int>& input) {
    if (input.empty()) {
        return {0, 0};
    }

    std::vector<int> sorted = input; // copy to avoid modifying caller's data
    std::sort(sorted.begin(), sorted.end());

    int bestValue = sorted[0];
    int bestCount = 1;

    int currentValue = sorted[0];
    int currentCount = 1;

    for (size_t i = 1; i < sorted.size(); ++i) {
        if (sorted[i] == currentValue) {
            ++currentCount;
        } else {
            if (currentCount > bestCount) {
                bestCount = currentCount;
                bestValue = currentValue;
            }
            // For ties (currentCount == bestCount), keep the smaller value,
            // which is already bestValue because we encounter values in ascending order.
            currentValue = sorted[i];
            currentCount = 1;
        }
    }

    // Handle the last group
    if (currentCount > bestCount) {
        bestCount = currentCount;
        bestValue = currentValue;
    }

    return {bestValue, bestCount};
}
#include <cassert>
#include <vector>
#include <utility>

// Assume the solution function is declared above.

int main() {
    assert(mostFrequentElement({}) == std::make_pair(0, 0));
    assert(mostFrequentElement({5}) == std::make_pair(5, 1));
    assert(mostFrequentElement({1, 2, 3, 4}) == std::make_pair(1, 1));
    assert(mostFrequentElement({2, 2, 2, 3, 3}) == std::make_pair(2, 3));
    assert(mostFrequentElement({3, 3, 1, 2, 2, 2}) == std::make_pair(2, 3));
    assert(mostFrequentElement({-1, -1, 1, 1}) == std::make_pair(-1, 2)); // tie -> smaller
    assert(mostFrequentElement({7, 7, 7, 7}) == std::make_pair(7, 4));
    assert(mostFrequentElement({4, 5, 4, 5, 4, 5, 6}) == std::make_pair(4, 3)); // tie -> 4
    assert(mostFrequentElement({9, -2, 9, -2, -2}) == std::make_pair(-2, 3));
}
