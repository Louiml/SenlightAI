Given a sequence of \(n\) integers (\(1 \le n \le 10^5\)), write a C++ function `long long minSwapsToSort(const std::vector<int>& arr)` that returns the minimum number of adjacent swaps needed to sort the array in non-decreasing order. However, the only allowed operation is swapping two adjacent elements, and you may only swap an element with its immediate neighbor. You are not allowed to use any sorting algorithm directly; instead, you must compute the minimum number of swaps by analyzing the array’s order. The function should return the exact minimum number of adjacent swaps required. For example, for `arr = {3, 2, 1}`, the answer is 3 (swap 3 with 2 → {2, 3, 1}, swap 3 with 1 → {2, 1, 3}, swap 2 with 1 → {1, 2, 3}). Edge cases include arrays already sorted (answer 0), arrays sorted in descending order (answer \(n(n-1)/2\) for distinct elements), and arrays with duplicate values (duplicates do not require swaps to become equal, and the count must reflect that). The input array may contain any integers, including negative and large values, but the answer fits in a 64-bit signed integer.

The minimum number of adjacent swaps needed to sort an array is equal to the number of inversions in the array, where an inversion is a pair of indices `(i, j)` with `i < j` and `arr[i] > arr[j]`. This is because each adjacent swap of two out-of-order elements reduces the inversion count by exactly 1, and sorting requires eliminating all inversions. We can count inversions efficiently using a merge sort algorithm. During the merge step, whenever we take an element from the right half, it forms an inversion with every remaining element in the left half, so we add the size of the remaining left half to the count. Duplicate values are handled because only strict greater-than counts as an inversion (`arr[i] > arr[j]`, not `>=`). The algorithm runs in O(n log n) time and uses O(n) auxiliary space for the temporary merge array. Edge cases: already sorted array gives 0; descending distinct array gives n(n-1)/2; single element gives 0; duplicate values do not create inversions between equal elements.

#include <vector>

// Count inversions using merge sort.
long long merge_sort_count(std::vector<int>& arr, int left, int right) {
    if (right - left <= 1) return 0;
    int mid = left + (right - left) / 2;
    long long inv = merge_sort_count(arr, left, mid);
    inv += merge_sort_count(arr, mid, right);

    // Merge the two sorted halves.
    std::vector<int> temp(right - left);
    int i = left, j = mid, k = 0;
    while (i < mid && j < right) {
        if (arr[i] <= arr[j]) {
            temp[k++] = arr[i++];
        } else {
            temp[k++] = arr[j++];
            inv += (mid - i); // all remaining left elements are larger
        }
    }
    while (i < mid) temp[k++] = arr[i++];
    while (j < right) temp[k++] = arr[j++];
    for (int p = 0; p < (int)temp.size(); ++p) {
        arr[left + p] = temp[p];
    }
    return inv;
}

// Public function: returns minimum adjacent swaps to sort in non-decreasing order.
long long min_adjacent_swaps_to_sort(const std::vector<int>& arr) {
    std::vector<int> copy = arr; // work on a copy to keep input const
    return merge_sort_count(copy, 0, static_cast<int>(copy.size()));
}

#include <cassert>
#include <vector>

// (The above solution code is assumed to be included)
int main() {
    assert(min_adjacent_swaps_to_sort({3, 2, 1}) == 3);
    assert(min_adjacent_swaps_to_sort({1, 2, 3}) == 0);
    assert(min_adjacent_swaps_to_sort({2, 2, 1}) == 2);
    assert(min_adjacent_swaps_to_sort({1}) == 0);
    assert(min_adjacent_swaps_to_sort({5, 4, 3, 2, 1}) == 10);
    assert(min_adjacent_swaps_to_sort({1, 3, 2, 4}) == 1);
    assert(min_adjacent_swaps_to_sort({2, 1, 3, 1}) == 3);
    assert(min_adjacent_swaps_to_sort({-1, -2, -3}) == 3);
    assert(min_adjacent_swaps_to_sort({10, 10, 10}) == 0);
    assert(min_adjacent_swaps_to_sort({3, 1, 2}) == 2);
}
