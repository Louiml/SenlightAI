Write a C++ function named `findKthLargestElement` that takes a vector of integers `nums` and a positive integer `k` (1 ≤ k ≤ nums.size()) and returns the k-th largest distinct value present in the array. The function must solve the problem efficiently using a min-heap (priority queue) of size k, and must not modify the original input vector. The input vector may contain duplicate values, negative numbers, and the order of elements is arbitrary; the function should treat duplicates as separate entries for ranking (e.g., for `[3,2,1,5,6,4]` and `k=2`, the answer is `5`; for `[3,2,3,1,2,4,5,5,6]` and `k=4`, the answer is `4`).

#include <cassert>
#include <vector>

// (The solution function is assumed to be included above.)

int main() {
    std::vector<int> nums1 = {3, 2, 1, 5, 6, 4};
    assert(findKthLargestElement(nums1, 2) == 5);
    assert(findKthLargestElement(nums1, 1) == 6);
    assert(findKthLargestElement(nums1, 6) == 1);

    std::vector<int> nums2 = {3, 2, 3, 1, 2, 4, 5, 5, 6};
    assert(findKthLargestElement(nums2, 4) == 4);
    assert(findKthLargestElement(nums2, 1) == 6);
    assert(findKthLargestElement(nums2, 9) == 1);

    std::vector<int> nums3 = {-1, -5, -3, -2};
    assert(findKthLargestElement(nums3, 1) == -1);
    assert(findKthLargestElement(nums3, 4) == -5);

    std::vector<int> nums4 = {7, 7, 7, 7};
    assert(findKthLargestElement(nums4, 2) == 7);
    assert(findKthLargestElement(nums4, 4) == 7);

    std::vector<int> nums5 = {100};
    assert(findKthLargestElement(nums5, 1) == 100);

    return 0;
}

#include <queue>
#include <vector>

// Returns the k-th largest element in the vector nums.
// Uses a min-heap of size k to keep track of the k largest elements.
int findKthLargestElement(const std::vector<int>& nums, int k) {
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    // Insert the first k elements into the heap.
    for (int i = 0; i < k; ++i) {
        minHeap.push(nums[i]);
    }

    // For each remaining element, if it is larger than the smallest in the heap,
    // replace that smallest element with the current one.
    for (int i = k; i < static_cast<int>(nums.size()); ++i) {
        if (nums[i] > minHeap.top()) {
            minHeap.pop();
            minHeap.push(nums[i]);
        }
    }

    // The root of the min-heap is the k-th largest element.
    return minHeap.top();
}

// The core idea is to maintain a min-heap that contains exactly the k largest elements seen so far in the array. First, push the first k elements into the heap. For each remaining element, compare it with the heap's root (which is the smallest among the current k largest). If the new element is larger than the root, remove the root and insert the new element. After processing all elements, the heap contains the k largest elements of the entire array, and its root is the k-th largest. This works because the min-heap always discards the smallest candidate among the current k largest, ensuring that the heap only grows with larger replacements. Edge cases include arrays with duplicates (they are handled naturally because duplicates may or may not be replaced depending on ordering), k = 1 (returns the maximum), k = nums.size() (returns the minimum), and negative numbers (comparisons work as usual). Time complexity is O(n log k) because each of the n elements may cause a heap operation (push/pop) costing O(log k) in the worst case. Space complexity is O(k) for the heap, and O(1) additional space beyond the input; the function does not modify the input vector.
