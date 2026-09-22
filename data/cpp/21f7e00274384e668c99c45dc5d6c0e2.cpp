// Write a C++ function named `medianStats` that takes a vector of integers (`std::vector<int>`) as input and returns a `std::vector<int>` containing exactly three values: the smaller of the two middle elements (i.e., the lower median), the count of how many times either median value appears in the original vector, and the number of integers from the lower median to the upper median inclusive (i.e., `upperMedian - lowerMedian + 1`). The function must handle duplicate values and odd/even-length vectors. For even lengths, the two middle elements are at indices `n/2 - 1` and `n/2` (0-based after sorting); for odd lengths, both medians are the same element at index `n/2`. The input vector can be empty, in which case return an empty vector. The function must not modify the input vector.
The problem requires analyzing the sorted version of the input to find the two central values. Start by making a copy of the vector and sorting it. If the vector is empty, return an empty vector immediately. For a non-empty vector of size `n`, the lower median is at index `(n-1)/2` and the upper median is at `n/2` (these are equal when `n` is odd). Then, iterate through the sorted vector counting how many elements equal either of these two median values. The third output is simply `upperMedian - lowerMedian + 1`. Since sorting dominates, the time complexity is \(O(n \log n)\), and space complexity is \(O(n)\) for the copy. Edge cases include: `n=1` (both medians are the same, count is 1, range is 1), all identical values (count equals `n`, range is 1), and even lengths where the two medians differ (count is number of occurrences of both values, range is the difference plus one).
#include <vector>
#include <algorithm>

// Returns {lowerMedian, frequencyOfBothMedians, upperMedian - lowerMedian + 1}
// For empty input, returns an empty vector.
std::vector<int> medianStats(const std::vector<int>& input) {
    if (input.empty()) {
        return {};
    }

    // Work on a sorted copy to avoid modifying the caller's data.
    std::vector<int> sorted = input;
    std::sort(sorted.begin(), sorted.end());

    const int n = static_cast<int>(sorted.size());
    const int lowerMedian = sorted[(n - 1) / 2];
    const int upperMedian = sorted[n / 2];

    // Count occurrences of both median values.
    int frequency = 0;
    for (const int value : sorted) {
        if (value == lowerMedian || value == upperMedian) {
            ++frequency;
        }
    }

    return {lowerMedian, frequency, upperMedian - lowerMedian + 1};
}
#include <cassert>
#include <vector>

// Declaration of the function under test.
std::vector<int> medianStats(const std::vector<int>& input);

int main() {
    // Odd number of elements, distinct values.
    assert(medianStats({1, 3, 5}) == std::vector<int>({3, 1, 1}));

    // Even number of elements, distinct values.
    assert(medianStats({1, 2, 3, 4}) == std::vector<int>({2, 2, 2}));

    // All duplicates.
    assert(medianStats({7, 7, 7}) == std::vector<int>({7, 3, 1}));

    // Single element.
    assert(medianStats({42}) == std::vector<int>({42, 1, 1}));

    // Many duplicates around the middle.
    assert(medianStats({5, 8, 8, 8, 9, 12, 14}) == std::vector<int>({8, 3, 1}));

    // Even length with overlapping frequency (two different medians).
    assert(medianStats({10, 20, 20, 30, 30, 40}) == std::vector<int>({20, 4, 11}));

    // Unsorted input.
    assert(medianStats({4, 1, 3, 2, 5}) == std::vector<int>({3, 1, 1}));

    // Negative and positive values.
    assert(medianStats({-10, -5, 0, 5, 10}) == std::vector<int>({0, 1, 1}));

    // Empty input.
    assert(medianStats({}) == std::vector<int>());

    // Very large range.
    assert(medianStats({100, 200, 300}) == std::vector<int>({200, 1, 1}));

    return 0;
}
