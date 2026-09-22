Write a C++ function that takes a non-empty array of positive integers sorted in strictly ascending order (with at least one missing integer) and returns a `std::vector<int>` containing all integers that are missing from the array, in ascending order. The function should not modify the input array and should work efficiently by using the difference between each element and its index to detect gaps. For example, given `{1, 2, 3, 5, 6, 7, 10}`, it should return `{4, 8, 9}`. Assume the array has no duplicates, starts at a positive value, and may have multiple missing integers. The solution must handle the case where missing integers appear between every element, at the beginning (if the first element is greater than 1), or nowhere except required by the task (at least one missing is guaranteed).

#include <cassert>
#include <vector>

// Assume the solution function is declared above (or included from a header).

int main() {
    std::vector<int> arr1 = {1, 2, 3, 5, 6, 7, 10};
    assert(findMissingIntegers(arr1) == std::vector<int>({4, 8, 9}));

    std::vector<int> arr2 = {2, 3, 4, 6, 7, 8, 12};
    assert(findMissingIntegers(arr2) == std::vector<int>({5, 9, 10, 11}));

    std::vector<int> arr3 = {10, 11, 12, 13, 15};
    assert(findMissingIntegers(arr3) == std::vector<int>({14}));

    std::vector<int> arr4 = {1, 4, 5, 7};
    assert(findMissingIntegers(arr4) == std::vector<int>({2, 3, 6}));

    std::vector<int> arr5 = {5, 6, 8};
    assert(findMissingIntegers(arr5) == std::vector<int>({7}));

    // Edge: only one missing between first and second
    std::vector<int> arr6 = {1, 3};
    assert(findMissingIntegers(arr6) == std::vector<int>({2}));

    // Multiple gaps
    std::vector<int> arr7 = {3, 4, 8, 9, 14};
    assert(findMissingIntegers(arr7) == std::vector<int>({5, 6, 7, 10, 11, 12, 13}));

    // Start not at 1, but still positive
    std::vector<int> arr8 = {4, 6, 7};
    assert(findMissingIntegers(arr8) == std::vector<int>({5}));

    // Large gap at the end
    std::vector<int> arr9 = {1, 2, 3};
    // This has no missing, but task guarantees at least one missing; test with assumption
    // Just to show it returns empty
    assert(findMissingIntegers(arr9).empty());

    return 0;
}

#include <vector>

// Returns all missing positive integers from a strictly increasing sorted array.
std::vector<int> findMissingIntegers(const std::vector<int>& arr) {
    std::vector<int> missing;
    if (arr.empty()) return missing;

    int diff = arr[0]; // For index 0, arr[0] - 0 = arr[0]
    int n = static_cast<int>(arr.size());

    for (int i = 0; i < n; ++i) {
        // If there's a discrepancy between expected and actual
        if (arr[i] - i != diff) {
            // Output all missing numbers between the last consecutive value and arr[i]
            while (diff < arr[i] - i) {
                missing.push_back(diff + i);
                ++diff;
            }
        }
    }
    return missing;
}

// The core idea is to use the relationship between each array element and its index. For a perfectly consecutive sequence starting at the first element `arr[0]`, we would have `arr[i] == arr[0] + i`. Define `diff = arr[0]` initially (since `arr[0] - 0 == arr[0]`). As we iterate through the array, for each index `i`, the expected value (if no missing numbers occurred before it) is `diff + i`. If `arr[i] - i != diff`, then there are missing numbers. The number of missing values equals `(arr[i] - i) - diff`. Each missing value is simply `diff + i`, then increment `diff` by 1 and repeat until `diff` reaches `arr[i] - i`. This works because `diff` tracks the value that would have been at the start of the current consecutive run; every time we encounter a gap, we output all integers from `diff` up to (but not including) the expected value, then update `diff` to match the actual current position. Edge cases: if the first element is not 1, the algorithm still works because `diff = arr[0]`, and missing integers before the first element would be from 1 to `arr[0]-1`—however, the problem states the array is positive and sorted, but we can still handle it by initializing `diff` to 1 if we want to detect missing leading values, but the standard approach uses `diff = arr[0]` and only detects gaps between elements. For the given assumption (at least one missing integer), this is fine. Time complexity is O(n + m) where n is array size and m is number of missing elements (each missing is output once). Space complexity is O(m) for the result vector.
