Write a C++ function `kLargest` that takes a vector of integers and a non-negative integer `k`, and returns a vector containing the k largest elements from the input in descending order (largest first). If `k` is 0 or the vector is empty, return an empty vector. If `k` is greater than the number of distinct or total elements, return all elements sorted in descending order. Duplicate values are allowed and counted individually. The solution must use a priority queue (max-heap or min-heap) to demonstrate its use, not simply sort the entire vector.

#include <cassert>
#include <vector>

// The function kLargest is assumed to be defined above.
int main() {
    // Basic case.
    std::vector<int> a = {3, 1, 4, 1, 5, 9, 2, 6};
    assert(kLargest(a, 3) == std::vector<int>({9, 6, 5}));

    // k = 0 returns empty.
    assert(kLargest(a, 0).empty());

    // Empty input returns empty.
    std::vector<int> empty;
    assert(kLargest(empty, 2).empty());

    // k larger than input returns all sorted descending.
    assert(kLargest(a, 100) == std::vector<int>({9, 6, 5, 4, 3, 2, 1, 1}));

    // Duplicate values counted individually.
    std::vector<int> dup = {7, 7, 7, 2};
    assert(kLargest(dup, 2) == std::vector<int>({7, 7}));

    // Single element, k=1.
    std::vector<int> single = {42};
    assert(kLargest(single, 1) == std::vector<int>({42}));

    // Negative numbers.
    std::vector<int> neg = {-5, -1, -3, -2, -4};
    assert(kLargest(neg, 2) == std::vector<int>({-1, -2}));

    // k equals size.
    assert(kLargest(a, 8) == std::vector<int>({9, 6, 5, 4, 3, 2, 1, 1}));

    // Negative k acts like 0.
    assert(kLargest(a, -1).empty());
}

#include <vector>
#include <queue>
#include <algorithm>

// Return the k largest integers from the input vector in descending order.
// Uses a min-heap of size k to keep only the largest k elements.
std::vector<int> kLargest(const std::vector<int>& nums, int k) {
    if (k <= 0 || nums.empty()) {
        return {};
    }

    // Min-heap to store the k largest elements seen so far.
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;

    for (int value : nums) {
        minHeap.push(value);
        if (static_cast<int>(minHeap.size()) > k) {
            minHeap.pop(); // Remove the smallest among current k+1.
        }
    }

    // Extract elements from the heap (they come out in ascending order).
    std::vector<int> result;
    result.reserve(minHeap.size());
    while (!minHeap.empty()) {
        result.push_back(minHeap.top());
        minHeap.pop();
    }

    // Reverse to get descending order.
    std::reverse(result.begin(), result.end());
    return result;
}

// The core idea is to maintain only the k largest elements seen so far using a min-heap of size k. Iterate through the input vector: push each element into a min-heap (priority_queue with `greater<int>`). If the heap size exceeds k, pop the smallest element (the top of the min-heap). After processing all elements, the heap contains exactly the k largest elements (or fewer if input size < k). Since a priority_queue has no direct extraction of all elements without popping, we can pop all elements from the min-heap into a temporary vector, which gives them in ascending order, then reverse it to get descending order. Edge cases: k=0 → return empty vector; empty input → return empty vector; k ≥ input size → return all elements sorted descending; duplicates are handled naturally by the heap. Time complexity is O(n log k) where n is the size of the input vector, because each push/pop on a heap of size at most k is O(log k). Space complexity is O(k) for the heap plus O(k) for the output vector.
