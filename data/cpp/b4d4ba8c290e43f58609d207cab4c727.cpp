Write a C++ function that takes an array of integers and its size, and returns a vector of all integers that appear more than once in the array, sorted in ascending order. If no duplicate values exist, the function must return a vector containing exactly one element: `-1`. The input array may contain negative numbers, zero, and positive numbers, and the size `n` is guaranteed to be at least 1. The function should not modify the input array.
#include <cassert>
#include <vector>

// solution function declaration (assume included from above)
std::vector<int> findDuplicates(const int arr[], int n);

int main() {
    // Test 1: Simple duplicates
    int arr1[] = {2, 3, 1, 2, 3};
    assert(findDuplicates(arr1, 5) == std::vector<int>({2, 3}));

    // Test 2: No duplicates -> -1
    int arr2[] = {1, 2, 3, 4, 5};
    assert(findDuplicates(arr2, 5) == std::vector<int>({-1}));

    // Test 3: All same numbers
    int arr3[] = {7, 7, 7};
    assert(findDuplicates(arr3, 3) == std::vector<int>({7}));

    // Test 4: Negative numbers and zero
    int arr4[] = {-1, 0, -1, 2, 0, -1};
    assert(findDuplicates(arr4, 6) == std::vector<int>({-1, 0}));

    // Test 5: Single element no duplicate
    int arr5[] = {42};
    assert(findDuplicates(arr5, 1) == std::vector<int>({-1}));

    // Test 6: Duplicates unsorted input
    int arr6[] = {3, 2, 1, 3, 2, 1};
    assert(findDuplicates(arr6, 6) == std::vector<int>({1, 2, 3}));

    // Test 7: Mixed with large values
    int arr7[] = {1000000, -1000000, 1000000, -1000000, 0};
    assert(findDuplicates(arr7, 5) == std::vector<int>({-1000000, 1000000}));

    return 0;
}
#include <vector>
#include <unordered_map>
#include <algorithm>

// Return all integers occurring more than once in arr, sorted ascending.
// If no duplicates exist, return {-1}.
std::vector<int> findDuplicates(const int arr[], int n) {
    std::unordered_map<int, int> frequency;
    for (int i = 0; i < n; ++i) {
        ++frequency[arr[i]];
    }

    std::vector<int> result;
    for (const auto& entry : frequency) {
        if (entry.second > 1) {
            result.push_back(entry.first);
        }
    }

    if (result.empty()) {
        result.push_back(-1);
    } else {
        std::sort(result.begin(), result.end());
    }
    return result;
}
// The solution uses an unordered_map to count the frequency of each element in the array. After counting, iterate over the map and collect every key whose associated count is greater than 1 into a result vector. If the result vector is empty (no duplicates found), push `-1` into it. Finally, sort the result vector in ascending order. This approach handles all edge cases: all unique elements, multiple duplicates, negative numbers, and zeros. Time complexity is O(n) for counting plus O(k log k) for sorting the k duplicate values, which in the worst case is O(n log n). Space complexity is O(n) for the hash map and result vector.
