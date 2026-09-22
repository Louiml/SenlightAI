Write a C++ function `findFrequency` that takes a non-empty vector of integers and an integer `x`, and returns the number of times `x` appears in the vector. The vector may contain negative numbers, duplicate values, and any number of elements. The function should not modify the input vector and should handle the case where `x` is not present by returning `0`. Provide a self-contained implementation with appropriate headers and `const` correctness.

// The solution iterates through the entire vector once, comparing each element to the target value `x`. A counter is incremented whenever a match is found. Since the vector is passed by value in the original snippet, but to avoid unnecessary copying and respect const correctness, the function should accept the vector by `const` reference. The main edge cases include an empty vector (should return 0, though the task states non-empty, it is safe to handle), `x` not present (returns 0), and all elements equal to `x` (returns the vector size). The time complexity is O(n), where n is the vector size, and space complexity is O(1) as only a single integer counter is used.

#include <vector>

// Count the number of occurrences of value x in the input vector.
// The vector is accepted by const reference to avoid copying and to guarantee no modification.
int findFrequency(const std::vector<int>& arr, int x) {
    int count = 0;
    for (int value : arr) {
        if (value == x) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <vector>

// Function declaration (include the implementation above or link with it)
int findFrequency(const std::vector<int>& arr, int x);

int main() {
    // Basic case with multiple occurrences
    assert(findFrequency({1, 2, 3, 2, 2}, 2) == 3);
    // Element not present
    assert(findFrequency({1, 2, 3}, 4) == 0);
    // Single element matches
    assert(findFrequency({7}, 7) == 1);
    // All elements match
    assert(findFrequency({5, 5, 5, 5}, 5) == 4);
    // Negative numbers and mixed values
    assert(findFrequency({-1, -1, 0, 1, -1}, -1) == 3);
    // Vector with no duplicates
    assert(findFrequency({10, 20, 30}, 20) == 1);
    // Empty vector (though task says non-empty, safe to test)
    assert(findFrequency({}, 1) == 0);
    // Large frequency check
    assert(findFrequency({9, 9, 9}, 9) == 3);
    return 0;
}
