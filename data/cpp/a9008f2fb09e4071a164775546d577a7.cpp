// Write a C++ function `nearlySortedArray` that takes an array of integers, its size `n`, and a positive integer `k`, and returns a vector containing the elements of the array in sorted order. The array is guaranteed to be "nearly sorted," meaning each element is at most `k` positions away from its correct sorted position (where `k` is smaller than `n`). The function must use a min-heap (priority queue) approach to exploit this property, achieving better performance than a full sort. The input array may contain duplicate values, and `k` can be as small as 1 or as large as `n-1`. The returned vector must contain all elements in non-decreasing order.
The key observation is that because each element is at most `k` positions from its sorted position, when we process elements from left to right, the smallest available element among the first `k+1` elements must be the correct next element in the sorted output. The algorithm uses a min-heap to maintain a window of the current `k+1` elements. For each index `i` from 0 to `n-1`, we push `arr[i]` into the heap. If the heap size exceeds `k` (i.e., we have more than `k+1` elements), then the heap's top is guaranteed to be the next smallest element, so we pop it and place it in the output vector. After processing all elements, we drain the remaining heap elements into the output. This works because the window never needs to hold more than `k+1` elements at a time, and each element enters and exits the heap exactly once. Edge cases include `k` being 0 (then each element is already in place, and the heap is never allowed to exceed size 1), duplicates (handled naturally by the heap), and the final draining when the heap size is at most `k+1`. Time complexity is O(n log k) because each push and pop is O(log k), and there are n pushes and n pops total. Space complexity is O(k) for the heap (plus O(n) for the output vector, which is necessary for the return value).
#include <vector>
#include <queue>

// Return a sorted vector of the nearly sorted input array.
// Each element is at most k positions from its sorted position.
std::vector<int> nearlySortedArray(const int arr[], int n, int k) {
    // Min-heap to store the current window of up to k+1 elements.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    std::vector<int> sorted;
    sorted.reserve(n);

    for (int i = 0; i < n; ++i) {
        minHeap.push(arr[i]);
        // If the window size exceeds k, the smallest in the window
        // is guaranteed to be the next element in sorted order.
        if (static_cast<int>(minHeap.size()) > k) {
            sorted.push_back(minHeap.top());
            minHeap.pop();
        }
    }

    // Drain the remaining elements in the heap.
    while (!minHeap.empty()) {
        sorted.push_back(minHeap.top());
        minHeap.pop();
    }

    return sorted;
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared in an included header or above.
// Here we just link to the provided free function.
// (For completeness, the solution code is assumed to be available.)

int main() {
    // Test case 1: Basic nearly sorted array with k=3.
    int arr1[] = {6, 5, 3, 2, 8, 10, 9};
    std::vector<int> res1 = nearlySortedArray(arr1, 7, 3);
    std::vector<int> expected1 = {2, 3, 5, 6, 8, 9, 10};
    assert(res1 == expected1);

    // Test case 2: Already sorted array with k=1.
    int arr2[] = {1, 2, 3, 4, 5};
    std::vector<int> res2 = nearlySortedArray(arr2, 5, 1);
    std::vector<int> expected2 = {1, 2, 3, 4, 5};
    assert(res2 == expected2);

    // Test case 3: Reverse sorted with k=4 (since n-1=4).
    int arr3[] = {5, 4, 3, 2, 1};
    std::vector<int> res3 = nearlySortedArray(arr3, 5, 4);
    std::vector<int> expected3 = {1, 2, 3, 4, 5};
    assert(res3 == expected3);

    // Test case 4: Duplicate elements with k=2.
    int arr4[] = {3, 3, 1, 2, 3};
    std::vector<int> res4 = nearlySortedArray(arr4, 5, 2);
    std::vector<int> expected4 = {1, 2, 3, 3, 3};
    assert(res4 == expected4);

    // Test case 5: Single element, k=1 (though k should be < n, but k=1 is valid for n=1? Actually k< n, so k=0 is valid, but here use n=1,k=0).
    // For n=1, k can be 0.
    int arr5[] = {42};
    std::vector<int> res5 = nearlySortedArray(arr5, 1, 0);
    std::vector<int> expected5 = {42};
    assert(res5 == expected5);

    // Test case 6: k=0 means already sorted.
    int arr6[] = {10, 20, 30};
    std::vector<int> res6 = nearlySortedArray(arr6, 3, 0);
    std::vector<int> expected6 = {10, 20, 30};
    assert(res6 == expected6);

    // Test case 7: Larger k, but still less than n.
    int arr7[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> res7 = nearlySortedArray(arr7, 9, 8);
    std::vector<int> expected7 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(res7 == expected7);

    // Test case 8: Negative numbers and duplicates.
    int arr8[] = {-5, -2, -5, 0, -1};
    std::vector<int> res8 = nearlySortedArray(arr8, 5, 2);
    std::vector<int> expected8 = {-5, -5, -2, -1, 0};
    assert(res8 == expected8);

    std::cout << "All tests passed." << std::endl;
    return 0;
}
