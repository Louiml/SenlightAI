// Write a C++ function that rearranges an array of non-negative integers such that each element at index `i` is replaced with the value `i` if `i` appears in the original array, and with `-1` otherwise. For example, given the array `{4, 0, 2, 1, 4}`, the output should be `{0, 1, 2, -1, 4}` because indices 0,1,2,4 appear in the original array but index 3 does not. The input array size can be up to 1000, and values are guaranteed to be in the range `[0, size-1]`. The function should modify the array in-place and return nothing.

The main idea is to first determine which indices are present in the original array. Since all values are within `[0, size-1]`, we can create a frequency array (or boolean presence array) of size equal to the input array size, initialized to zero. Iterate through the input array once, and for each value `v` encountered, set `presence[v] = 1` (or increment a counter). Then, iterate through each index `i` from `0` to `size-1`. If `presence[i] == 1`, set `arr[i] = i`; otherwise set `arr[i] = -1`. This works even with duplicates because we only need to know whether an index appears at least once. Edge cases include an array of size 1 (where index 0 is always present because value is 0), and an array where no index appears (impossible for size>0 because values are in range). Time complexity is O(n) for two passes, and space complexity is O(n) for the presence array. The approach avoids sorting or extra complex structures.

#include <vector>
#include <algorithm>

// Rearrange the array in-place: set arr[i] = i if i exists in arr, else -1.
// Assumes all values in arr are in [0, size-1].
void rearrangeArray(int* arr, int size) {
    if (size <= 0) return;
    
    // Presence track: presence[i] == 1 if i appears in the original array.
    std::vector<int> presence(size, 0);
    for (int i = 0; i < size; ++i) {
        int val = arr[i];
        if (val >= 0 && val < size) {
            presence[val] = 1;
        }
    }
    
    // Modify array according to presence.
    for (int i = 0; i < size; ++i) {
        arr[i] = presence[i] ? i : -1;
    }
}

#include <cassert>

int main() {
    // Test 1: Example with missing index
    int arr1[] = {4, 0, 2, 1, 4};
    rearrangeArray(arr1, 5);
    assert(arr1[0] == 0);
    assert(arr1[1] == 1);
    assert(arr1[2] == 2);
    assert(arr1[3] == -1);
    assert(arr1[4] == 4);

    // Test 2: All indices present
    int arr2[] = {2, 0, 1};
    rearrangeArray(arr2, 3);
    assert(arr2[0] == 0);
    assert(arr2[1] == 1);
    assert(arr2[2] == 2);

    // Test 3: Single element
    int arr3[] = {0};
    rearrangeArray(arr3, 1);
    assert(arr3[0] == 0);

    // Test 4: Duplicates cause some indices missing
    int arr4[] = {0, 0, 0};
    rearrangeArray(arr4, 3);
    assert(arr4[0] == 0);
    assert(arr4[1] == -1);
    assert(arr4[2] == -1);

    // Test 5: All same max value
    int arr5[] = {3, 3, 3, 3};
    rearrangeArray(arr5, 4);
    assert(arr5[0] == -1);
    assert(arr5[1] == -1);
    assert(arr5[2] == -1);
    assert(arr5[3] == 3);

    // Test 6: Already correct order
    int arr6[] = {0, 1, 2, 3};
    rearrangeArray(arr6, 4);
    assert(arr6[0] == 0);
    assert(arr6[1] == 1);
    assert(arr6[2] == 2);
    assert(arr6[3] == 3);

    // Test 7: Size 0 edge case (no assertions, just ensure no crash)
    int* arr7 = nullptr;
    rearrangeArray(arr7, 0);

    return 0;
}
