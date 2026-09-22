Write a C++ function named `isCircularSorted` that takes a constant integer array and its size as parameters, and returns `true` if the array is a sorted array that has been rotated (i.e., a circularly sorted array), and `false` otherwise. A circularly sorted array is one that would be non-decreasing if you started at some rotation point and read the elements in order. For example, `{3, 4, 5, 1, 2}` is circularly sorted (rotate left by 3 positions yields `{1, 2, 3, 4, 5}`), while `{5, 4, 3, 2, 1}` is not. All duplicate values are allowed, and arrays with one element are considered circularly sorted. The function should not modify the input array.
int main() {
    int arr1[] = {3, 4, 5, 1, 2};
    assert(isCircularSorted(arr1, 5) == true);

    int arr2[] = {1, 2, 3, 4, 5};
    assert(isCircularSorted(arr2, 5) == true);

    int arr3[] = {5, 4, 3, 2, 1};
    assert(isCircularSorted(arr3, 5) == false);

    int arr4[] = {7};
    assert(isCircularSorted(arr4, 1) == true);

    int arr5[] = {2, 2, 2, 2};
    assert(isCircularSorted(arr5, 4) == true);

    int arr6[] = {1, 3, 2, 4};
    assert(isCircularSorted(arr6, 4) == false);

    int arr7[] = {4, 1, 2, 3};
    assert(isCircularSorted(arr7, 4) == true);

    int arr8[] = {2, 1, 4, 3};
    assert(isCircularSorted(arr8, 4) == false);

    int arr9[] = {1, 2, 3, 1};
    assert(isCircularSorted(arr9, 4) == true);

    int arr10[] = {3, 1, 2};
    assert(isCircularSorted(arr10, 3) == true);
}
#include <cstddef>

// Returns true if the array is circularly sorted (non-decreasing when rotated).
bool isCircularSorted(const int arr[], std::size_t n) {
    if (n <= 1) return true;

    int descents = 0;
    for (std::size_t i = 1; i < n; ++i) {
        if (arr[i - 1] > arr[i]) {
            ++descents;
        }
    }
    if (arr[n - 1] > arr[0]) {
        ++descents;
    }
    return descents <= 1;
}
// The solution observes that in a non-rotated non-decreasing array, the number of "descents" or places where an element is strictly greater than the next element is zero. If the array is rotated, there will be exactly one descent (at the rotation boundary where the largest element is followed by the smallest element). Additionally, the wrap-around must be checked: for the array to be circularly sorted, the last element must not be greater than the first element (except possibly when the rotation point is at index 0 or when all elements are equal). The algorithm counts the number of positions i (including the wrap from n-1 to 0) where `arr[i-1] > arr[i]` (modular arithmetic). If this count is less than or equal to 1, the array is circularly sorted. Considering the example `{3,4,5,1,2}`: descents occur at `arr[2]=5 > arr[3]=1` (one count), and wrap: `arr[4]=2 > arr[0]=3` is false, so count=1 → true. For `{5,4,3}`: descents at i=1 and i=2 (two counts), plus wrap `arr[2]=3 > arr[0]=5` false, count=2 → false. Edge cases: arrays with all equal elements have count=0, which is ≤1 → true; array with one element has no loop iterations and wrap counts 0 → true. Time complexity is O(n) with O(1) auxiliary space.
