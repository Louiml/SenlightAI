/*
Write a C++ function named `largestElement` that takes a non-empty vector of integers by const reference and returns the largest value in the vector. The function must work correctly for vectors containing negative numbers, zeros, positive numbers, and duplicate values. The input vector must not be modified, and the function should be efficient for vectors of any size. Additionally, write a separate test program that verifies the function using several `assert` statements covering typical cases, edge cases (single element, all negatives, all equal), and larger inputs.
*/
#include <vector>
#include <algorithm> // for std::max

// Returns the largest element in a non-empty vector of integers.
// The input vector is not modified.
int largestElement(const std::vector<int>& numbers) {
    // Assume the vector is non-empty per specification.
    int largest = numbers.front();
    
    for (size_t i = 1; i < numbers.size(); ++i) {
        if (numbers[i] > largest) {
            largest = numbers[i];
        }
    }
    
    return largest;
}
#include <cassert>
#include <vector>

// Declaration of the function (assuming solution function is defined elsewhere)
int largestElement(const std::vector<int>& numbers);

int main() {
    // Typical case with positive numbers
    std::vector<int> v1 = {3, 1, 7, 2, 9};
    assert(largestElement(v1) == 9);
    
    // All negative numbers
    std::vector<int> v2 = {-10, -3, -20, -1};
    assert(largestElement(v2) == -1);
    
    // Single element
    std::vector<int> v3 = {5};
    assert(largestElement(v3) == 5);
    
    // All equal values
    std::vector<int> v4 = {4, 4, 4, 4};
    assert(largestElement(v4) == 4);
    
    // Mixed with zeros and negatives
    std::vector<int> v5 = {0, -5, 2, 0, -1};
    assert(largestElement(v5) == 2);
    
    // Large vector with duplicate maximum
    std::vector<int> v6 = {100, 50, 100, -200, 0};
    assert(largestElement(v6) == 100);
    
    // Already descending order
    std::vector<int> v7 = {9, 8, 7, 6};
    assert(largestElement(v7) == 9);
    
    // Already ascending order
    std::vector<int> v8 = {2, 5, 10, 15};
    assert(largestElement(v8) == 15);
    
    return 0;
}
// The solution is straightforward: iterate through the vector once while maintaining a running maximum. Start by initializing the maximum to the first element (since the vector is guaranteed non-empty), then update it whenever a later element is larger. This correctly handles all cases: negative-only vectors (the least negative becomes the maximum), positive vectors, duplicates (comparison uses `>=` or `>` equivalently since duplicates don't affect the result), and single-element vectors. The algorithm runs in O(n) time, where n is the number of elements, and uses O(1) auxiliary space. No edge cases require special handling except ensuring the vector is non-empty (which is guaranteed by the task spec); an empty vector would cause undefined behavior if accessed, so the contract must be respected. The function should be `const`-qualified in the sense that it accepts the vector by `const&`, and the function itself does not modify anything.
