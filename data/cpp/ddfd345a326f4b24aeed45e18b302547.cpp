// Write a C++ function named `largestElement` that takes a non-empty vector of integers and returns the largest element. The function must not modify the input vector, must work correctly with negative numbers, duplicates, and a single-element vector, and must not rely on sorting. Use a linear scan to find the maximum, and define the function with appropriate `const` correctness (accept the vector by `const std::vector<int>&`). The return type is `int`.
// The solution iterates through the vector once, maintaining a running maximum variable initialized to the first element. For each subsequent element, if it is greater than the current maximum, update the maximum. This works for all edge cases: negative numbers are handled because we start with the first element (which could be negative), duplicates are naturally ignored, and a single-element vector returns that element immediately. No sorting is needed, which avoids mutating the input and is more efficient. Time complexity is O(n) for n elements, and auxiliary space is O(1) because we only use a few integer variables.
#include <vector>
#include <algorithm> // for std::max

// Returns the largest element in a non-empty vector of integers.
// Does not modify the input vector; O(n) time, O(1) space.
int largestElement(const std::vector<int>& numbers) {
    int maximum = numbers[0];
    for (size_t i = 1; i < numbers.size(); ++i) {
        maximum = std::max(maximum, numbers[i]);
    }
    return maximum;
}
#include <cassert>
#include <vector>

int main() {
    // Declare the function (normally from a header, but here for standalone test)
    int largestElement(const std::vector<int>&);

    std::vector<int> v1 = {1, 2, 3, 4, 5, 6};
    assert(largestElement(v1) == 6);

    std::vector<int> v2 = {-10, -2, -5, -1};
    assert(largestElement(v2) == -1);

    std::vector<int> v3 = {7};
    assert(largestElement(v3) == 7);

    std::vector<int> v4 = {3, 3, 3, 3};
    assert(largestElement(v4) == 3);

    std::vector<int> v5 = {100, -100, 0, 50, 50};
    assert(largestElement(v5) == 100);

    std::vector<int> v6 = {0, 0, 0};
    assert(largestElement(v6) == 0);

    std::vector<int> v7 = {5, 4, 3, 2, 1};
    assert(largestElement(v7) == 5);

    // Ensure the input vector is not modified
    std::vector<int> original = {1, 9, 3, 7};
    std::vector<int> copy = original;
    largestElement(original);
    assert(original == copy);
}
