Given five integers entered from standard input in an arbitrary order, write a C++ function named `sortFiveDescending` that takes a `const std::vector<int>&` containing exactly 5 integers, sorts them in descending order (largest to smallest), and returns a `std::string` representing the sorted numbers separated by single spaces, with no trailing space. The input vector must not be modified, and the function should handle duplicate values, negative numbers, and zeros correctly. The returned string should be suitable for direct output.

The solution copies the input vector into a local mutable vector, sorts it in ascending order using `std::sort`, then constructs the output string by iterating from the last element (largest) down to the first (smallest). For each element, it appends the integer converted to a string, separated by a space except after the final element. Using `std::ostringstream` simplifies formatting: write the first number, then for each subsequent number write a leading space before it. Edge cases include all equal values (e.g., `{3,3,3,3,3}` → `"3 3 3 3 3"`) and negative numbers (e.g., `{-1,-2,-3,-4,-5}` → `"-1 -2 -3 -4 -5"`). Time complexity is O(5 log 5) = O(1) because the size is constant; space complexity is O(1) for the temporary vector and string buffer, ignoring output size.

#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

// Returns a string of the given 5 integers sorted in descending order,
// separated by single spaces. The input vector is not modified.
std::string sortFiveDescending(const std::vector<int>& numbers) {
    // Work on a copy to preserve the caller's data.
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());

    std::ostringstream out;
    // Iterate from largest (last after ascending sort) to smallest.
    out << sorted[4];
    for (int i = 3; i >= 0; --i) {
        out << " " << sorted[i];
    }
    return out.str();
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic order.
    assert(sortFiveDescending({1, 2, 3, 4, 5}) == "5 4 3 2 1");
    // Reverse order.
    assert(sortFiveDescending({5, 4, 3, 2, 1}) == "5 4 3 2 1");
    // Negative numbers.
    assert(sortFiveDescending({-1, -5, -3, -2, -4}) == "-1 -2 -3 -4 -5");
    // Duplicates and zeros.
    assert(sortFiveDescending({0, 0, 2, 2, 2}) == "2 2 2 0 0");
    // All equal.
    assert(sortFiveDescending({7, 7, 7, 7, 7}) == "7 7 7 7 7");
    // Mixed positive and negative.
    assert(sortFiveDescending({-10, 20, 0, -5, 15}) == "20 15 0 -5 -10");
    return 0;
}
