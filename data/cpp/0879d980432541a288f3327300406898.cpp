// Write a C++ function that takes a vector of integers and sorts it in ascending order using the quicksort algorithm. The function must modify the vector in place and return void. You may implement the partition step as a helper function or inline it, but the overall structure should follow the classic quicksort divide-and-conquer approach with a pivot chosen as the first element of the current subarray. Assume the input vector is non-empty and contains at least one element. The function should be robust for duplicate values and handle subarrays of size 0 or 1 as base cases. Provide a single function named `quicksortInPlace` that accepts a reference to a `std::vector<int>` (non-const, since it modifies the container).

#include <cassert>
#include <vector>

// Assuming the solution is included above.

int main() {
    std::vector<int> a = {5, 2, 9, 1, 5, 6};
    quicksortInPlace(a);
    assert(a == std::vector<int>({1, 2, 5, 5, 6, 9}));

    std::vector<int> b = {1};
    quicksortInPlace(b);
    assert(b == std::vector<int>({1}));

    std::vector<int> c = {3, 3, 3};
    quicksortInPlace(c);
    assert(c == std::vector<int>({3, 3, 3}));

    std::vector<int> d = {10, -5, 0, 7, -5};
    quicksortInPlace(d);
    assert(d == std::vector<int>({-5, -5, 0, 7, 10}));

    std::vector<int> e = {2, 1};
    quicksortInPlace(e);
    assert(e == std::vector<int>({1, 2}));

    std::vector<int> f = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    quicksortInPlace(f);
    assert(f == std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));

    std::vector<int> g = {7, 2, 7, 2, 7, 2};
    quicksortInPlace(g);
    assert(g == std::vector<int>({2, 2, 2, 7, 7, 7}));

    std::vector<int> h = {100, 100, 99, 98, 100};
    quicksortInPlace(h);
    assert(h == std::vector<int>({98, 99, 100, 100, 100}));
    
    return 0;
}

#include <vector>
#include <algorithm>

// Partition helper: returns the final index of the pivot (chosen as v[low]).
static int partition(std::vector<int>& v, int low, int high) {
    int pivot = v[low];
    int count = 0;
    for (int i = low + 1; i <= high; ++i) {
        if (pivot >= v[i]) {
            ++count;
        }
    }
    int pivotIndex = low + count;
    std::swap(v[low], v[pivotIndex]);

    int i = low;
    int j = high;
    while (i < pivotIndex && j > pivotIndex) {
        while (v[i] <= pivot) {
            ++i;
        }
        while (v[j] > pivot) {
            --j;
        }
        if (i < pivotIndex && j > pivotIndex) {
            std::swap(v[i++], v[j--]);
        }
    }
    return pivotIndex;
}

// Recursive quicksort on the subarray [low, high].
static void quicksortHelper(std::vector<int>& v, int low, int high) {
    if (low >= high) {
        return;
    }
    int p = partition(v, low, high);
    quicksortHelper(v, low, p - 1);
    quicksortHelper(v, p + 1, high);
}

// Public function: sorts the entire vector in ascending order.
void quicksortInPlace(std::vector<int>& v) {
    if (v.size() <= 1) {
        return;
    }
    quicksortHelper(v, 0, static_cast<int>(v.size()) - 1);
}

// The solution implements the standard quicksort algorithm. The core idea is to select the first element of the current subarray as the pivot, then count how many elements in the subarray are less than or equal to the pivot to determine its correct sorted position. After swapping the pivot to that position, we partition the remaining elements so that those less than or equal to the pivot are on the left and those greater are on the right. Then we recursively apply the same process to the left and right subarrays. The base case occurs when the subarray has fewer than two elements (low >= high), which is already sorted. A key edge case is handling duplicate values: the comparison `v[i] <= pivot` when moving the left pointer and `v[j] > pivot` for the right pointer ensures that duplicates are placed consistently on one side, preventing infinite recursion. The choice of pivot as the first element can lead to worst-case O(n²) time on already sorted input, but on average it is O(n log n). The space complexity is O(log n) for the recursion stack in the average case, and O(n) in the worst case due to unbalanced partitions. The partition step itself runs in O(n) time and O(1) auxiliary space, making the overall algorithm in-place except for the recursion stack.
