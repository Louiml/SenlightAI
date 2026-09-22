// Write a standalone C++ function named `mergeSortedInPlace` that takes two sorted integer vectors `A` and `B`, along with the valid element counts `m` (for `A`) and `n` (for `B`), and merges `B` into `A` so that `A` contains all `m+n` elements in non-decreasing order. The vector `A` is guaranteed to have a size equal to `m+n` (with the extra trailing slots conceptually pre‑allocated but contain unspecified values), and `B` has size `n`. The function must modify `A` in place and return void. You may assume `m` and `n` are non-negative and that the first `m` elements of `A` and all `n` elements of `B` are sorted individually. The goal is to solve the problem without using any extra array beyond the input itself, and with a linear‑time, constant‑extra‑space algorithm.

// The standard approach is to fill `A` from the back (largest indices downward), because the tail of `A` is “empty” from the perspective of the valid prefix. Maintain two indices: `i = m-1` (last valid element in `A`) and `j = n-1` (last element in `B`). For each position `k` from `A.size()-1` down to `0`, compare the current candidates `A[i]` and `B[j]`. If `j` has become negative, or if `i` is still non‑negative and `A[i] >= B[j]`, write `A[i]` to `A[k]` and decrement `i`; otherwise write `B[j]` and decrement `j`. This works because we always take the larger of the two remaining elements, placing it at the highest available slot, preserving sorted order. Edge cases: when `m=0`, the loop copies all of `B`; when `n=0`, the loop copies the existing `A` elements back into the same positions; when duplicates exist, the condition `>=` ensures stability if desired (though not required). Time complexity is O(m+n) because each element is moved exactly once. Space complexity is O(1) beyond the input vectors.

#include <vector>

// Merge two sorted vectors in place.
// A has size m+n; the first m elements are sorted, the rest are unused.
// B has size n and is sorted.
// After the call, A[0..m+n-1] contains the merged sorted sequence.
void mergeSortedInPlace(std::vector<int>& A, int m, const std::vector<int>& B, int n) {
    int i = m - 1;          // last valid element in A
    int j = n - 1;          // last element in B
    int k = static_cast<int>(A.size()) - 1; // write position from the back

    while (k >= 0) {
        if (j < 0 || (i >= 0 && A[i] >= B[j])) {
            A[k--] = A[i--];
        } else {
            A[k--] = B[j--];
        }
    }
}

#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.

int main() {
    // Basic merge
    {
        std::vector<int> A = {1, 3, 5, 0, 0, 0};
        std::vector<int> B = {2, 4, 6};
        mergeSortedInPlace(A, 3, B, 3);
        assert((A == std::vector<int>{1, 2, 3, 4, 5, 6}));
    }

    // One vector empty (B empty)
    {
        std::vector<int> A = {2, 4, 6};
        std::vector<int> B;
        mergeSortedInPlace(A, 3, B, 0);
        assert((A == std::vector<int>{2, 4, 6}));
    }

    // One vector empty (A empty)
    {
        std::vector<int> A = {0, 0, 0};
        std::vector<int> B = {1, 5, 9};
        mergeSortedInPlace(A, 0, B, 3);
        assert((A == std::vector<int>{1, 5, 9}));
    }

    // Duplicates
    {
        std::vector<int> A = {1, 1, 2, 0, 0};
        std::vector<int> B = {1, 2};
        mergeSortedInPlace(A, 3, B, 2);
        assert((A == std::vector<int>{1, 1, 1, 2, 2}));
    }

    // All elements from B are smaller than A
    {
        std::vector<int> A = {5, 6, 7, 0, 0};
        std::vector<int> B = {1, 2};
        mergeSortedInPlace(A, 3, B, 2);
        assert((A == std::vector<int>{1, 2, 5, 6, 7}));
    }

    // All elements from B are larger than A
    {
        std::vector<int> A = {1, 2, 0, 0, 0};
        std::vector<int> B = {8, 9, 10};
        mergeSortedInPlace(A, 2, B, 3);
        assert((A == std::vector<int>{1, 2, 8, 9, 10}));
    }

    // Negative numbers
    {
        std::vector<int> A = {-5, -1, 0, 0, 0};
        std::vector<int> B = {-3, 2, 4};
        mergeSortedInPlace(A, 2, B, 3);
        assert((A == std::vector<int>{-5, -3, -1, 2, 4}));
    }

    // Single element each
    {
        std::vector<int> A = {3, 0};
        std::vector<int> B = {1};
        mergeSortedInPlace(A, 1, B, 1);
        assert((A == std::vector<int>{1, 3}));
    }

    return 0;
}
