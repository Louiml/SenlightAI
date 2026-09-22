/*
Write a C++ function named `findFirstPosition` that takes a vector of integers, the number of elements to search, and a target value. The function should return the 1-based position (index + 1) of the first occurrence of the target value in the array. If the target is not found, return 0. The function must be `const`-correct: it should accept the array as a `const int*` or a `const std::vector<int>&`, and it should not modify the input. Handle edge cases such as an empty array, a target that appears multiple times (return the first), and a target that does not exist.
*/

#include <vector>

// Returns the 1-based position of the first occurrence of target in the first n elements of arr.
// If target is not found, returns 0.
int findFirstPosition(const int arr[], int n, int target) {
    for (int i = 0; i < n; ++i) {
        if (arr[i] == target) {
            return i + 1; // 1-based position
        }
    }
    return 0; // not found
}

#include <cassert>

// The function under test (must be declared/defined before this main)
int findFirstPosition(const int arr[], int n, int target);

int main() {
    // Test 1: Empty array
    int empty[] = {};
    assert(findFirstPosition(empty, 0, 5) == 0);

    // Test 2: Single element found
    int single[] = {7};
    assert(findFirstPosition(single, 1, 7) == 1);

    // Test 3: Single element not found
    assert(findFirstPosition(single, 1, 3) == 0);

    // Test 4: Multiple elements, target at first position
    int arr1[] = {3, 8, 2, 8};
    assert(findFirstPosition(arr1, 4, 3) == 1);

    // Test 5: Target appears multiple times, returns first occurrence
    assert(findFirstPosition(arr1, 4, 8) == 2);

    // Test 6: Target not in array
    assert(findFirstPosition(arr1, 4, 99) == 0);

    // Test 7: Target at last position (1-based = n)
    int arr2[] = {1, 2, 3, 4};
    assert(findFirstPosition(arr2, 4, 4) == 4);

    // Test 8: Negative numbers
    int arr3[] = {-5, -2, -9};
    assert(findFirstPosition(arr3, 3, -2) == 2);

    // Test 9: n smaller than actual array (partial search)
    int arr4[] = {10, 20, 30, 40};
    assert(findFirstPosition(arr4, 2, 30) == 0); // 30 is outside the first 2 elements

    // Test 10: n = 1, not found
    assert(findFirstPosition(single, 1, 0) == 0);

    return 0;
}

// The solution is a straightforward linear scan of the array from index 0 to n-1, comparing each element to the target. The moment a match is found, return `i + 1` (the 1-based position). If the loop completes without finding the target, return 0. This approach handles all edge cases: an empty array immediately returns 0 without entering the loop; duplicate targets are resolved because the first match triggers the return; and a missing target ends with the baseline return of 0. Time complexity is O(n) in the worst case (target absent or at the end), and space complexity is O(1) since we only use a loop counter. The implementation should be `const`-correct by taking the array as a `const int*` and `n` as an `int`, ensuring the function does not modify the input.
