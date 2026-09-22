Given a sorted rotated array of distinct integers (i.e., a sorted array that has been rotated by some pivot position, such as [3,4,5,6,7,8,1,2] from [1,2,3,4,5,6,7,8]), write a C++ function `int findRotationIndex(const int arr[], int size)` that returns the index of the smallest element in the array (the pivot point). For example, for the array `{3,4,5,6,7,8,1,2}`, the function should return 6, since `arr[6] == 1` is the smallest. The function must work for any valid rotated sorted array with distinct elements, including arrays that are not rotated (where the smallest element is at index 0). Do not modify the input array.
The problem is a classic binary search variation on a rotated sorted array. The key observation is that in a rotated sorted array, the smallest element is the only element that is smaller than its predecessor (or at index 0). To find it efficiently, we compare the middle element with the first element of the array. If `arr[mid] >= arr[0]`, then the middle is in the left (larger) part, so the smallest is to the right, hence we move `low = mid + 1`. Otherwise, `arr[mid] < arr[0]`, meaning the middle is in the right (smaller) part, so the smallest is at or to the left of `mid`, hence we set `high = mid`. The loop continues while `low < high`. At termination, `low` equals `high` and points to the smallest element. Edge cases: if the array is not rotated (e.g., `{1,2,3,4}`), then `arr[mid] >= arr[0]` always holds, so `low` increases until it reaches size-1, but then `arr[mid]` will still be `arr[mid] >= arr[0]`? Actually for a non-rotated sorted array, the smallest is at index 0. The binary search as described will converge to index 0: because initially `arr[mid] >= arr[0]` is true for all mid in the non-rotated array, so `low` moves to `mid+1` repeatedly? Let's test: For `{1,2,3,4}`, size=4. low=0, high=3, mid=1. arr[1]=2 >= arr[0]=1, so low=2. Now low=2, high=3, mid=2, arr[2]=3>=1, low=3. Now low=3, high=3, loop stops, low=3. That returns 3, but the answer should be 0. So the algorithm as written in the original snippet actually returns the number of rotations (the first index where the array starts increasing from the smallest?) Wait, the original snippet returned 6 for `{3,4,5,6,7,8,1,2}`? Let's test: size=8, low=0, high=7, mid=3, arr[3]=6 >= arr[0]=3, low=4. low=4, high=7, mid=5, arr[5]=8 >=3, low=6. low=6, high=7, mid=6, arr[6]=1 < 3, high=6. loop ends (low=6, high=6), returns 6. So it returns 6, which is indeed the index of the smallest element. For non-rotated `{1,2,3,4}`, the algorithm as given returns? low=0,high=3,mid=1, arr[1]=2>=1, low=2; low=2,high=3,mid=2, arr[2]=3>=1, low=3; low=3,high=3, loop ends, returns 3, which is wrong. So the original snippet has a bug for non-rotated arrays. Therefore, the correct algorithm should handle that by a different condition. The common correct approach: use binary search with `low=0, high=size-1`. While `low < high`, mid = (low+high)/2. If `arr[mid] > arr[high]`, then smallest is in right half, so low = mid+1; else high = mid. At end, low is the pivot index. This works for both rotated and non-rotated arrays (for non-rotated, arr[mid] never > arr[high] for a normal sorted array, so high decreases to 0). Let's test: `{1,2,3,4}`: low=0,high=3,mid=1, arr[1]=2 > arr[3]=4? false, so high=1. Now low=0,high=1,mid=0, arr[0]=1 > arr[1]=2? false, high=0. Loop ends, low=0. Correct. For `{3,4,5,6,7,8,1,2}`: low=0,high=7,mid=3, arr[3]=6 > arr[7]=2? true, low=4. low=4,high=7,mid=5, arr[5]=8 > arr[7]=2? true, low=6. low=6,high=7,mid=6, arr[6]=1 > arr[7]=2? false, high=6. Loop ends, low=6. Correct. So the correct algorithm is: find the smallest element by comparing with the last element. Edge cases: size 1 returns 0; all distinct elements; array may be rotated any number of times, including full rotation (back to original). Time complexity O(log n), space O(1).
#include <cstddef>

// Returns the index of the smallest element in a sorted rotated array.
// Assumes the array contains distinct integers and has at least one element.
int findRotationIndex(const int arr[], int size) {
    int low = 0;
    int high = size - 1;

    while (low < high) {
        int mid = low + (high - low) / 2;

        // If middle element is greater than the last element,
        // the pivot lies in the right half.
        if (arr[mid] > arr[high]) {
            low = mid + 1;
        } else {
            // Otherwise, pivot is in the left half (including mid).
            high = mid;
        }
    }

    return low;
}
#include <cassert>

int main() {
    // Standard rotated array
    int arr1[] = {3, 4, 5, 6, 7, 8, 1, 2};
    assert(findRotationIndex(arr1, 8) == 6);

    // Non-rotated sorted array (fully sorted)
    int arr2[] = {1, 2, 3, 4, 5};
    assert(findRotationIndex(arr2, 5) == 0);

    // Rotated by 2 positions
    int arr3[] = {7, 8, 9, 1, 2, 3, 4, 5, 6};
    assert(findRotationIndex(arr3, 9) == 3);

    // Single element
    int arr4[] = {42};
    assert(findRotationIndex(arr4, 1) == 0);

    // Two elements rotated
    int arr5[] = {2, 1};
    assert(findRotationIndex(arr5, 2) == 1);

    // Two elements not rotated
    int arr6[] = {1, 2};
    assert(findRotationIndex(arr6, 2) == 0);

    // Larger rotation (almost fully rotated)
    int arr7[] = {2, 3, 4, 5, 1};
    assert(findRotationIndex(arr7, 5) == 4);

    // Array rotated full circle (back to original)
    int arr8[] = {10, 20, 30, 40};
    assert(findRotationIndex(arr8, 4) == 0);

    // Negative numbers
    int arr9[] = {-5, -3, -1, -7, -6};
    assert(findRotationIndex(arr9, 5) == 3);

    return 0;
}
