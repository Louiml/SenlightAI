Write a C++ function `int firstRepeatedPosition(const std::vector<int>& arr)` that, given a vector of integers (which may contain negative values, zero, and duplicates), returns the **1-based position** of the first element that appears more than once in the array. If no element repeats, return `-1`. For example, for `arr = {1, 2, 3, 2, 1}`, the first repeating element is `1` at position `1` (and it repeats later), but since position `1` is the first occurrence of any repeated element, the answer is `1`. For `arr = {4, 5, 6}`, the answer is `-1`. The function must be efficient for large arrays and must not modify the input.
// The core idea is to identify the first index (1-based) whose value has at least one duplicate elsewhere in the array. A straightforward approach is to count frequencies of all elements using an `unordered_map<int, int>` in a first pass, storing how many times each value appears. Then, in a second pass through the array, iterate from the beginning and check the frequency of the current element; if its frequency is greater than 1, that index is the first occurrence of a repeated element, so return `i + 1` (since indexing is 0-based). If no element has frequency > 1, return `-1`. Edge cases include empty arrays (return `-1`), arrays with a single element (return `-1`), all elements unique (return `-1`), and arrays where the first repeated element appears multiple times but the earliest occurrence is at a later index (e.g., `{2, 1, 3, 1}` – the first repeated element is `1` at position 2, because `2` and `3` are unique, and `1` repeats). The algorithm runs in `O(n)` time on average, since both hash map insertions and lookups are average `O(1)`, and requires `O(n)` auxiliary space for the hash map. The solution does not require sorting or nested loops, avoiding the `O(n^2)` worst-case time of a brute-force approach.
#include <vector>
#include <unordered_map>

// Returns the 1-based index of the first element that appears more than once.
// Returns -1 if no element repeats.
int firstRepeatedPosition(const std::vector<int>& arr) {
    std::unordered_map<int, int> frequency;

    // Count occurrences of each value.
    for (int value : arr) {
        ++frequency[value];
    }

    // Find the first index whose value has a frequency > 1.
    for (std::size_t i = 0; i < arr.size(); ++i) {
        if (frequency[arr[i]] > 1) {
            return static_cast<int>(i) + 1; // 1-based position
        }
    }

    return -1;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(firstRepeatedPosition({1, 2, 3, 2, 1}) == 1);
    assert(firstRepeatedPosition({4, 5, 6}) == -1);
    assert(firstRepeatedPosition({}) == -1);
    assert(firstRepeatedPosition({7}) == -1);

    // Repeated element not at the start
    assert(firstRepeatedPosition({2, 1, 3, 1}) == 2);
    assert(firstRepeatedPosition({10, 20, 30, 20, 40}) == 2);

    // Negative and zero values
    assert(firstRepeatedPosition({-1, 0, -1, 5}) == 1);
    assert(firstRepeatedPosition({0, 0, 0}) == 1);
    assert(firstRepeatedPosition({-5, -5, 1, 2}) == 1);

    // More complex with multiple repeats
    assert(firstRepeatedPosition({1, 2, 3, 4, 2, 3}) == 2); // first repeat is 2 at pos 2
    assert(firstRepeatedPosition({9, 8, 7, 8, 9}) == 1);     // 9 repeats, first at pos 1

    // All unique
    assert(firstRepeatedPosition({1, 2, 3, 4, 5}) == -1);
    assert(firstRepeatedPosition({-1, -2, 0, 3}) == -1);
}
