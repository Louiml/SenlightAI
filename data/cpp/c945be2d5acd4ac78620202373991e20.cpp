// Write a C++ function that takes a vector of integers and returns the number of inversions in it, where an inversion is a pair of indices `(i, j)` with `i < j` and `arr[i] > arr[j]`. The function must use a modified merge sort to count inversions efficiently, and it must handle an empty vector (returning 0) and vectors with duplicate values (where duplicates do not count as inversions). The solution should not modify the input vector; instead, it should work on a copy or use a temporary array internally.
#include <cassert>
#include <vector>

// Include the solution here (or link to it)

int main() {
    // Test 1: Empty vector
    assert(countInversions({}) == 0);

    // Test 2: Single element
    assert(countInversions({42}) == 0);

    // Test 3: Already sorted (ascending) - no inversions
    assert(countInversions({1, 2, 3, 4}) == 0);

    // Test 4: Reverse sorted - maximum inversions n*(n-1)/2 = 4*3/2 = 6
    assert(countInversions({4, 3, 2, 1}) == 6);

    // Test 5: General mixed case
    assert(countInversions({1, 20, 6, 4, 5}) == 5); // (20,6), (20,4), (20,5), (6,4), (6,5)

    // Test 6: Duplicates - equal values do not count
    assert(countInversions({2, 2, 2}) == 0);

    // Test 7: Duplicates with inversions
    assert(countInversions({3, 3, 1}) == 2); // (3,1) twice

    // Test 8: Large value for correctness (simple loop check)
    std::vector<int> data = {5, 4, 3, 2, 1};
    assert(countInversions(data) == 10);

    // Test 9: Input vector not modified
    std::vector<int> original = {3, 1, 2};
    countInversions(original);
    assert(original == std::vector<int>({3, 1, 2}));

    // Test 10: Large - but small enough to manually verify
    assert(countInversions({1, 3, 2, 5, 4}) == 2); // (3,2), (5,4)

    return 0;
}
#include <vector>

// Helper function that merges two sorted halves of arr (from left to mid, and mid+1 to right)
// into temp_arr, counting cross inversions, and writes the sorted result back into arr.
long long mergeAndCount(std::vector<int>& arr, std::vector<int>& temp_arr, int left, int mid, int right) {
    int i = left;      // index for left subarray
    int j = mid + 1;   // index for right subarray
    int k = left;      // index for temp_arr
    long long inv_count = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) {
            temp_arr[k++] = arr[i++];
        } else {
            // arr[j] is smaller than arr[i], and since left subarray is sorted,
            // all elements from i to mid are > arr[j] => they form inversions.
            temp_arr[k++] = arr[j++];
            inv_count += (mid - i + 1);
        }
    }

    // Copy remaining elements of left subarray, if any
    while (i <= mid) {
        temp_arr[k++] = arr[i++];
    }

    // Copy remaining elements of right subarray, if any
    while (j <= right) {
        temp_arr[k++] = arr[j++];
    }

    // Copy sorted subarray back to original array
    for (i = left; i <= right; ++i) {
        arr[i] = temp_arr[i];
    }

    return inv_count;
}

// Recursive function that splits arr and counts inversions, modifying arr to be sorted.
long long mergeSortAndCount(std::vector<int>& arr, std::vector<int>& temp_arr, int left, int right) {
    long long inv_count = 0;
    if (left < right) {
        int mid = left + (right - left) / 2;  // avoid overflow
        inv_count += mergeSortAndCount(arr, temp_arr, left, mid);
        inv_count += mergeSortAndCount(arr, temp_arr, mid + 1, right);
        inv_count += mergeAndCount(arr, temp_arr, left, mid, right);
    }
    return inv_count;
}

// Public function: counts inversions in the input vector without modifying it.
// Returns the number of pairs (i, j) with i < j and arr[i] > arr[j].
long long countInversions(const std::vector<int>& input) {
    if (input.size() < 2) {
        return 0;
    }

    // Work on a copy so the original remains unchanged
    std::vector<int> arr = input;
    std::vector<int> temp_arr(input.size());

    return mergeSortAndCount(arr, temp_arr, 0, static_cast<int>(input.size()) - 1);
}
// The task requires counting inversions using a divide-and-conquer approach based on merge sort. The key idea is that during the merge step, when an element from the right subarray is placed before an element from the left subarray, all remaining elements in the left subarray (from the current index to the middle) form inversions with that right element. The algorithm recursively splits the array into two halves, counts inversions within each half, and then counts inversions across the halves during merging. Important edge cases include an empty vector (return 0), a single element (return 0), already sorted arrays (return 0), reverse-sorted arrays (maximum inversions = n*(n-1)/2), and duplicate elements (they do not form inversions because the condition is strictly greater). The time complexity is O(n log n) and the auxiliary space is O(n) because a temporary array is used during merging. To preserve the original input, the solution makes a copy of the input vector and sorts that copy while counting.
