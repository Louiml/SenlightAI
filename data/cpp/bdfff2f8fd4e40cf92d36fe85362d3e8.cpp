// Write a C++ function that takes a vector of integers and an integer `k` and returns the `k`-th smallest element in the vector. The vector may contain duplicate values, and `k` is 1-indexed (so `k=1` returns the minimum). If `k` is out of range (less than 1 or greater than the number of elements), return a sentinel value of `-1`. Your function must not modify the input vector, must handle empty input, and should avoid full sorting if possible by using a partial selection algorithm for efficiency.
The task requires finding the `k`-th smallest element without modifying the input. Since the vector may be large, using `std::nth_element` on a copy would modify the copy, but that is acceptable because we are not modifying the original — however, copying the entire vector is O(n) extra space, which is fine. Alternatively, we can use a max-heap of size `k` to hold the smallest `k` elements; the top of that heap will be the `k`-th smallest. This is O(n log k) time and O(k) space, which is efficient when k is small. Edge cases: empty vector → return -1; k < 1 or k > vector size → return -1; duplicates are handled naturally since we count all occurrences. If k equals the size, the result is the maximum element. Time complexity: O(n log k) for the heap approach, or O(n) average with `std::nth_element` on a copy. Space complexity: O(k) for the heap, or O(n) for the copy. The heap approach is more space-efficient and does not require copying the whole input. We will use the heap approach for clarity and const-correctness.
#include <vector>
#include <queue>
#include <cstddef>

// Returns the k-th smallest element (1-indexed) from the input vector without modifying it.
// Returns -1 if k is out of range or the vector is empty.
int kthSmallest(const std::vector<int>& numbers, int k) {
    if (numbers.empty() || k < 1 || static_cast<size_t>(k) > numbers.size()) {
        return -1;
    }

    // Max-heap to keep the smallest k elements seen so far.
    std::priority_queue<int> maxHeap;
    for (int value : numbers) {
        if (maxHeap.size() < static_cast<size_t>(k)) {
            maxHeap.push(value);
        } else if (value < maxHeap.top()) {
            maxHeap.pop();
            maxHeap.push(value);
        }
    }

    // The top of the heap is the k-th smallest.
    return maxHeap.top();
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    std::vector<int> v1 = {3, 1, 2};
    assert(kthSmallest(v1, 1) == 1);
    assert(kthSmallest(v1, 2) == 2);
    assert(kthSmallest(v1, 3) == 3);

    // Duplicates
    std::vector<int> v2 = {5, 5, 5, 2};
    assert(kthSmallest(v2, 1) == 2);
    assert(kthSmallest(v2, 2) == 5);
    assert(kthSmallest(v2, 3) == 5);
    assert(kthSmallest(v2, 4) == 5);

    // Edge cases
    std::vector<int> empty;
    assert(kthSmallest(empty, 1) == -1);
    assert(kthSmallest(v1, 0) == -1);
    assert(kthSmallest(v1, 4) == -1);

    // Negative numbers
    std::vector<int> v3 = {-10, -1, -5};
    assert(kthSmallest(v3, 1) == -10);
    assert(kthSmallest(v3, 3) == -1);

    // Large k equals size
    std::vector<int> v4 = {42};
    assert(kthSmallest(v4, 1) == 42);

    return 0;
}
