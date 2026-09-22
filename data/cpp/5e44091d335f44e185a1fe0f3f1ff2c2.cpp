Write a C++ function `int findKthSmallest(int* arr, int len, int k)` that returns the k-th smallest element (1-indexed, so k=1 returns the minimum, k=len returns the maximum) from an unsorted array of integers, using the Quick Select algorithm based on the partitioning logic shown in the snippet. The function must handle arrays with duplicate values correctly. If `k` is out of bounds (less than 1 or greater than `len`), the function should return `INT_MIN` (from `<climits>`) as an error indicator. The function must not sort the entire array; it should only partially partition as needed, and it should not use any additional dynamically allocated arrays (operate in-place on the input array, but you may modify its order). Assume `len >= 0`; if `len` is 0 and `k` is within bounds? Since no valid k exists, return `INT_MIN` for any k when `len==0`.

// The solution leverages the partitioning step from the given Quick Sort snippet: `Partition` rearranges the subarray `arr[low..high]` so that elements less than or equal to the pivot are to the left, and elements greater are to the right, returning the final pivot index. For Quick Select, we recursively or iteratively target only the side that contains the k-th smallest element. Key edge cases: duplicate values must not break the logic because the partition uses `>` for the right scan and `<=` for the left scan, ensuring equal elements stay on the left, which preserves the invariant that the pivot index is correctly positioned relative to all elements (pivot is the (index-from-start+1)-th smallest). If `k==pivotIndex+1` (1-indexed), we return `arr[pivotIndex]`. If `k` is smaller, we recurse on the left; if larger, on the right. Out-of-bounds k or empty array returns `INT_MIN`. Time complexity is average \(O(n)\) per call, worst-case \(O(n^2)\) when pivot is always the smallest or largest (e.g., already sorted array), but this is standard for Quick Select. Space complexity is \(O(\log n)\) for the recursion stack on average, \(O(n)\) worst-case.

#include <climits>

// Partitions arr[low..high] using arr[low] as pivot.
// Returns the final index of the pivot.
int partitionForSelect(int* arr, int low, int high) {
    int pivot = arr[low];
    while (low < high) {
        while (low < high && arr[high] > pivot) {
            high--;
        }
        if (low < high) {
            arr[low] = arr[high];
        }
        while (low < high && arr[low] <= pivot) {
            low++;
        }
        if (low < high) {
            arr[high] = arr[low];
        }
    }
    arr[low] = pivot;
    return low;
}

// Recursive helper for Quick Select.
int quickSelectHelper(int* arr, int low, int high, int k) {
    if (low > high) {
        return INT_MIN; // Should not happen for valid k.
    }
    int pivotIndex = partitionForSelect(arr, low, high);
    // k is 1-indexed; pivotIndex is 0-indexed.
    int rank = pivotIndex - low + 1; // number of elements up to pivot in this subarray
    if (rank == k) {
        return arr[pivotIndex];
    } else if (k < rank) {
        return quickSelectHelper(arr, low, pivotIndex - 1, k);
    } else {
        return quickSelectHelper(arr, pivotIndex + 1, high, k - rank);
    }
}

// Returns the k-th smallest element (1-indexed) in arr of length len.
// Returns INT_MIN if k is out of bounds or len==0.
int findKthSmallest(int* arr, int len, int k) {
    if (len <= 0 || k < 1 || k > len) {
        return INT_MIN;
    }
    return quickSelectHelper(arr, 0, len - 1, k);
}

#include <cassert>
#include <climits>

// Assume findKthSmallest is defined above.

int main() {
    // Basic cases
    int a1[] = {3, 1, 2};
    assert(findKthSmallest(a1, 3, 1) == 1);
    assert(findKthSmallest(a1, 3, 2) == 2);
    assert(findKthSmallest(a1, 3, 3) == 3);

    // Duplicates
    int a2[] = {5, 5, 3, 5, 1};
    assert(findKthSmallest(a2, 5, 1) == 1);
    assert(findKthSmallest(a2, 5, 2) == 3);
    assert(findKthSmallest(a2, 5, 3) == 5);
    assert(findKthSmallest(a2, 5, 4) == 5);
    assert(findKthSmallest(a2, 5, 5) == 5);

    // Negative numbers
    int a3[] = {-3, -1, -2, 0, 4};
    assert(findKthSmallest(a3, 5, 1) == -3);
    assert(findKthSmallest(a3, 5, 3) == -1);
    assert(findKthSmallest(a3, 5, 5) == 4);

    // Single element
    int a4[] = {42};
    assert(findKthSmallest(a4, 1, 1) == 42);

    // Out of bounds k
    int a5[] = {1, 2};
    assert(findKthSmallest(a5, 2, 0) == INT_MIN);
    assert(findKthSmallest(a5, 2, 3) == INT_MIN);

    // Empty array
    int* a6 = nullptr;
    assert(findKthSmallest(a6, 0, 1) == INT_MIN);

    // Already sorted large array
    int a7[] = {1, 2, 3, 4, 5, 6, 7};
    assert(findKthSmallest(a7, 7, 4) == 4);
    assert(findKthSmallest(a7, 7, 1) == 1);
    assert(findKthSmallest(a7, 7, 7) == 7);

    return 0;
}
