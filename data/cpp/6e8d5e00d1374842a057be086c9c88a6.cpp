Write a C++ function `void selectionSortArray(int arr[], int n)` that sorts an array of integers in ascending order using the selection sort algorithm. The function should modify the array in-place, handle arrays with duplicate values, and work correctly for arrays of any non-negative size (including empty arrays, where it should do nothing). After sorting, the array must be fully sorted in non-decreasing order. Assume the input array is valid and contains `n` integer elements where `n >= 0`.
#include <cassert>
#include <vector>

// The solution function is assumed to be defined above (in the same file).

int main() {
    // Test 1: Empty array
    int empty[] = {};
    selectionSortArray(empty, 0);
    assert(true); // nothing to check, should not crash

    // Test 2: Single element
    int single[] = {42};
    selectionSortArray(single, 1);
    assert(single[0] == 42);

    // Test 3: Already sorted array
    int sorted[] = {1, 2, 3, 4, 5};
    selectionSortArray(sorted, 5);
    assert(sorted[0] == 1 && sorted[4] == 5);

    // Test 4: Reverse sorted array
    int reverse[] = {5, 4, 3, 2, 1};
    selectionSortArray(reverse, 5);
    int expectedReverse[] = {1, 2, 3, 4, 5};
    for (int i = 0; i < 5; ++i) assert(reverse[i] == expectedReverse[i]);

    // Test 5: Array with duplicates
    int duplicates[] = {3, 1, 2, 1, 3};
    selectionSortArray(duplicates, 5);
    int expectedDup[] = {1, 1, 2, 3, 3};
    for (int i = 0; i < 5; ++i) assert(duplicates[i] == expectedDup[i]);

    // Test 6: Negative numbers
    int negatives[] = {-1, -5, -2, -10};
    selectionSortArray(negatives, 4);
    int expectedNeg[] = {-10, -5, -2, -1};
    for (int i = 0; i < 4; ++i) assert(negatives[i] == expectedNeg[i]);

    // Test 7: Large mixed values
    int mixed[] = {100, -100, 0, 50, -50, 25};
    selectionSortArray(mixed, 6);
    int expectedMixed[] = {-100, -50, 0, 25, 50, 100};
    for (int i = 0; i < 6; ++i) assert(mixed[i] == expectedMixed[i]);

    // Test 8: All equal values
    int equal[] = {7, 7, 7};
    selectionSortArray(equal, 3);
    assert(equal[0] == 7 && equal[1] == 7 && equal[2] == 7);

    return 0;
}
#include <vector>
#include <algorithm> // for std::swap if needed, but we use manual swap for clarity

// Sort an array of integers in ascending order using selection sort.
// The array is modified in-place. For n <= 1, no action is taken.
void selectionSortArray(int arr[], int n) {
    for (int i = 0; i < n - 1; ++i) {
        int minIndex = i;
        // Find the index of the minimum element in the unsorted part
        for (int j = i + 1; j < n; ++j) {
            if (arr[j] < arr[minIndex]) {
                minIndex = j;
            }
        }
        // Swap the found minimum with the first element of the unsorted part
        if (minIndex != i) {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }
    }
}
// The selection sort algorithm works by repeatedly finding the minimum element from the unsorted part of the array and swapping it with the first element of that unsorted part. Specifically, for each index `i` from 0 to `n-2`, we scan indices `i` through `n-1` to find the index of the smallest value, then swap that value with the element at index `i`. This places the smallest remaining element in its correct final position. The process is repeated until only one element remains (which is trivially sorted). Edge cases: an empty array (`n==0`) or a single-element array (`n==1`) require no swaps; duplicate values are handled naturally because the algorithm only swaps when a strictly smaller element is found, so duplicates stay in stable relative order (though stability is not required here). Time complexity is `O(n^2)` for both best and worst cases because the inner loop always scans the remaining unsorted portion. Space complexity is `O(1)` as sorting is done in-place with only a few temporary variables.
