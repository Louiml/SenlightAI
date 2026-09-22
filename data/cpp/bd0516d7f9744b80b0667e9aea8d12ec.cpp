Given an array of `n` distinct integers (1-indexed for convenience), write a C++ function `int minAdjacentSwapsToSort(const std::vector<int>& arr)` that returns the minimum number of adjacent swaps needed to sort the array in non-decreasing order. The function must compute this count by determining, for each original element, how many elements that are greater than it and appear before it in the original order (i.e., its inversion count contribution), then sum those contributions. The result will be the total number of inversions in the array. The input array is guaranteed to contain distinct integers and `n >= 1`. Your function should not modify the input array and should work efficiently for `n` up to 10^5.

#include <cassert>
#include <vector>
#include <cstdint>

// The function declaration is assumed to be available from the solution above.
int64_t minAdjacentSwapsToSort(const std::vector<int>& arr);

int main() {
    // Single element
    assert(minAdjacentSwapsToSort({5}) == 0);
    // Already sorted
    assert(minAdjacentSwapsToSort({1, 2, 3, 4}) == 0);
    // Reverse sorted
    assert(minAdjacentSwapsToSort({4, 3, 2, 1}) == 6); // 4*3/2
    // Mixed case
    assert(minAdjacentSwapsToSort({3, 1, 2}) == 2); // inversions: (3,1), (3,2)
    assert(minAdjacentSwapsToSort({2, 3, 1}) == 2); // inversions: (2,1), (3,1)
    // Larger random-like
    assert(minAdjacentSwapsToSort({10, 7, 8, 9}) == 3); // (10,7),(10,8),(10,9)
    // Duplicate-free distinct but not permutation
    assert(minAdjacentSwapsToSort({100, 50, 200, 1}) == 4); // all pairs with 100 and 50 vs 1, plus 200 vs 1: (100,50),(100,1),(50,1),(200,1)
    // All same value not allowed because distinct, but test with 2 elements
    assert(minAdjacentSwapsToSort({1, 0}) == 1);
    assert(minAdjacentSwapsToSort({0, 1}) == 0);
    return 0;
}

#include <vector>
#include <cstdint>

// Returns the minimum number of adjacent swaps needed to sort the input array.
// Equivalent to the number of inversions in the array (since all elements are distinct).
int64_t minAdjacentSwapsToSort(const std::vector<int>& arr) {
    std::vector<int> temp(arr.size());
    std::vector<int> copy = arr;
    
    // Recursive merge sort that counts inversions.
    // Returns inversion count for the range [left, right) in `copy`.
    // Uses `temp` as auxiliary storage.
    auto mergeSort = [&](auto&& self, int left, int right) -> int64_t {
        if (right - left <= 1) {
            return 0;
        }
        int mid = left + (right - left) / 2;
        int64_t inv = self(self, left, mid) + self(self, mid, right);
        
        // Merge the two sorted halves and count cross inversions.
        int i = left;
        int j = mid;
        int k = left;
        while (i < mid && j < right) {
            if (copy[i] <= copy[j]) {
                temp[k++] = copy[i++];
            } else {
                temp[k++] = copy[j++];
                inv += (mid - i);  // All remaining elements in left half are greater.
            }
        }
        while (i < mid) temp[k++] = copy[i++];
        while (j < right) temp[k++] = copy[j++];
        
        // Copy merged result back to `copy`.
        for (int p = left; p < right; ++p) {
            copy[p] = temp[p];
        }
        return inv;
    };
    
    return mergeSort(mergeSort, 0, static_cast<int>(arr.size()));
}

// The minimum number of adjacent swaps to sort an array equals the number of inversions in it. An inversion is a pair of indices `(i, j)` such that `i < j` but `arr[i] > arr[j]`. Each adjacent swap can fix at most one inversion, so the total inversions is both necessary and sufficient. Since the problem here relates to the provided snippet that uses sorting and lower_bound to count parity, we reinterpret it: the snippet computes `k` = number of indices where the position of the element in the sorted order (its rank) differs from its original index by an odd amount, then outputs `k/2`. That approach works only when the array is a permutation of 1..n. But for a general array of distinct integers, the correct measure is straightforward inversion counting. We can compute inversions efficiently using a merge sort or a Fenwick tree. However, the task asks for a function that returns the exact minimum swaps; for distinct integers, that is the inversion count. We can implement a merge sort-based inversion counter with `O(n log n)` time and `O(n)` auxiliary space. Edge cases: single element returns 0; already sorted array returns 0; reverse-sorted array returns `n*(n-1)/2`. The solution uses a copy of the array to avoid modifying the input.
