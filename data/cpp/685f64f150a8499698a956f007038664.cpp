Write a C++ function named `reverseRange` that takes a pointer to the first element of an integer array, the array size `n`, and two indices `i` and `j` (with `0 <= i <= j < n`), and recursively reverses the elements in the subarray from index `i` to index `j` inclusive. The function should modify the array in place and return `void`. You must implement the reversal using recursion (no loops), with the base case occurring when `i >= j`. After the function completes, the elements in the range `[i, j]` should be in reversed order, while elements outside that range remain untouched. For example, given the array `{1,2,3,4,5}` and indices `1` and `3`, the resulting array should be `{1,4,3,2,5}`. Ensure your function handles edge cases such as `i == j` (no change) and `i == 0`, `j == n-1` (full array reversal).

The solution uses a direct recursive approach: at each call, swap the elements at positions `i` and `j`, then make a recursive call with `i+1` and `j-1`. The recursion terminates when `i >= j`, meaning the subarray has zero or one element left (or the pointers have crossed). For the base case where `i == j`, no swap is needed, and the function returns. The algorithm performs exactly `floor((j-i+1)/2)` swaps, each taking constant time, so the time complexity is O(k) where `k = j-i+1` is the length of the subarray. The space complexity is O(k) in the worst case due to the recursion stack depth, as each recursive call reduces the range by two elements. The function must handle invalid indices gracefully? The task assumes `0 <= i <= j < n` per specification, so no error checking is necessary, but if robust behavior is desired, one could clamp or return early, but not required here. Key edge cases: `i == j` (no-op), `j = i+1` (one swap then stop), and empty or single-element array scenario `n=0` or `n=1` should be safe to call with `0, n-1`, but the problem likely expects `n > 0` per typical usage.

#include <utility>  // for std::swap

// Recursively reverse the subarray arr[i..j] in place.
// Precondition: 0 <= i <= j < n, where n is the size of arr.
void reverseRange(int* arr, int n, int i, int j) {
    // Base case: if indices have crossed or are equal, nothing to do.
    if (i >= j) {
        return;
    }

    // Swap the current outer elements.
    std::swap(arr[i], arr[j]);

    // Recurse on the inner subarray.
    reverseRange(arr, n, i + 1, j - 1);
}

int main() {
    // Test 1: Full array reversal (i=0, j=n-1)
    int arr1[5] = {1, 2, 3, 4, 5};
    reverseRange(arr1, 5, 0, 4);
    assert(arr1[0] == 5 && arr1[1] == 4 && arr1[2] == 3 && arr1[3] == 2 && arr1[4] == 1);

    // Test 2: Subarray reversal in the middle
    int arr2[5] = {1, 2, 3, 4, 5};
    reverseRange(arr2, 5, 1, 3);
    assert(arr2[0] == 1 && arr2[1] == 4 && arr2[2] == 3 && arr2[3] == 2 && arr2[4] == 5);

    // Test 3: Single-element range (i == j) — no change
    int arr3[3] = {9, 8, 7};
    reverseRange(arr3, 3, 1, 1);
    assert(arr3[0] == 9 && arr3[1] == 8 && arr3[2] == 7);

    // Test 4: Reverse first two elements
    int arr4[4] = {10, 20, 30, 40};
    reverseRange(arr4, 4, 0, 1);
    assert(arr4[0] == 20 && arr4[1] == 10 && arr4[2] == 30 && arr4[3] == 40);

    // Test 5: Reverse last two elements
    int arr5[4] = {10, 20, 30, 40};
    reverseRange(arr5, 4, 2, 3);
    assert(arr5[0] == 10 && arr5[1] == 20 && arr5[2] == 40 && arr5[3] == 30);

    // Test 6: Reverse two adjacent in a larger array
    int arr6[6] = {1, 2, 3, 4, 5, 6};
    reverseRange(arr6, 6, 2, 3);
    assert(arr6[0] == 1 && arr6[1] == 2 && arr6[2] == 4 && arr6[3] == 3 && arr6[4] == 5 && arr6[5] == 6);

    // Test 7: Reverse entire array of size 1 (i==j==0)
    int arr7[1] = {42};
    reverseRange(arr7, 1, 0, 0);
    assert(arr7[0] == 42);

    // Test 8: Reverse entire array of size 2
    int arr8[2] = {3, 7};
    reverseRange(arr8, 2, 0, 1);
    assert(arr8[0] == 7 && arr8[1] == 3);

    // Test 9: Reverse a middle subarray of odd length (e.g., 3 elements)
    int arr9[7] = {5, 6, 7, 8, 9, 10, 11};
    reverseRange(arr9, 7, 2, 4);
    assert(arr9[0] == 5 && arr9[1] == 6 && arr9[2] == 9 && arr9[3] == 8 && arr9[4] == 7 && arr9[5] == 10 && arr9[6] == 11);

    // Test 10: Reverse the whole array again to check symmetry (i=0, j=n-1)
    int arr10[4] = {1, 2, 3, 4};
    reverseRange(arr10, 4, 0, 3);  // becomes 4,3,2,1
    reverseRange(arr10, 4, 0, 3);  // becomes 1,2,3,4 again
    assert(arr10[0] == 1 && arr10[1] == 2 && arr10[2] == 3 && arr10[3] == 4);
}
