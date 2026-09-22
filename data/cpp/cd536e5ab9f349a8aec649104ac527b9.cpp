Write a C++ function that takes a vector of integers and returns the number of consecutive elements after the median (in sorted order) that are equal to the median value. The median is defined as the element at index (n/2) for odd n, and at index (n/2 - 1) for even n (i.e., the lower median for even-sized arrays). The function should first sort the input vector in non-decreasing order, then determine the median index, and count how many times the median value repeats consecutively, starting from the median index itself and moving to the right until a different value is encountered. For example, given [1, 2, 3, 3, 3], the sorted array has median index 2 (value 3), and the count of consecutive 3s from index 2 onward is 3. For even sizes like [1, 2, 3, 4], median index is 1 (value 2), and since the next value is 3, the count is 1 (only the median itself). The function must handle arrays with all identical values, single-element arrays, and negative numbers, and must not modify the original input vector (pass by const reference). The return value is always at least 1 because the median itself always counts.

// The solution first creates a copy of the input vector and sorts it in non-decreasing order. Then determine the median index using the rule: for odd n, index = n/2; for even n, index = (n/2 - 1). Initialize a counter to 1 (the median itself). Then iterate from the median index + 1 to the end of the array, breaking as soon as the current element differs from the previous one; otherwise, increment the counter. This correctly counts consecutive duplicates to the right of (and including) the median. Edge cases: if the array has only one element, the median index is 0 and the loop doesn’t run, so the count is 1. If all elements are equal, the loop runs to the end and the count equals n. Negative numbers do not affect the logic. Time complexity is O(n log n) due to sorting; space complexity is O(n) for the copy. The original vector remains unmodified.

#include <vector>
#include <algorithm>

// Return the number of consecutive occurrences of the median value
// starting from the median index (lower median for even sizes) and moving right.
int countMedianRepeats(const std::vector<int>& input) {
    std::vector<int> sorted = input;
    std::sort(sorted.begin(), sorted.end());

    const int n = static_cast<int>(sorted.size());
    const int medIndex = (n % 2 == 0) ? (n / 2 - 1) : (n / 2);

    int count = 1;
    for (int i = medIndex + 1; i < n; ++i) {
        if (sorted[i] == sorted[i - 1]) {
            ++count;
        } else {
            break;
        }
    }
    return count;
}

#include <cassert>
#include <vector>

int countMedianRepeats(const std::vector<int>& input);

int main() {
    // Example from snippet: odd length
    assert(countMedianRepeats({1, 2, 3, 3, 3}) == 3);
    // Even length, lower median repeats once
    assert(countMedianRepeats({1, 2, 3, 4}) == 1);
    // Single element
    assert(countMedianRepeats({7}) == 1);
    // All equal
    assert(countMedianRepeats({5, 5, 5, 5}) == 4);
    // Negative numbers and unsorted input
    assert(countMedianRepeats({-3, -1, -2, -2}) == 2);
    // Even length where median and next are equal
    assert(countMedianRepeats({10, 20, 20, 30}) == 2);
    // Odd length, no repeats after median
    assert(countMedianRepeats({1, 2, 4, 5, 6}) == 1);
    // Duplicates elsewhere but not after median
    assert(countMedianRepeats({1, 1, 2, 3, 4}) == 1);
    // Large values
    assert(countMedianRepeats({1000000000, 1000000000, 1000000000}) == 3);
    // Original vector not modified
    std::vector<int> original = {3, 1, 2};
    countMedianRepeats(original);
    assert((original == std::vector<int>{3, 1, 2}));
    return 0;
}
