// Write a C++ function `vector<int> processDoubleEndedQueue(const vector<string>& operations)` that processes a sequential list of operations on a multiset (implemented using `std::multiset<int>`), where each operation string has the form `"I n"` (insert integer `n`) or `"D 1"` (delete the maximum element if the multiset is non-empty) or `"D -1"` (delete the minimum element if the multiset is non-empty). After processing all operations, the function should return a vector containing exactly two integers: the maximum remaining element and the minimum remaining element, in that order. If the multiset is empty at the end, return `{0, 0}`. The input may contain duplicate integers, and operations are guaranteed to be well-formed (each string starts with either `'I'` or `'D'` followed by an integer). Assume the input vector is non-empty.
// The solution uses a `std::multiset<int>` (with default `std::less<int>` ordering) to maintain the elements in ascending order. For each operation string, we parse the first character and the integer using a `std::stringstream`. If the character is `'I'`, we insert the integer into the multiset. If the character is `'D'`, we check the integer value: if it is `1` and the multiset is not empty, we delete the last element (maximum) by calling `erase(prev(ms.end()))`; if it is `-1` and the multiset is not empty, we delete the first element (minimum) by calling `erase(ms.begin())`. After processing all operations, if the multiset is empty, we return `{0, 0}`; otherwise, we return `{*ms.rbegin(), *ms.begin()}` (i.e., maximum first, minimum second). Important edge cases: never attempt to erase from an empty multiset (the conditions guard against that); duplicates are handled automatically by `multiset` (erasing one instance via an iterator removes only that occurrence); and the final maximum/minimum are obtained via `rbegin()` and `begin()`. Time complexity: each operation is O(log m) for insert/erase, where m is the current size, so total O(n log n) where n is the number of operations. Space complexity: O(n) for storing up to n elements in the multiset.
#include <string>
#include <vector>
#include <set>
#include <sstream>

// Processes insert/delete-max/delete-min operations on a multiset.
// Returns {max, min} of remaining elements, or {0, 0} if empty.
std::vector<int> processDoubleEndedQueue(const std::vector<std::string>& operations) {
    std::multiset<int> ms;  // ascending order

    for (const std::string& op : operations) {
        std::stringstream ss(op);
        char command;
        int value;
        ss >> command >> value;

        if (command == 'I') {
            ms.insert(value);
        } else {  // command == 'D'
            if (value == 1 && !ms.empty()) {
                ms.erase(std::prev(ms.end()));  // delete maximum
            } else if (value == -1 && !ms.empty()) {
                ms.erase(ms.begin());           // delete minimum
            }
        }
    }

    if (ms.empty()) {
        return {0, 0};
    }
    return {*ms.rbegin(), *ms.begin()};  // max, min
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic insert and delete max/min
    assert(processDoubleEndedQueue({"I 5", "I 3", "I 8"}) == std::vector<int>({8, 3}));
    assert(processDoubleEndedQueue({"I 5", "I 3", "D 1", "I 4"}) == std::vector<int>({4, 3}));
    assert(processDoubleEndedQueue({"I 5", "I 3", "D -1", "I 4"}) == std::vector<int>({5, 4}));

    // Duplicates: delete max removes one occurrence
    assert(processDoubleEndedQueue({"I 7", "I 7", "D 1"}) == std::vector<int>({7, 7}));
    assert(processDoubleEndedQueue({"I 7", "I 7", "D -1"}) == std::vector<int>({7, 7}));

    // Empty result
    assert(processDoubleEndedQueue({"I 1", "D 1", "D -1"}) == std::vector<int>({0, 0}));
    assert(processDoubleEndedQueue({"D 1", "D -1", "I 2", "D 1", "D -1"}) == std::vector<int>({0, 0}));

    // Deletes on empty do nothing
    assert(processDoubleEndedQueue({"D 1", "D -1", "I 9"}) == std::vector<int>({9, 9}));

    // Mixed operations
    assert(processDoubleEndedQueue({"I -5", "I 10", "D 1", "I -3", "D -1", "I 0"}) == std::vector<int>({0, -3}));

    // Only one element remains
    assert(processDoubleEndedQueue({"I 42", "D 1", "I 100", "D 1"}) == std::vector<int>({42, 42}));

    // Negative and large values
    assert(processDoubleEndedQueue({"I -1000", "I 1000", "I 0"}) == std::vector<int>({1000, -1000}));
}
