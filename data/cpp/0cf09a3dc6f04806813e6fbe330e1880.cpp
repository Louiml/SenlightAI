Write a C++ function `long long inversionCount(std::vector<long long>& arr)` that takes a vector of integers (possibly negative, with duplicates) and returns the number of inversions in the array. An inversion is a pair of indices `(i, j)` such that `i < j` and `arr[i] > arr[j]`. The function must not modify the original vector (i.e., it should work on a copy internally) and must handle arrays of any length from 0 upward. Implement the algorithm using a merge-sort-based approach for efficiency. The function should be self-contained, use appropriate `const` correctness, and return the count as a `long long` since the maximum number of inversions for a descending array of size `n` can be up to `n*(n-1)/2`, which exceeds 32-bit integer range for large `n`.
// The solution uses a modified merge sort to count inversions. The key idea is that while merging two sorted halves, if an element from the right half is placed before an element from the left half, then that right-half element forms an inversion with every remaining element in the left half. Specifically, when merging `[left, mid]` and `[mid+1, right]`, if `arr[i] > arr[j]` (where `i` is in the left half and `j` in the right), then all elements from index `i` to `mid` are greater than `arr[j]`, adding `(mid - i + 1)` inversions. The recursive function splits the array, counts inversions in each half, then counts cross inversions during merge. The original array is not modified because we work on a copy passed by value. Edge cases: empty or single-element arrays return 0 inversions; arrays with duplicates count inversions only when `arr[i] > arr[j]` (strict inequality), so equal elements do not count. Time complexity is `O(n log n)` due to merge sort, and space complexity is `O(n)` for the temporary merge array.
#include <vector>

// Count inversions in an array using merge sort. Original array is not modified.
long long inversionCount(std::vector<long long> arr) {
    long long n = (long long)arr.size();
    if (n <= 1) return 0;

    std::vector<long long> temp(n);

    // Recursive helper lambda to count inversions in [left, right]
    // Uses long long to avoid overflow.
    // Lambda captures arr and temp by reference.
    // The function sorts arr (a copy) and returns inversion count.
    // Implemented as a nested recursive function using std::function.
    std::function<long long(long long, long long)> mergeSortCount = [&](long long left, long long right) -> long long {
        if (left >= right) return 0;
        long long mid = left + (right - left) / 2;
        long long inv = 0;
        inv += mergeSortCount(left, mid);
        inv += mergeSortCount(mid + 1, right);

        // Merge two sorted halves and count cross inversions
        long long i = left;
        long long j = mid + 1;
        long long k = left;
        while (i <= mid && j <= right) {
            if (arr[i] <= arr[j]) {
                temp[k++] = arr[i++];
            } else {
                // arr[i] > arr[j], so all elements from i to mid are > arr[j]
                inv += (mid - i + 1);
                temp[k++] = arr[j++];
            }
        }
        while (i <= mid) temp[k++] = arr[i++];
        while (j <= right) temp[k++] = arr[j++];
        for (long long idx = left; idx <= right; ++idx) arr[idx] = temp[idx];

        return inv;
    };

    return mergeSortCount(0, n - 1);
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.

int main() {
    // Empty and single-element arrays
    std::vector<long long> a1 = {};
    assert(inversionCount(a1) == 0);

    std::vector<long long> a2 = {42};
    assert(inversionCount(a2) == 0);

    // Already sorted ascending -> no inversions
    std::vector<long long> a3 = {1, 2, 3, 4, 5};
    assert(inversionCount(a3) == 0);

    // Descending order -> maximum inversions for n=5: 10
    std::vector<long long> a4 = {5, 4, 3, 2, 1};
    assert(inversionCount(a4) == 10);

    // Mixed array with duplicates
    std::vector<long long> a5 = {3, 1, 2, 3, 1};
    // Pairs: (0,1):3>1, (0,2):3>2, (0,4):3>1, (1,4):1>1? no, (2,4):2>1, (3,4):3>1
    // Detailed: index0=3 inversions with 1,2,1 -> 3; index1=1 with none after smaller; index2=2 with 1 -> 1; index3=3 with 1 -> 1; total 5
    assert(inversionCount(a5) == 5);

    // Negative numbers
    std::vector<long long> a6 = {-3, -1, -2, 0};
    // Pairs: (-3,-1)? no, (-3,-2)? no, (-3,0)? no, (-1,-2): yes, (-1,0): no, (-2,0): no -> 1
    assert(inversionCount(a6) == 1);

    // Large worst-case: descending 1000 elements
    std::vector<long long> a7(1000);
    for (int i = 0; i < 1000; ++i) a7[i] = 1000 - i;
    assert(inversionCount(a7) == 1000 * 999 / 2); // 499500

    // Verify original vectors are unchanged (function takes by value, so no modification)
    assert(a5 == std::vector<long long>({3, 1, 2, 3, 1}));
    assert(a7 == std::vector<long long>({1000, 999, 998, /* ... */}));

    return 0;
}
