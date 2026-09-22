/*
Write a C++ function `rotateArrayLeftByOne` that takes a `std::vector<double>` by reference and modifies it in-place so that each element moves one position to the left, with the first element wrapping around to the end. For example, `[1.0, 2.0, 3.0, 4.0]` becomes `[2.0, 3.0, 4.0, 1.0]`. The function must handle empty vectors (no change) and vectors of any size. It should be const-correct where appropriate, not use any external libraries beyond the C++ standard library, and have a time complexity of O(n) and space complexity of O(1) by performing the rotation using a single temporary variable.
*/
#include <vector>

// Rotate a vector left by one position in-place.
// Example: [a, b, c] -> [b, c, a]
void rotateArrayLeftByOne(std::vector<double>& vec) {
    if (vec.size() <= 1) {
        return;  // Nothing to rotate
    }

    double first = vec[0];  // Save the first element

    // Shift all elements one position to the left
    for (std::size_t i = 0; i < vec.size() - 1; ++i) {
        vec[i] = vec[i + 1];
    }

    // Place the saved first element at the end
    vec.back() = first;
}
#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Test 1: Normal case
    std::vector<double> v1 = {1.0, 2.0, 3.0, 4.0};
    rotateArrayLeftByOne(v1);
    assert(v1 == std::vector<double>({2.0, 3.0, 4.0, 1.0}));

    // Test 2: Single element (should remain unchanged)
    std::vector<double> v2 = {42.0};
    rotateArrayLeftByOne(v2);
    assert(v2 == std::vector<double>({42.0}));

    // Test 3: Empty vector (should remain empty)
    std::vector<double> v3;
    rotateArrayLeftByOne(v3);
    assert(v3.empty());

    // Test 4: Two elements
    std::vector<double> v4 = {3.5, -1.2};
    rotateArrayLeftByOne(v4);
    assert(v4 == std::vector<double>({-1.2, 3.5}));

    // Test 5: Larger vector with decimals
    std::vector<double> v5 = {0.1, 0.2, 0.3, 0.4, 0.5};
    rotateArrayLeftByOne(v5);
    assert(v5 == std::vector<double>({0.2, 0.3, 0.4, 0.5, 0.1}));

    // Test 6: All same values
    std::vector<double> v6 = {7.0, 7.0, 7.0};
    rotateArrayLeftByOne(v6);
    assert(v6 == std::vector<double>({7.0, 7.0, 7.0}));

    // Test 7: Negative and zero values
    std::vector<double> v7 = {-1.0, 0.0, -2.0};
    rotateArrayLeftByOne(v7);
    assert(v7 == std::vector<double>({0.0, -2.0, -1.0}));

    return 0;
}
// The problem is a standard left rotation by one position. The main algorithm is straightforward: save the first element in a temporary variable, then shift all remaining elements left by one position using a loop that assigns `vec[i] = vec[i+1]` for `i` from 0 to `size-2`, and finally set the last element to the saved temporary value. Edge cases: an empty vector or a vector with one element requires no changes (the loop would do nothing, and the final assignment would overwrite the only element with itself). No special case code is strictly needed, but adding an early return for `size <= 1` improves clarity and avoids unnecessary operations. Time complexity is O(n) for the single pass over the `n-1` elements, and space complexity is O(1) because only one temporary `double` is used. The function should modify the vector in-place and return `void` to match the specification.
