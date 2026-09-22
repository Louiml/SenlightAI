Write a C++ function `bool isSortedAfterQuickSort(int arr[], int n)` that takes an array of integers and its size, performs a randomized in-place quicksort using the Lomuto partition scheme with a randomly chosen pivot (seeded by the caller), and then returns `true` if the resulting array is sorted in non-decreasing order and `false` otherwise. The function must sort the array in-place and must not print anything. Assume `n >= 0`; for `n == 0` or `n == 1`, the array is trivially sorted, so return `true` without modifying it.

#include <cassert>
#include <cstdlib>

// Declare the solution function.
bool isSortedAfterQuickSort(int arr[], int n);

int main() {
    // Seed the random generator deterministically.
    std::srand(0);

    // Test 1: Empty array.
    int emptyArr[] = {};
    assert(isSortedAfterQuickSort(emptyArr, 0) == true);

    // Test 2: Single element.
    int singleArr[] = {42};
    assert(isSortedAfterQuickSort(singleArr, 1) == true);
    assert(singleArr[0] == 42); // unchanged

    // Test 3: Already sorted.
    int sortedArr[] = {1, 2, 3, 4, 5};
    assert(isSortedAfterQuickSort(sortedArr, 5) == true);
    assert(sortedArr[0] == 1 && sortedArr[4] == 5);

    // Test 4: Reverse sorted.
    int reverseArr[] = {5, 4, 3, 2, 1};
    assert(isSortedAfterQuickSort(reverseArr, 5) == true);
    assert(reverseArr[0] == 1 && reverseArr[4] == 5);

    // Test 5: Duplicates.
    int dupArr[] = {3, 1, 3, 1, 3};
    assert(isSortedAfterQuickSort(dupArr, 5) == true);
    assert(dupArr[0] == 1 && dupArr[4] == 3);

    // Test 6: Negative numbers.
    int negArr[] = {-5, -1, -10, 3, 0};
    assert(isSortedAfterQuickSort(negArr, 5) == true);
    assert(negArr[0] == -10 && negArr[4] == 3);

    // Test 7: Large random array (deterministic) - check sortedness.
    const int LARGE = 1000;
    int largeArr[LARGE];
    for (int i = 0; i < LARGE; ++i) {
        largeArr[i] = std::rand() % 1000;
    }
    assert(isSortedAfterQuickSort(largeArr, LARGE) == true);
    for (int i = 1; i < LARGE; ++i) {
        assert(largeArr[i - 1] <= largeArr[i]);
    }

    // Test 8: Random with fixed seed - verify correctness against a known sorted copy.
    std::srand(123);
    int arr[] = {7, 2, 9, 4, 1, 8, 3, 6, 5};
    assert(isSortedAfterQuickSort(arr, 9) == true);
    for (int i = 1; i < 9; ++i) {
        assert(arr[i - 1] <= arr[i]);
    }
    // Check that it's a permutation (contains 1..9).
    int expected[9] = {1,2,3,4,5,6,7,8,9};
    for (int i = 0; i < 9; ++i) {
        assert(arr[i] == expected[i]);
    }

    return 0;
}

#include <cstdlib>

// Randomized quicksort using Lomuto partition.
// Precondition: arr is an array of size n. After sorting, arr is in non-decreasing order.
// Returns true if arr is sorted after sorting, false otherwise.
bool isSortedAfterQuickSort(int arr[], int n) {
    // Helper lambda for partition inside the main function.
    auto partition = [&](int p, int r) -> int {
        int x = arr[r]; // pivot value (last element after possible swap)
        int i = p - 1;
        for (int j = p; j <= r - 1; ++j) {
            if (arr[j] <= x) {
                ++i;
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        int temp = arr[i + 1];
        arr[i + 1] = arr[r];
        arr[r] = temp;
        return i + 1;
    };

    // Recursive lambda for quicksort.
    auto quickSortRec = [&](auto&& self, int p, int r) -> void {
        if (p < r) {
            // Random pivot index between p and r inclusive.
            int randIndex = p + std::rand() % (r - p + 1);
            // Swap pivot with last element.
            int temp = arr[r];
            arr[r] = arr[randIndex];
            arr[randIndex] = temp;
            // Partition and recurse.
            int q = partition(p, r);
            self(self, p, q - 1);
            self(self, q + 1, r);
        }
    };

    // Perform the sort if there are at least 2 elements.
    if (n > 1) {
        quickSortRec(quickSortRec, 0, n - 1);
    }

    // Check if array is sorted.
    for (int i = 1; i < n; ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

// The task is to implement a randomized quicksort with a Lomuto partition. The partition step selects the last element as pivot after swapping it with a random element from the range `[p, r]`. The partition function rearranges the subarray so that all elements <= pivot are on the left, places the pivot in its correct position, and returns that pivot index. Quicksort recursively sorts the left and right subarrays. Edge cases: empty or single-element arrays are already sorted; duplicate values are handled by using `<=` in the partition comparison. Since random pivot selection is used, the expected time complexity is `O(n log n)` and worst-case `O(n^2)`, with `O(log n)` recursion stack space for balanced partitions. The final check for sortedness runs in `O(n)` time and `O(1)` space. For verification with deterministic tests, the function should be called with a pre-seeded random generator (e.g., `srand(0)`) in the test harness.
