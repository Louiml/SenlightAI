/*
Write a C++ function that takes two sorted vectors of integers (each containing no duplicates) and returns a new vector containing the sorted union of both input vectors, also with no duplicate values. The input vectors are guaranteed to be sorted in non-decreasing order. The function must preserve the sorted order in the output and handle cases where one or both vectors are empty. The function should be named `sortedUnion`, take the two vectors by const reference, and return the resulting vector by value.
*/
#include <vector>

// Return the sorted union of two sorted vectors (each with no duplicates).
std::vector<int> sortedUnion(const std::vector<int>& a, const std::vector<int>& b) {
    std::vector<int> result;
    size_t i = 0, j = 0;
    const size_t n1 = a.size();
    const size_t n2 = b.size();

    // Merge until one vector is exhausted.
    while (i < n1 && j < n2) {
        int value;
        if (a[i] <= b[j]) {
            value = a[i];
            ++i;
        } else {
            value = b[j];
            ++j;
        }
        if (result.empty() || result.back() != value) {
            result.push_back(value);
        }
    }

    // Append remaining elements of the first vector.
    while (i < n1) {
        int value = a[i++];
        if (result.empty() || result.back() != value) {
            result.push_back(value);
        }
    }

    // Append remaining elements of the second vector.
    while (j < n2) {
        int value = b[j++];
        if (result.empty() || result.back() != value) {
            result.push_back(value);
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (or in the same translation unit).
std::vector<int> sortedUnion(const std::vector<int>& a, const std::vector<int>& b);

int main() {
    // Basic union with no overlap.
    assert(sortedUnion({1, 3, 5}, {2, 4, 6}) == std::vector<int>({1, 2, 3, 4, 5, 6}));

    // Overlap with duplicates.
    assert(sortedUnion({1, 2, 3}, {2, 3, 4}) == std::vector<int>({1, 2, 3, 4}));

    // One vector empty.
    assert(sortedUnion({}, {1, 2}) == std::vector<int>({1, 2}));
    assert(sortedUnion({1, 2}, {}) == std::vector<int>({1, 2}));

    // Both empty.
    assert(sortedUnion({}, {}).empty());

    // Identical vectors (all duplicates).
    assert(sortedUnion({1, 1, 1}, {1, 1, 1}) == std::vector<int>({1}));

    // Negative numbers and mixed values.
    assert(sortedUnion({-5, 0, 3}, {-4, -1, 2}) == std::vector<int>({-5, -4, -1, 0, 2, 3}));

    // Boundary case: same value at the boundary between vectors.
    assert(sortedUnion({1, 2, 3}, {3, 4, 5}) == std::vector<int>({1, 2, 3, 4, 5}));

    // First vector is prefix of second.
    assert(sortedUnion({1, 2}, {1, 2, 3, 4}) == std::vector<int>({1, 2, 3, 4}));

    return 0;
}
// The problem is a classic two-pointer merge of two sorted arrays, with an additional check to avoid inserting duplicate consecutive values into the result. Since each input vector is individually sorted and has no internal duplicates, the only possible duplicates in the union come from the same value appearing in both input vectors. We set two indices `i` and `j` starting at 0 for the first and second vectors. At each step, we compare the current elements: if the element in the first vector is smaller or equal, we consider it first; otherwise we consider the second. Before pushing the selected value into the result, we check whether the result is empty or its last element is different from the current value; this handles the case where both vectors contain the same value (the selected one appears twice consecutively). After one vector is exhausted, we copy the remaining elements of the other vector, again using the same duplicate-avoidance check (though since each input has no duplicates, this check is only needed for the boundary case where the last pushed value equals the first remaining element—possible only if the same value appears at the end of one vector and the beginning of the other). Edge cases include empty inputs, identical arrays, one array being a prefix of the other, and negative integers. Time complexity is O(n1 + n2) because each element is processed once. Space complexity is O(n1 + n2) for the result vector (excluding input storage), and we only use O(1) auxiliary space beyond that.
