// Write a C++ function that takes a vector of integers as input and returns the 1-based index of the element that appears only once in the entire array, given that every other element appears exactly twice. The function should handle arrays of odd length (at least 1) and return the index of the single non-repeated element. If the input violates the guarantee (e.g., has even length or multiple non-repeated elements), you may return -1 as a safety check. The function must be efficient and avoid modifying the input.

The core idea is to use the XOR bitwise operation, which has the property that `x ^ x = 0` and `x ^ 0 = x`. By XORing all elements of the array, all pairs cancel out, leaving the unique element. After finding the unique element's value, we scan the array once to find its first occurrence and return its 1-based index. Edge cases: an empty vector should return -1; a vector with even length typically won't have a unique element under the guarantee, but we can still compute XOR (it would be 0 if all pairs exist, but there could be multiple unique numbers; we just return the first occurrence if it exists, otherwise -1 if not found). However, to adhere to the problem's intent, we assume valid input. Time complexity is O(n) for two passes (XOR and search), space is O(1). The solution uses `const` reference to avoid copying.

#include <vector>
#include <cstddef>

// Returns the 1-based index of the single non-repeated element.
// If input is empty or the unique element is not found, returns -1.
int findUniqueIndex(const std::vector<int>& arr) {
    if (arr.empty()) return -1;

    int unique = 0;
    for (int value : arr) {
        unique ^= value;
    }

    // If unique is 0 and array is non-empty, it's ambiguous (either all pairs or zero appears).
    // But per problem guarantee, unique is non-zero if length odd and all others pair.
    // Still, we search for 'unique' value; if not found, return -1.
    for (std::size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == unique) {
            return static_cast<int>(i) + 1; // 1-based index
        }
    }
    return -1;
}

#include <cassert>
#include <vector>

int main() {
    // Basic case: unique element in the middle
    std::vector<int> v1 = {1, 2, 3, 2, 1};
    assert(findUniqueIndex(v1) == 3);

    // Unique element at the beginning
    std::vector<int> v2 = {7, 5, 5, 9, 9};
    assert(findUniqueIndex(v2) == 1);

    // Unique element at the end
    std::vector<int> v3 = {4, 4, 8, 8, 9};
    assert(findUniqueIndex(v3) == 5);

    // Single element
    std::vector<int> v4 = {42};
    assert(findUniqueIndex(v4) == 1);

    // Negative numbers
    std::vector<int> v5 = {-3, 2, -3, 2, 0};
    assert(findUniqueIndex(v5) == 5);

    // Large values and duplicates
    std::vector<int> v6 = {100, 200, 100, 300, 200};
    assert(findUniqueIndex(v6) == 4);

    // Empty input
    std::vector<int> v7;
    assert(findUniqueIndex(v7) == -1);

    // Even-length input with no unique (should return -1 because unique XOR may be 0, not found)
    std::vector<int> v8 = {1, 1, 2, 2};
    assert(findUniqueIndex(v8) == -1); // unique = 0, not in array

    // Unique value is zero
    std::vector<int> v9 = {5, 0, 5, 2, 2};
    assert(findUniqueIndex(v9) == 2);
}
