/*
Write a C++ function `rotateLeftByK(int arr[], int n, int k)` that rotates a given array of integers to the **left** by `k` positions (i.e., the first element moves to the end, and so on). The array size `n` is positive, and `k` can be any non-negative integer (including values larger than `n`). The rotation must be performed **in place**, modifying the input array directly and returning nothing. For example, given `[1,2,3,4]` and `k=2`, the result should be `[3,4,1,2]`. The function must handle `k=0` and `k` being a multiple of `n` gracefully (in those cases, the array should remain unchanged).
*/

#include <algorithm>  // for std::swap

// Reverse the subarray arr[start..end] inclusive.
void reverseSubarray(int arr[], int start, int end) {
    while (start < end) {
        std::swap(arr[start], arr[end]);
        ++start;
        --end;
    }
}

// Rotate array to the left by k positions (in place). k may be >= n.
void rotateLeftByK(int arr[], int n, int k) {
    if (n <= 1) return;  // nothing to rotate for empty or single-element arrays
    
    k = k % n;  // effective rotation count
    
    // Left rotation by k = reverse first k, reverse rest, reverse all.
    reverseSubarray(arr, 0, k - 1);
    reverseSubarray(arr, k, n - 1);
    reverseSubarray(arr, 0, n - 1);
}

#include <cassert>

// We assume rotateLeftByK and reverseSubarray are already defined above.
int main() {
    // Basic left rotation
    int arr1[] = {1, 2, 3, 4};
    rotateLeftByK(arr1, 4, 2);
    assert(arr1[0] == 3 && arr1[1] == 4 && arr1[2] == 1 && arr1[3] == 2);

    // k = 0: unchanged
    int arr2[] = {5, 6, 7};
    rotateLeftByK(arr2, 3, 0);
    assert(arr2[0] == 5 && arr2[1] == 6 && arr2[2] == 7);

    // k larger than n
    int arr3[] = {10, 20, 30, 40, 50};
    rotateLeftByK(arr3, 5, 12);  // 12 % 5 = 2 → left by 2
    assert(arr3[0] == 30 && arr3[1] == 40 && arr3[2] == 50 && arr3[3] == 10 && arr3[4] == 20);

    // k multiple of n: unchanged
    int arr4[] = {1, 2, 3};
    rotateLeftByK(arr4, 3, 6);  // 6 % 3 = 0
    assert(arr4[0] == 1 && arr4[1] == 2 && arr4[2] == 3);

    // Single element: never changes
    int arr5[] = {42};
    rotateLeftByK(arr5, 1, 99);
    assert(arr5[0] == 42);

    // Negative numbers and k = n (unchanged)
    int arr6[] = {-1, -2, -3, -4};
    rotateLeftByK(arr6, 4, 4);
    assert(arr6[0] == -1 && arr6[1] == -2 && arr6[2] == -3 && arr6[3] == -4);

    // Left by 1
    int arr7[] = {7, 8, 9};
    rotateLeftByK(arr7, 3, 1);
    assert(arr7[0] == 8 && arr7[1] == 9 && arr7[2] == 7);

    return 0;
}

// The solution uses a three-step reversal technique. First, reduce `k` modulo `n` so that the effective rotation count is in the range `[0, n-1]`. For a left rotation by `k`, we reverse the first `k` elements, then reverse the remaining `n-k` elements, and finally reverse the entire array. This works because reversing the whole array after reversing the two segments brings the two groups into the correct order. For instance, with `[1,2,3,4]` and `k=2`: reverse first 2 → `[2,1,3,4]`, reverse last 2 → `[2,1,4,3]`, reverse all → `[3,4,1,2]`. Edge cases: `k=0` gives no reversals (the algorithm still works because reversing zero elements is a no-op), and `k` a multiple of `n` reduces to `0`. All reversals are in-place with `start` and `end` pointers. Time complexity is `O(n)` because each element is swapped at most twice across the three reversals. Space complexity is `O(1)` auxiliary (only a few integer variables), not counting the input array itself.
