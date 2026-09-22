Write a C++ function that sorts an array containing only the values 0, 1, and 2 in a single pass using the Dutch National Flag algorithm. The function should take a pointer to an integer array and its size, and modify the array in-place so that all 0s come first, followed by all 1s, then all 2s. The input array may be empty, contain only one distinct value, or have all three values mixed arbitrarily. Ensure the function works with `const` correctness where appropriate (though the array itself is modifiable by design).

// The Dutch National Flag algorithm uses three pointers: `low`, `mid`, and `high`. Initially, `low` and `mid` point to the start of the array, and `high` points to the end. The invariant is that all elements before `low` are 0s, all elements between `low` and `mid` (exclusive) are 1s, and all elements after `high` are 2s. The algorithm repeatedly examines the element at `mid`:
// - If it is 0, swap it with the element at `low`, then increment both `low` and `mid`.
// - If it is 1, just increment `mid`.
// - If it is 2, swap it with the element at `high`, then decrement `high` (but do not increment `mid`, because the swapped-in element from `high` has not been inspected yet).
//
// The loop continues while `mid <= high`. Edge cases include: an empty array (immediately return), an array with no 2s (the `high` pointer stays at the end, loop processes all elements), and an array with no 0s (the `low` pointer never moves). The algorithm is stable in the sense that it correctly partitions in one pass. Time complexity is O(n) since each element is examined at most once. Space complexity is O(1) because it uses only a few integer variables and swaps in-place.

#include <utility>

// Sort an array containing only 0, 1, and 2 in-place using Dutch National Flag.
// arr: pointer to the first element of the array
// n: number of elements in the array
void dutchNationalFlagSort(int arr[], int n) {
    if (n <= 1) {
        return; // Nothing to sort for empty or single-element arrays
    }

    int low = 0;
    int mid = 0;
    int high = n - 1;

    while (mid <= high) {
        if (arr[mid] == 0) {
            std::swap(arr[low], arr[mid]);
            ++low;
            ++mid;
        } else if (arr[mid] == 1) {
            ++mid;
        } else { // arr[mid] == 2
            std::swap(arr[mid], arr[high]);
            --high;
        }
    }
}

#include <cassert>
#include <vector>

// Forward declaration of the function under test (already included via solution header)
void dutchNationalFlagSort(int arr[], int n);

int main() {
    // Test 1: Mixed values
    int arr1[] = {0, 1, 1, 0, 1, 2, 1, 2, 0, 0, 0, 1};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    dutchNationalFlagSort(arr1, n1);
    for (int i = 0; i < n1; ++i) {
        if (i < 5) assert(arr1[i] == 0);
        else if (i < 10) assert(arr1[i] == 1);
        else assert(arr1[i] == 2);
    }

    // Test 2: Already sorted
    int arr2[] = {0, 0, 1, 1, 2, 2};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    dutchNationalFlagSort(arr2, n2);
    std::vector<int> expected2 = {0, 0, 1, 1, 2, 2};
    for (int i = 0; i < n2; ++i) assert(arr2[i] == expected2[i]);

    // Test 3: Reverse sorted
    int arr3[] = {2, 2, 1, 1, 0, 0};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    dutchNationalFlagSort(arr3, n3);
    std::vector<int> expected3 = {0, 0, 1, 1, 2, 2};
    for (int i = 0; i < n3; ++i) assert(arr3[i] == expected3[i]);

    // Test 4: All zeros
    int arr4[] = {0, 0, 0};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    dutchNationalFlagSort(arr4, n4);
    for (int i = 0; i < n4; ++i) assert(arr4[i] == 0);

    // Test 5: All ones
    int arr5[] = {1, 1, 1, 1};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    dutchNationalFlagSort(arr5, n5);
    for (int i = 0; i < n5; ++i) assert(arr5[i] == 1);

    // Test 6: All twos
    int arr6[] = {2, 2};
    int n6 = sizeof(arr6) / sizeof(arr6[0]);
    dutchNationalFlagSort(arr6, n6);
    for (int i = 0; i < n6; ++i) assert(arr6[i] == 2);

    // Test 7: Single element
    int arr7[] = {1};
    int n7 = sizeof(arr7) / sizeof(arr7[0]);
    dutchNationalFlagSort(arr7, n7);
    assert(arr7[0] == 1);

    // Test 8: Empty array (n=0)
    int* arr8 = nullptr;
    dutchNationalFlagSort(arr8, 0); // Should not crash

    // Test 9: Large array with random order
    int arr9[] = {2, 0, 1, 2, 0, 1, 2, 0, 1};
    int n9 = sizeof(arr9) / sizeof(arr9[0]);
    dutchNationalFlagSort(arr9, n9);
    std::vector<int> expected9 = {0, 0, 0, 1, 1, 1, 2, 2, 2};
    for (int i = 0; i < n9; ++i) assert(arr9[i] == expected9[i]);

    return 0;
}
