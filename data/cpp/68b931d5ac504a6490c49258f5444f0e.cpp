Write a C++ function named `findDuplicates` that takes an array of integers `arr` and its size `n` as input, and returns a `std::vector<int>` containing all integers that appear more than once in the array, sorted in ascending order. If no duplicate exists, the function should return a vector containing `-1` (i.e., `{-1}`). The array may contain non-negative integers, and there is no guarantee that the array is sorted. The function must handle edge cases such as an array with only one element, all unique elements, or multiple duplicates of the same value.
// The solution uses a `std::map<int, int>` to count the frequency of each distinct integer in the array. Iterate through the input array once, incrementing the count for each element. After building the frequency map, iterate through the map (which automatically sorts keys in ascending order) and collect every key whose count is greater than 1 into a result vector. If the result vector is non-empty, return it; otherwise, return a vector containing `-1`. Edge cases: an array of size 0 or 1 will have no duplicates, returning `{-1}`; duplicates like `[5, 5]` will correctly produce `[5]`; all unique elements yield `{-1}`. Time complexity is O(n log n) due to map insertions (each insertion is O(log n) for balanced tree), and space complexity is O(n) for the map. Alternatively, using a hash map (`unordered_map`) would give O(n) average time, but the map approach guarantees sorted output without extra sorting.
#include <vector>
#include <map>

// Returns all integers that appear more than once in arr, sorted ascending.
// If no duplicates exist, returns a vector containing -1.
std::vector<int> findDuplicates(const int arr[], int n) {
    std::map<int, int> frequency;
    for (int i = 0; i < n; ++i) {
        frequency[arr[i]]++;
    }

    std::vector<int> result;
    for (const auto& entry : frequency) {
        if (entry.second > 1) {
            result.push_back(entry.first);
        }
    }

    if (!result.empty()) {
        return result;
    }
    return {-1};
}
#include <cassert>
#include <vector>

int main() {
    // Case 1: No duplicates
    int arr1[] = {1, 2, 3, 4};
    std::vector<int> res1 = findDuplicates(arr1, 4);
    assert(res1 == std::vector<int>({-1}));

    // Case 2: Single duplicate
    int arr2[] = {4, 2, 4, 1};
    std::vector<int> res2 = findDuplicates(arr2, 4);
    assert(res2 == std::vector<int>({4}));

    // Case 3: Multiple duplicates, unsorted input
    int arr3[] = {5, 1, 5, 2, 1, 3};
    std::vector<int> res3 = findDuplicates(arr3, 6);
    assert(res3 == std::vector<int>({1, 5}));

    // Case 4: All elements same
    int arr4[] = {7, 7, 7, 7};
    std::vector<int> res4 = findDuplicates(arr4, 4);
    assert(res4 == std::vector<int>({7}));

    // Case 5: Single element
    int arr5[] = {42};
    std::vector<int> res5 = findDuplicates(arr5, 1);
    assert(res5 == std::vector<int>({-1}));

    // Case 6: Empty array (size 0)
    int arr6[] = {};
    std::vector<int> res6 = findDuplicates(arr6, 0);
    assert(res6 == std::vector<int>({-1}));

    // Case 7: Duplicates with zeros
    int arr7[] = {0, 0, 1, 2, 2, 2};
    std::vector<int> res7 = findDuplicates(arr7, 6);
    assert(res7 == std::vector<int>({0, 2}));

    // Case 8: Duplicates in descending order, expect ascending output
    int arr8[] = {9, 8, 9, 8, 7};
    std::vector<int> res8 = findDuplicates(arr8, 5);
    assert(res8 == std::vector<int>({8, 9}));

    return 0;
}
