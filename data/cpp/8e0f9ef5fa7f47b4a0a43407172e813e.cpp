Write a standalone C++ function `quickSortArray` that takes a `std::vector<int>` and sorts it in ascending order using the Quick Sort algorithm with the specific partitioning scheme shown in the provided code snippet (last element as pivot, with all elements smaller than the pivot moved to the left, and the pivot placed in its final sorted position). The function must modify the vector in place and return `void`. It must handle empty vectors, vectors with one element, duplicate values, already-sorted, reverse-sorted, and randomly-ordered inputs correctly. The solution must replicate the exact partitioning logic from the snippet, including the use of `swap` and the loop structure, and must be recursive. Edge cases include vectors of size 0 or 1 where no sorting is performed, and the partition step must correctly handle a high index equal to low (though this will not be called from the main quickSort function). The function must not use any standard library sorting functions (like `std::sort`), and must be `const`-correct (i.e., the function itself does not need `const` since it modifies the vector, but the function's parameters should be appropriate).

// The solution is a classic recursive implementation of Quick Sort with the Lomuto partition scheme (as in the snippet). The `partition` function chooses the last element as the pivot, initializes an index `i` to `low - 1`, then iterates `j` from `low` to `high - 1`. Whenever an element `v[j]` is less than the pivot, `i` is incremented and `v[i]` and `v[j]` are swapped. After the loop, the pivot is swapped with `v[i+1]`, placing it in its correct sorted position, and the function returns `i+1`. The `quickSort` function recursively calls itself on the left subarray `[l, pi-1]` and the right subarray `[pi+1, r]` if `l < r`. Edge cases: empty and single-element vectors require no operation; also, if the pivot is the smallest or largest element, one of the recursive calls will have an empty range (e.g., `l > pi-1` or `pi+1 > r`), which is naturally handled by the `l < r` basis case. Time complexity is average-case \(O(n \log n)\) and worst-case \(O(n^2)\) (when the pivot is always the smallest or largest element, e.g., sorted input with this scheme), but the task does not require optimization (like random pivot). Space complexity is \(O(\log n)\) on average for the recursion stack, but \(O(n)\) in the worst case for unbalanced partitions (e.g., sorted input). The implementation must be self-contained and match the given snippet's logic exactly.

#include <vector>

// Partition the subarray [low, high] using the last element as pivot.
// Returns the final index of the pivot after rearrangement.
int partition(std::vector<int>& v, int low, int high) {
    int pivot = v[high];
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (v[j] < pivot) {
            ++i;
            std::swap(v[i], v[j]);
        }
    }
    std::swap(v[i + 1], v[high]);
    return i + 1;
}

// Recursively sorting the subarray [l, r] using Quick Sort.
void quickSortRecursive(std::vector<int>& v, int l, int r) {
    if (l < r) {
        int pi = partition(v, l, r);
        quickSortRecursive(v, l, pi - 1);
        quickSortRecursive(v, pi + 1, r);
    }
}

// Public entry point: sorts the entire vector in ascending order.
void quickSortArray(std::vector<int>& v) {
    if (v.size() <= 1) return;
    quickSortRecursive(v, 0, static_cast<int>(v.size()) - 1);
}

#include <cassert>
#include <vector>
// Include the solution header or paste the above code here.

int main() {
    // Empty vector
    std::vector<int> a;
    quickSortArray(a);
    assert(a.empty());

    // Single element
    std::vector<int> b = {42};
    quickSortArray(b);
    assert(b == std::vector<int>({42}));

    // Already sorted
    std::vector<int> c = {1, 2, 3, 4, 5};
    quickSortArray(c);
    assert(c == std::vector<int>({1, 2, 3, 4, 5}));

    // Reverse sorted
    std::vector<int> d = {9, 7, 5, 3, 1};
    quickSortArray(d);
    assert(d == std::vector<int>({1, 3, 5, 7, 9}));

    // Duplicates
    std::vector<int> e = {4, 2, 4, 2, 1};
    quickSortArray(e);
    assert(e == std::vector<int>({1, 2, 2, 4, 4}));

    // Random order with negatives
    std::vector<int> f = {3, -1, 0, -5, 2, 10};
    quickSortArray(f);
    assert(f == std::vector<int>({-5, -1, 0, 2, 3, 10}));

    // All same values
    std::vector<int> g = {7, 7, 7, 7};
    quickSortArray(g);
    assert(g == std::vector<int>({7, 7, 7, 7}));

    // Two elements out of order
    std::vector<int> h = {2, 1};
    quickSortArray(h);
    assert(h == std::vector<int>({1, 2}));
}
