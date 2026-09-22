Write a C++ function named `reverseArrayInPlace` that takes a vector of integers by reference and reverses its order in place, without using any additional container or temporary array beyond a single swap buffer. The function must correctly handle vectors of even and odd lengths, an empty vector, and a vector with a single element. The original vector must be modified directly, and no return value is needed. The function should be self-contained, include the necessary `<vector>` header, and use `const` correctness appropriately in any helper logic (though the parameter itself is non-const because it is modified).
// The solution uses the classic two-pointer swap technique: initialize `left` to index 0 and `right` to the last index (`size - 1`). While `left < right`, swap the elements at these indices using `std::swap` or a manual temporary variable, then increment `left` and decrement `right`. This handles even-length vectors perfectly (swaps pairs until pointers cross) and odd-length vectors (the middle element stays in place because after the last swap, `left` and `right` become equal, and the loop condition fails). An empty vector and a single-element vector are trivially handled because the loop never executes. Time complexity is O(n), where n is the number of elements, with each element moved exactly once. Space complexity is O(1) extra space, as only a few integer indices and one temporary variable are used.
#include <vector>

// Reverse the order of elements in the given vector in place.
void reverseArrayInPlace(std::vector<int>& values) {
    size_t left = 0;
    size_t right = values.size();
    
    // If empty or single-element, right = size 0 or 1, so left == right or left > right.
    while (left < right) {
        --right; // Move right to the last valid index
        if (left < right) {
            int temp = values[left];
            values[left] = values[right];
            values[right] = temp;
            ++left;
        }
    }
}
#include <cassert>
#include <vector>

// The solution function is declared above (include the header or paste here).
// For testing, we assume the function is available.

int main() {
    // Test even length
    std::vector<int> v1 = {1, 2, 3, 4};
    reverseArrayInPlace(v1);
    assert((v1 == std::vector<int>{4, 3, 2, 1}));

    // Test odd length
    std::vector<int> v2 = {1, 2, 3, 4, 5};
    reverseArrayInPlace(v2);
    assert((v2 == std::vector<int>{5, 4, 3, 2, 1}));

    // Test single element
    std::vector<int> v3 = {42};
    reverseArrayInPlace(v3);
    assert((v3 == std::vector<int>{42}));

    // Test empty vector
    std::vector<int> v4;
    reverseArrayInPlace(v4);
    assert(v4.empty());

    // Test with negative numbers and duplicates
    std::vector<int> v5 = {-1, 0, -5, 3, -1};
    reverseArrayInPlace(v5);
    assert((v5 == std::vector<int>{-1, 3, -5, 0, -1}));

    // Test already reversed (reverse twice gives original)
    std::vector<int> v6 = {7, 8, 9};
    reverseArrayInPlace(v6);
    reverseArrayInPlace(v6);
    assert((v6 == std::vector<int>{7, 8, 9}));

    // Test large size (e.g., 1000 elements) - check first and last after reverse
    std::vector<int> v7(1000);
    for (int i = 0; i < 1000; ++i) v7[i] = i;
    reverseArrayInPlace(v7);
    assert(v7[0] == 999 && v7[999] == 0);
    // Check a few middle elements
    assert(v7[500] == 499 && v7[499] == 500);

    return 0;
}
