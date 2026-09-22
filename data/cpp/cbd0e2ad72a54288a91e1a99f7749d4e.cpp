// Write a C++ function that takes an array of integers and its size, and returns a new array (or vector) containing the elements that are greater than their immediate left neighbor (if one exists). The original array's order must be preserved, and index 0 is never included in the result because it has no left neighbor. The function should handle arrays of any non-negative size, and if no such elements exist, return an empty result.
// The solution iterates over the array starting from index 1 (since index 0 has no left neighbor). For each index i, compare arr[i] with arr[i-1]. If arr[i] > arr[i-1], append arr[i] to the result vector. This is a straightforward linear scan: for an array of size n, we perform n-1 comparisons, so time complexity is O(n). Space complexity is O(k) where k is the number of qualifying elements, but in the worst case (e.g., strictly increasing array) k = n-1, so O(n) auxiliary space. Edge cases include: empty array (size 0) returns empty; array of size 1 returns empty (since no left neighbor exists); equal or decreasing neighbors produce no output for that index. The solution uses const-correctness by taking the input as `const std::vector<int>&` or a pointer+size, and returns a `std::vector<int>` to avoid manual memory management. If using raw arrays, the function signature could be `std::vector<int> greaterThanLeft(const int arr[], size_t size)`.
#include <vector>

// Return elements that are strictly greater than their immediate left neighbor.
std::vector<int> greaterThanLeft(const std::vector<int>& arr) {
    std::vector<int> result;
    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] > arr[i - 1]) {
            result.push_back(arr[i]);
        }
    }
    return result;
}
#include <cassert>
#include <vector>

// Function under test (provided above)
std::vector<int> greaterThanLeft(const std::vector<int>& arr);

int main() {
    // Basic case with mixed values
    assert((greaterThanLeft({10, 5, 12, 18, 20}) == std::vector<int>{12, 18, 20}));
    // Strictly decreasing array -> empty
    assert((greaterThanLeft({5, 4, 3, 2, 1}) == std::vector<int>{}));
    // Strictly increasing array -> all except first
    assert((greaterThanLeft({1, 2, 3, 4}) == std::vector<int>{2, 3, 4}));
    // Array with duplicates and equals
    assert((greaterThanLeft({3, 3, 4, 4, 5}) == std::vector<int>{4, 5}));
    // Single element -> empty
    assert((greaterThanLeft({7}) == std::vector<int>{}));
    // Empty array -> empty
    assert((greaterThanLeft({}) == std::vector<int>{}));
    // Negative numbers
    assert((greaterThanLeft({-3, -1, -2, 0}) == std::vector<int>{-1, 0}));
    return 0;
}
