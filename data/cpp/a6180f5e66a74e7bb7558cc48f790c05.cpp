// Write a C++ function named `findMaximum` that takes a vector of integers and returns the maximum value found in the series. The function must handle both positive and negative numbers, including cases where all values are negative, and should work correctly for an empty vector by returning a sentinel value of `INT_MIN` (the smallest possible `int`). The function must not modify the input vector and should be implemented with proper `const` correctness.

// The solution is straightforward: iterate through the vector once, maintaining a running maximum. Initialize the maximum to `INT_MIN` from `<climits>` so that any real element in the vector will be larger (or equal) and thus correctly replace it. For each element, compare it with the current maximum using `std::max` or an if-statement and update accordingly. Edge cases include an empty vector (return `INT_MIN`), a vector with a single element (that element is the maximum), and vectors with duplicate maximum values (the first occurrence is sufficient, but duplicates cause no issue). Time complexity is O(n) where n is the number of elements, and space complexity is O(1) auxiliary, aside from the input vector itself.

#include <vector>
#include <climits>
#include <algorithm>

// Return the maximum value in the given vector, or INT_MIN if the vector is empty.
int findMaximum(const std::vector<int>& numbers) {
    int max_val = INT_MIN;
    for (int value : numbers) {
        max_val = std::max(max_val, value);
    }
    return max_val;
}

#include <cassert>
#include <vector>
#include <climits>

int findMaximum(const std::vector<int>& numbers); // declaration from solution

int main() {
    // Standard positive and mixed numbers
    std::vector<int> v1 = {3, 1, 4, 1, 5, 9, 2, 6};
    assert(findMaximum(v1) == 9);

    // All negative numbers
    std::vector<int> v2 = {-5, -2, -9, -1};
    assert(findMaximum(v2) == -1);

    // Single element
    std::vector<int> v3 = {42};
    assert(findMaximum(v3) == 42);

    // Duplicate maximum
    std::vector<int> v4 = {7, 3, 7, 1, 7};
    assert(findMaximum(v4) == 7);

    // Empty vector should return INT_MIN
    std::vector<int> v5;
    assert(findMaximum(v5) == INT_MIN);

    // Large negative mixed with zero
    std::vector<int> v6 = {-100, 0, -50};
    assert(findMaximum(v6) == 0);

    // Already sorted descending
    std::vector<int> v7 = {9, 8, 7, 6};
    assert(findMaximum(v7) == 9);

    // Already sorted ascending
    std::vector<int> v8 = {1, 5, 10, 20};
    assert(findMaximum(v8) == 20);

    return 0;
}
