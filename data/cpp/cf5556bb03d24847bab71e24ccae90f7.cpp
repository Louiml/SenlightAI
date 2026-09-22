Write a C++ function that takes a `std::vector<int>` and modifies it in-place so that for each position `i`, the element becomes the largest value among all elements to its right (i.e., in positions `i+1` through `n-1`). The last element must always become `-1`. The function should handle empty vectors and single-element vectors correctly, avoiding any undefined behavior. Return the modified vector (by reference or value—specify clearly in your solution). Your implementation must use an efficient approach—specifically, a single right-to-left pass—and must not allocate auxiliary storage proportional to the input size.
The optimal solution exploits the fact that when scanning from right to left, the maximum value to the right of any index can be maintained incrementally. Start by storing the original last element in a temporary variable, then set the last position to `-1`. For each index from `n-2` down to `0`, the current maximum to the right is the maximum of the temporary variable (which holds the maximum of all elements to the right) and the element at `i+1` (which is already the replaced value, but careful: we need the original values). The correct approach: maintain a running maximum of the *original* elements to the right. Initially, before modifying, the maximum to the right of index `n-2` is `arr[n-1]`. Then for each index `i` from `n-2` down to `0`: save the original `arr[i]` in a temp, set `arr[i]` to the current running maximum, then update the running maximum to `max(running_max, temp)`. This ensures we always use original values, not the replaced ones. Edge cases: empty vector returns empty; single-element vector becomes `{-1}`. Time complexity is \(O(n)\), space complexity is \(O(1)\) auxiliary (excluding input and output storage).
#include <vector>
#include <algorithm>

// Replace each element with the greatest element to its right.
// The last element becomes -1. Works in-place in O(n) time and O(1) space.
std::vector<int>& replaceElements(std::vector<int>& arr) {
    if (arr.empty()) return arr;
    
    int n = arr.size();
    // The maximum among elements to the right of the current index.
    int max_right = arr[n - 1];
    // The last element always becomes -1.
    arr[n - 1] = -1;
    
    // Process from the second last element down to the first.
    for (int i = n - 2; i >= 0; --i) {
        int original = arr[i];          // Save the original value before overwriting.
        arr[i] = max_right;             // Current element becomes the max to its right.
        max_right = std::max(max_right, original); // Update max for the next left position.
    }
    
    return arr;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Test 1: Standard case
    std::vector<int> a1 = {17, 18, 5, 4, 6, 1};
    replaceElements(a1);
    std::vector<int> e1 = {18, 6, 6, 6, 1, -1};
    assert(a1 == e1);

    // Test 2: Single element
    std::vector<int> a2 = {400};
    replaceElements(a2);
    std::vector<int> e2 = {-1};
    assert(a2 == e2);

    // Test 3: Empty vector
    std::vector<int> a3;
    replaceElements(a3);
    assert(a3.empty());

    // Test 4: Strictly decreasing
    std::vector<int> a4 = {5, 4, 3, 2, 1};
    replaceElements(a4);
    std::vector<int> e4 = {4, 3, 2, 1, -1};
    assert(a4 == e4);

    // Test 5: Strictly increasing
    std::vector<int> a5 = {1, 2, 3, 4, 5};
    replaceElements(a5);
    std::vector<int> e5 = {5, 5, 5, 5, -1};
    assert(a5 == e5);

    // Test 6: All equal
    std::vector<int> a6 = {7, 7, 7, 7};
    replaceElements(a6);
    std::vector<int> e6 = {7, 7, 7, -1};
    assert(a6 == e6);

    // Test 7: Two elements
    std::vector<int> a7 = {10, 3};
    replaceElements(a7);
    std::vector<int> e7 = {3, -1};
    assert(a7 == e7);

    // Test 8: Two elements with equal values
    std::vector<int> a8 = {6, 6};
    replaceElements(a8);
    std::vector<int> e8 = {6, -1};
    assert(a8 == e8);

    // Test 9: Large vector with repeated maximums
    std::vector<int> a9 = {2, 10, 10, 5, 3, 8};
    replaceElements(a9);
    std::vector<int> e9 = {10, 10, 8, 8, 8, -1};
    assert(a9 == e9);

    // Test 10: Negative numbers
    std::vector<int> a10 = {-1, -5, -3, -2};
    replaceElements(a10);
    std::vector<int> e10 = {-2, -2, -2, -1};
    assert(a10 == e10);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
