Write a standalone C++ function `hybridSort(int* arr, int n)` that sorts an array of non-negative integers using a hybrid strategy: if the maximum value plus one (call it `k`) is less than or equal to the threshold `n * log2(n)`, use counting sort (which runs in O(n + k)); otherwise, use an in-place merge sort (O(n log n)). The function should handle arrays with duplicate values, size `n ≥ 1`, and must not assume the input array is already sorted. The function should modify the array in place and return nothing. You must implement both counting sort and merge sort internally, with merge sort using a helper merge function. Test with various arrays that trigger both branches of the hybrid decision.

// The solution implements two sorting algorithms: counting sort for small value ranges and merge sort for larger ranges.  
// - **Counting sort** (when `k ≤ n log2(n)`): create a frequency array of size `k` (maximum value + 1), count occurrences, compute cumulative counts to determine positions, then build a sorted output array by iterating from the end to preserve stability. Finally copy back to the original array.  
// - **Merge sort** (when `k > n log2(n)`): recursively split the array into halves until single elements, then merge two sorted halves. The merge function uses two temporary left/right arrays, compares elements, and copies the remainder if one side is exhausted.  
// - **Edge cases**:  
//   - `n = 1` → trivially sorted; both algorithms handle it.  
//   - All elements equal → counting sort works, merge sort also.  
//   - Maximum value is `0` → `k = 1`, then `k ≤ n log2(n)` for any `n ≥ 1`, so counting sort.  
//   - For small `n` and large maximum (e.g., `{1000000}`), merge sort is chosen.  
// - **Complexity**:  
//   - Counting sort: O(n + k) time, O(k + n) auxiliary space (but last copy is in-place; we use O(k) extra).  
//   - Merge sort: O(n log n) time, O(n) auxiliary space (temporary arrays per merge call, but overall recursion uses O(n) per level).  
//   - Hybrid: O(min(n log n, n + k)) time, extra space O(n + k) in worst case if counting sort is used, otherwise O(n).  
// The threshold ensures that when `k` is large compared to `n log n`, merge sort is more efficient.

#include <algorithm>
#include <cmath>
#include <vector>

// Merge two sorted subarrays a[p..q] and a[q+1..r] into a[p..r].
void merge(int a[], int p, int q, int r) {
    int leftLen = q - p + 1;
    int rightLen = r - q;

    std::vector<int> left(a + p, a + q + 1);
    std::vector<int> right(a + q + 1, a + r + 1);

    int i = 0, j = 0, k = p;
    while (i < leftLen && j < rightLen) {
        if (left[i] <= right[j]) {
            a[k++] = left[i++];
        } else {
            a[k++] = right[j++];
        }
    }
    while (i < leftLen) a[k++] = left[i++];
    while (j < rightLen) a[k++] = right[j++];
}

// Recursive merge sort on a[left..right].
void mergeSort(int a[], int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(a, left, mid);
    mergeSort(a, mid + 1, right);
    merge(a, left, mid, right);
}

// Counting sort for non-negative integers with max value k-1.
void countingSort(int a[], int n, int k) {
    std::vector<int> count(k, 0);
    for (int i = 0; i < n; ++i) count[a[i]]++;

    for (int i = 1; i < k; ++i) count[i] += count[i - 1];

    std::vector<int> sorted(n);
    for (int i = n - 1; i >= 0; --i) {
        count[a[i]]--;
        sorted[count[a[i]]] = a[i];
    }
    for (int i = 0; i < n; ++i) a[i] = sorted[i];
}

// Hybrid sort: counting if k <= n log2(n), otherwise merge sort.
void hybridSort(int* arr, int n) {
    if (n <= 0) return;

    int maxVal = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > maxVal) maxVal = arr[i];
    }
    int k = maxVal + 1;

    const double threshold = n * (log(static_cast<double>(n)) / log(2.0));
    if (k <= threshold) {
        countingSort(arr, n, k);
    } else {
        mergeSort(arr, 0, n - 1);
    }
}

#include <cassert>
#include <vector>
#include <algorithm>

int main() {
    // Test counting branch (small k)
    int a1[] = {4, 3, 1, 0, 1, 8, 7, 5};
    hybridSort(a1, 8);
    assert(std::is_sorted(a1, a1 + 8));

    // Test merge branch (large k, small n)
    int a2[] = {1000000, 0, 5, 3, 999999};
    hybridSort(a2, 5);
    assert(std::is_sorted(a2, a2 + 5));

    // All equal
    int a3[] = {7, 7, 7, 7};
    hybridSort(a3, 4);
    assert(std::is_sorted(a3, a3 + 4));

    // Single element
    int a4[] = {42};
    hybridSort(a4, 1);
    assert(a4[0] == 42);

    // Already sorted
    int a5[] = {1, 2, 3, 4};
    hybridSort(a5, 4);
    assert(std::is_sorted(a5, a5 + 4));

    // Reverse sorted with duplicates
    int a6[] = {9, 9, 8, 7, 6, 5, 4, 3, 2};
    hybridSort(a6, 9);
    assert(std::is_sorted(a6, a6 + 9));

    // Large n with moderate k (should trigger counting)
    std::vector<int> vec(1000);
    for (int i = 0; i < 1000; ++i) vec[i] = (i * 37) % 200; // k=200
    int* a7 = vec.data();
    hybridSort(a7, 1000);
    assert(std::is_sorted(a7, a7 + 1000));

    // Large n with huge k (should trigger merge)
    std::vector<int> vec2(1000);
    for (int i = 0; i < 1000; ++i) vec2[i] = (i * 99991) % 1000000; // k=1000000
    int* a8 = vec2.data();
    hybridSort(a8, 1000);
    assert(std::is_sorted(a8, a8 + 1000));

    // Empty array (should do nothing)
    int a9[] = {};
    hybridSort(a9, 0);
    assert(true);

    return 0;
}
