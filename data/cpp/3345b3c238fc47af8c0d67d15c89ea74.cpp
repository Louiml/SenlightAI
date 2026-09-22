// Write a C++ function `void parallelQuickSort(int* arr, int size)` that sorts an integer array in ascending order using a multi-threaded recursive quicksort. The function should use POSIX threads (`pthread`) to spawn two threads for each recursive call — one for the left partition (elements ≤ pivot) and one for the right partition (elements > pivot) — and join them before returning. The sorting must be done in-place on the original array. The function should handle empty (size = 0) and single-element (size = 1) arrays without doing anything. Assume the input array is valid (non-null if size > 0). The solution must avoid undefined behavior such as using pointers to local stack objects after the function returns. The function should be self-contained with all necessary headers inside the function signature file (provided as code only, no main).

#include <cassert>

int main() {
    // Test 1: Typical unsorted array
    int arr1[] = {5, 2, 9, 1, 7};
    parallelQuickSort(arr1, 5);
    assert(arr1[0] == 1 && arr1[1] == 2 && arr1[2] == 5 && arr1[3] == 7 && arr1[4] == 9);

    // Test 2: Already sorted array
    int arr2[] = {1, 2, 3, 4};
    parallelQuickSort(arr2, 4);
    assert(arr2[0] == 1 && arr2[1] == 2 && arr2[2] == 3 && arr2[3] == 4);

    // Test 3: Reverse sorted array
    int arr3[] = {9, 8, 7, 6};
    parallelQuickSort(arr3, 4);
    assert(arr3[0] == 6 && arr3[1] == 7 && arr3[2] == 8 && arr3[3] == 9);

    // Test 4: Duplicate values
    int arr4[] = {3, 1, 3, 2, 1};
    parallelQuickSort(arr4, 5);
    assert(arr4[0] == 1 && arr4[1] == 1 && arr4[2] == 2 && arr4[3] == 3 && arr4[4] == 3);

    // Test 5: Negative numbers
    int arr5[] = {-3, -1, -4, -2};
    parallelQuickSort(arr5, 4);
    assert(arr5[0] == -4 && arr5[1] == -3 && arr5[2] == -2 && arr5[3] == -1);

    // Test 6: Single element
    int arr6[] = {42};
    parallelQuickSort(arr6, 1);
    assert(arr6[0] == 42);

    // Test 7: Empty array (size 0)
    int* arr7 = nullptr;
    parallelQuickSort(arr7, 0); // should not crash

    // Test 8: Mixed large array
    int arr8[] = {10, -5, 0, 3, 8, -2, 7};
    parallelQuickSort(arr8, 7);
    assert(arr8[0] == -5 && arr8[1] == -2 && arr8[2] == 0 && arr8[3] == 3 && arr8[4] == 7 && arr8[5] == 8 && arr8[6] == 10);

    // Test 9: All equal elements
    int arr9[] = {7, 7, 7, 7};
    parallelQuickSort(arr9, 4);
    assert(arr9[0] == 7 && arr9[1] == 7 && arr9[2] == 7 && arr9[3] == 7);

    // Test 10: Larger random-like array (order known)
    int arr10[] = {4, 1, 5, 2, 6, 3};
    parallelQuickSort(arr10, 6);
    assert(arr10[0] == 1 && arr10[1] == 2 && arr10[2] == 3 && arr10[3] == 4 && arr10[4] == 5 && arr10[5] == 6);

    return 0;
}

#include <pthread.h>
#include <cstddef>

struct SortArgs {
    int* arr;
    int first;
    int last;
};

// Partition using the first element as pivot (Lomuto scheme)
int partition(int* arr, int first, int last) {
    int pivotValue = arr[first];
    int pivotIndex = first;

    for (int i = first + 1; i <= last; ++i) {
        if (arr[i] <= pivotValue) {
            ++pivotIndex;
            std::swap(arr[i], arr[pivotIndex]);
        }
    }
    std::swap(arr[first], arr[pivotIndex]);
    return pivotIndex;
}

// Thread entry point
void* parallelQuickSortWorker(void* arg) {
    SortArgs* args = static_cast<SortArgs*>(arg);
    int first = args->first;
    int last = args->last;
    int* arr = args->arr;

    delete args; // free the heap-allocated struct

    if (first < last) {
        int pivotIndex = partition(arr, first, last);

        // Allocate heap structs for each subproblem to avoid dangling pointers
        SortArgs* leftArgs = new SortArgs{arr, first, pivotIndex - 1};
        SortArgs* rightArgs = new SortArgs{arr, pivotIndex + 1, last};

        pthread_t leftThread, rightThread;

        // Create left thread if left range is non-empty
        pthread_create(&leftThread, nullptr, parallelQuickSortWorker, leftArgs);
        // Create right thread if right range is non-empty
        pthread_create(&rightThread, nullptr, parallelQuickSortWorker, rightArgs);

        pthread_join(leftThread, nullptr);
        pthread_join(rightThread, nullptr);
    }
    return nullptr;
}

// Public function: sorts arr in ascending order in place
void parallelQuickSort(int* arr, int size) {
    if (size <= 1) return;
    SortArgs* initialArgs = new SortArgs{arr, 0, size - 1};
    parallelQuickSortWorker(initialArgs);
}

// The core algorithm is a standard quicksort using the Lomuto partition scheme, but modified for parallel execution. The `pivot` function selects the first element as the pivot, scans the rest of the subarray, and places the pivot in its correct sorted position by swapping elements ≤ pivot to the left side. The recursive function receives a struct containing the array pointer and the inclusive range `[first, last]`. For ranges with more than one element, it partitions, then creates two child `args` structs for the left and right subranges. It launches two threads via `pthread_create`, each calling the same recursive function on its respective subrange, then joins both threads to ensure the recursion completes before returning. Critical edge cases: (1) when `first >= last`, the function must do nothing and return immediately; (2) to avoid dangling pointers, each thread’s `args` struct must be allocated on the heap (using `new`) and freed appropriately. However, for safety and simplicity, the reference solution will allocate `args` on the heap inside the function and have the thread function `delete` its argument after processing to avoid memory leaks. In the multi-threaded version, thread creation overhead is significant, so the algorithm is not efficient for small arrays, but that is not a concern for correctness. Time complexity is average-case \(O(n \log n)\) with maximum parallelism, and worst-case \(O(n^2)\) when the pivot is consistently poor. Space complexity is \(O(\log n)\) for the recursion stack in the sequential case, but with threads each having their own stack, the total stack memory used grows with the number of concurrent threads, which is bounded by \(O(\log n)\) if the partition is balanced. Since the array is shared, no copying is needed; each thread operates on disjoint subranges.
