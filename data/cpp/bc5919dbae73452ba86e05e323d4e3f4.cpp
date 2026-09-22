/*
Write a C++ function named `kthSmallestInMatrix` that accepts a square matrix of integers (represented as `std::vector<std::vector<int>>`) and a positive integer `k`. The matrix is sorted in non-decreasing order both row-wise and column-wise (i.e., each row is sorted from left to right, and each column is sorted from top to bottom). The function should return the `k`-th smallest element (1-indexed) from the entire matrix. For example, for the matrix `[[1,5,9],[10,11,13],[12,13,15]]` and `k = 8`, the function should return `13`. Assume `k` is always between 1 and `n*n` inclusive, where `n` is the number of rows (which equals the number of columns). The function must handle the case of duplicate values correctly (duplicates are counted as separate occurrences), and must not modify the input matrix. The implementation should work efficiently for large matrices (e.g., up to 300×300) and large `k`.
*/
#include <vector>
#include <queue>

// Return the k-th smallest element (1-indexed) from a row- and column-sorted square matrix.
int kthSmallestInMatrix(const std::vector<std::vector<int>>& matrix, int k) {
    // Max-heap to keep the k smallest elements seen so far.
    std::priority_queue<int> maxHeap;
    
    for (const auto& row : matrix) {
        for (const auto& val : row) {
            maxHeap.push(val);
            // Keep heap size limited to k; pop the largest when exceeding.
            if (maxHeap.size() > static_cast<size_t>(k)) {
                maxHeap.pop();
            }
        }
    }
    
    return maxHeap.top();
}
#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.
int main() {
    // Basic example from the problem statement.
    std::vector<std::vector<int>> m1 = {{1,5,9},{10,11,13},{12,13,15}};
    assert(kthSmallestInMatrix(m1, 1) == 1);
    assert(kthSmallestInMatrix(m1, 2) == 5);
    assert(kthSmallestInMatrix(m1, 5) == 11);
    assert(kthSmallestInMatrix(m1, 8) == 13);
    assert(kthSmallestInMatrix(m1, 9) == 15);

    // Single-element matrix.
    std::vector<std::vector<int>> m2 = {{42}};
    assert(kthSmallestInMatrix(m2, 1) == 42);

    // All duplicates.
    std::vector<std::vector<int>> m3 = {{2,2},{2,2}};
    assert(kthSmallestInMatrix(m3, 1) == 2);
    assert(kthSmallestInMatrix(m3, 2) == 2);
    assert(kthSmallestInMatrix(m3, 3) == 2);
    assert(kthSmallestInMatrix(m3, 4) == 2);

    // Larger sorted matrix with mixed values.
    std::vector<std::vector<int>> m4 = {{1,3,5},{2,4,6},{3,5,7}};
    assert(kthSmallestInMatrix(m4, 1) == 1);
    assert(kthSmallestInMatrix(m4, 3) == 3);
    assert(kthSmallestInMatrix(m4, 6) == 5);
    assert(kthSmallestInMatrix(m4, 9) == 7);

    // k = n*n returns the maximum element.
    std::vector<std::vector<int>> m5 = {{-5,-1},{0,2}};
    assert(kthSmallestInMatrix(m5, 1) == -5);
    assert(kthSmallestInMatrix(m5, 4) == 2);

    return 0;
}
// The simplest and most straightforward approach is to push all elements of the matrix into a max-heap (priority queue), but limit the heap size to `k`. While iterating through every element of the matrix, each element is pushed into the heap. If the heap size exceeds `k`, we pop the largest element, ensuring the heap always contains the `k` smallest elements seen so far. After processing all elements, the top of the max-heap is the `k`-th smallest element (since the heap contains exactly the `k` smallest elements, and the largest among those is the `k`-th smallest overall). This works correctly regardless of the matrix's row/column sort order because we consider every element. Edge cases: `k = 1` returns the global minimum; `k = n*n` returns the global maximum; duplicates are counted individually because each is pushed separately. Time complexity is `O(n² log k)` because each push/pop on a heap of size at most `k` takes `O(log k)`, and we process `n²` elements. Space complexity is `O(k)` for the heap. This solution does not exploit the sorted property of rows/columns, but it is simple, robust, and acceptable for the given constraints (for n=300, n²=90,000 and log k is small). Note: To avoid unnecessary allocation, we iterate using range-based for loops with `const auto&`.
