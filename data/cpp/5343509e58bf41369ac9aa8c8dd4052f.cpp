// Write a C++ function `reverseArray` that takes a vector of integers and returns a new vector containing the same integers in reversed order. The function must preserve the original vector (i.e., it should not modify the input). Handle vectors of any length, including empty vectors (return an empty vector for an empty input). Do not use any standard library reverse algorithm; implement the reversal manually by iterating from the last element to the first. The function signature must be `std::vector<int> reverseArray(const std::vector<int>& arr)`.
// The solution creates a new empty vector `result` and reserves space equal to the input size to avoid reallocations. Then it iterates from the last index (`arr.size()-1`) down to 0, pushing each element into `result`. This correctly reverses the order. Important edge cases: an empty input vector results in an empty output (the loop simply doesn’t execute). A single-element vector is trivially reversed (the loop pushes that one element). The algorithm runs in O(n) time because each element is visited once, and uses O(n) auxiliary space for the output vector, not counting the input. The input vector is passed by const reference, ensuring it is not modified. The function is `const`‑correct: the parameter is `const std::vector<int>&` and the function does not mutate any external state.
#include <vector>

// Returns a new vector containing the elements of arr in reversed order.
// The input vector arr is not modified.
std::vector<int> reverseArray(const std::vector<int>& arr) {
    std::vector<int> result;
    result.reserve(arr.size());  // Avoid reallocations

    // Iterate from last element to first, appending each element.
    for (size_t i = arr.size(); i > 0; --i) {
        result.push_back(arr[i - 1]);
    }
    return result;
}
#include <cassert>
#include <vector>

// Function prototype (normally declared in a header)
std::vector<int> reverseArray(const std::vector<int>& arr);

int main() {
    // Test with a typical vector
    std::vector<int> v1 = {1, 2, 3, 4, 5};
    assert(reverseArray(v1) == std::vector<int>({5, 4, 3, 2, 1}));

    // Test with duplicate values
    std::vector<int> v2 = {7, 7, 7};
    assert(reverseArray(v2) == std::vector<int>({7, 7, 7}));

    // Test with a single element
    std::vector<int> v3 = {42};
    assert(reverseArray(v3) == std::vector<int>({42}));

    // Test with negative numbers
    std::vector<int> v4 = {-3, 0, 1, -8};
    assert(reverseArray(v4) == std::vector<int>({-8, 1, 0, -3}));

    // Test with an empty vector
    std::vector<int> v5;
    assert(reverseArray(v5) == std::vector<int>());

    // Test that input is not modified
    std::vector<int> original = {10, 20, 30};
    reverseArray(original);
    assert(original == std::vector<int>({10, 20, 30}));

    return 0;
}
