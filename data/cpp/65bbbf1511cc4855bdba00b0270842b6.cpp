Write a C++ function named `medianOfSorted` that takes a non-empty vector of integers (which may contain duplicates) as input and returns the median value as a double. The function should not use the standard library's `std::nth_element`, `std::sort`, or any other sorting utility directly; instead, it must implement the heap-based algorithm derived from heap sort: first build a max-heap, then extract elements one by one until reaching the middle position(s), computing the median accordingly. If the vector size is odd, return the middle element; if even, return the average of the two middle elements. The function must preserve the original vector (i.e., work on a copy) and return the correct double median.

The solution adapts the heap sort algorithm to find the median without fully sorting the array. We first create a copy of the input vector to avoid mutating the original. Then build a max-heap using the `heapify` procedure (sift-down) so the largest element is at index 0. To find the median, we need the element(s) at position `mid = n/2` (0-based). For an odd `n`, the median is the element at index `n/2` after extracting `n/2` largest elements (each extraction moves the current maximum to the end of the heap region). For even `n`, we need the two middle elements: after extracting `n/2 - 1` largest elements, the next root is the lower middle, and then after one more extraction (which moves that root to the end), the new root is the upper middle. However, a simpler approach: repeatedly swap the root with the last element of the heap portion and reduce the heap size by 1, exactly like heap sort, but stop early once the needed indices are fixed. For odd `n`, after `k = n/2` extractions, the element now at index `k` (which is the last element of the reduced heap region) is the median. For even `n`, after `k = n/2 - 1` extractions, the element at index `k` is the lower middle, and after one more extraction, the element at index `k+1` is the upper middle. Edge cases: `n=1` returns that element; duplicates are handled naturally. Time complexity: building the heap is `O(n)`, and each extraction calls `heapify` which is `O(log n)`, so total `O(n log n)` in the worst case (though average could be better, it's still `O(n log n)`). Space complexity is `O(n)` due to the copy. No additional data structures beyond the vector and a few variables are used.

#include <vector>
#include <algorithm>

// Helper to heapify a subtree rooted at index i in a max-heap of size n.
void maxHeapify(std::vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        maxHeapify(arr, n, largest);
    }
}

// Build a max-heap from a vector.
void buildMaxHeap(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        maxHeapify(arr, n, i);
    }
}

// Compute the median of a non-empty vector using heap-based partial extraction.
double medianOfSorted(const std::vector<int>& input) {
    std::vector<int> arr = input; // work on a copy
    int n = arr.size();
    buildMaxHeap(arr);

    if (n % 2 == 1) {
        // Need the element that would end up at index n/2 after sorting.
        int target = n / 2;
        // Extract the largest (n-1 - target) elements, i.e., (n/2) extractions.
        for (int i = n - 1; i > target; --i) {
            std::swap(arr[0], arr[i]);
            maxHeapify(arr, i, 0);
        }
        // After loop, arr[target] contains the median (because we performed n/2 extractions).
        return static_cast<double>(arr[target]);
    } else {
        // Even length: need indices n/2 - 1 and n/2.
        int lowerTarget = n / 2 - 1;
        // Extract elements so that the lower target becomes the root of the reduced heap.
        for (int i = n - 1; i > lowerTarget + 1; --i) {
            std::swap(arr[0], arr[i]);
            maxHeapify(arr, i, 0);
        }
        // Now perform one more extraction: the current root is the lower middle.
        double lower = arr[0];
        std::swap(arr[0], arr[lowerTarget + 1]);
        maxHeapify(arr, lowerTarget + 1, 0);
        // Now the root of the reduced heap is the upper middle.
        double upper = arr[0];
        return (lower + upper) / 2.0;
    }
}

#include <cassert>
#include <vector>
#include <cmath>

// Declaration of the function to test (provided in the solution).
double medianOfSorted(const std::vector<int>& input);

int main() {
    // Odd length
    assert(medianOfSorted({5}) == 5.0);
    assert(medianOfSorted({3, 1, 2}) == 2.0);
    assert(medianOfSorted({1, 2, 3, 4, 5}) == 3.0);
    assert(medianOfSorted({10, 20, 30, 40, 50}) == 30.0);
    // Even length
    assert(medianOfSorted({1, 2}) == 1.5);
    assert(medianOfSorted({4, 1, 3, 2}) == 2.5);
    assert(medianOfSorted({5, 6, 1, 2, 3, 4}) == 3.5);
    // Duplicates
    assert(medianOfSorted({2, 2, 2}) == 2.0);
    assert(medianOfSorted({1, 1, 1, 1}) == 1.0);
    // Negative numbers
    assert(medianOfSorted({-5, -1, -3}) == -3.0);
    assert(medianOfSorted({-2, -4}) == -3.0);
    // Check original vector unchanged
    std::vector<int> original = {9, 8, 7, 6};
    double med = medianOfSorted(original);
    assert(med == 7.5);
    assert(original.size() == 4 && original[0] == 9 && original[1] == 8 && original[2] == 7 && original[3] == 6);
    // Large random test to verify correctness against a brute-force sort
    std::vector<int> v(1000);
    for (int i = 0; i < 1000; ++i) {
        v[i] = (i * 17) % 100 - 50;
    }
    std::vector<int> sorted = v;
    std::sort(sorted.begin(), sorted.end());
    double expected;
    if (sorted.size() % 2 == 1) {
        expected = sorted[sorted.size() / 2];
    } else {
        expected = (sorted[sorted.size() / 2 - 1] + sorted[sorted.size() / 2]) / 2.0;
    }
    double actual = medianOfSorted(v);
    assert(std::abs(actual - expected) < 1e-9);

    return 0;
}
