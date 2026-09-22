/*
Write a C++ function that takes a vector of integers and an integer `k` as input, and returns a new vector containing the `k` smallest elements from the original vector, in ascending order. The function should use a min-heap data structure to efficiently select the `k` smallest elements, without modifying the original vector. If `k` is larger than the vector size, return all elements sorted in ascending order. If `k` is zero or negative, return an empty vector.
*/

#include <vector>
#include <algorithm>

// Return the k smallest elements from the input vector in ascending order.
// Uses a min-heap implemented via the standard heap functions on a vector.
std::vector<int> kSmallestElements(const std::vector<int>& input, int k) {
    std::vector<int> result;
    if (k <= 0) {
        return result; // empty for non-positive k
    }

    // Build a min-heap by copying and heapifying.
    std::vector<int> heap = input;
    std::make_heap(heap.begin(), heap.end(), std::greater<int>());

    // Extract the smallest element k times or until heap is empty.
    while (!heap.empty() && static_cast<int>(result.size()) < k) {
        // Extract min: pop_heap moves the smallest to the end, then remove it.
        std::pop_heap(heap.begin(), heap.end(), std::greater<int>());
        result.push_back(heap.back());
        heap.pop_back();
    }

    return result;
}

#include <cassert>
#include <vector>

// Forward declaration of the solution function.
std::vector<int> kSmallestElements(const std::vector<int>& input, int k);

int main() {
    // Basic case: pick 3 smallest from a set.
    std::vector<int> v1 = {5, 3, 8, 1, 9, 2};
    assert(kSmallestElements(v1, 3) == std::vector<int>({1, 2, 3}));

    // k larger than vector size: should return all sorted.
    std::vector<int> v2 = {4, 2, 7};
    assert(kSmallestElements(v2, 5) == std::vector<int>({2, 4, 7}));

    // k equal to vector size.
    std::vector<int> v3 = {10, -5, 0};
    assert(kSmallestElements(v3, 3) == std::vector<int>({-5, 0, 10}));

    // k = 0 returns empty.
    std::vector<int> v4 = {1, 2, 3};
    assert(kSmallestElements(v4, 0).empty());

    // k negative returns empty.
    std::vector<int> v5 = {1, 2, 3};
    assert(kSmallestElements(v5, -2).empty());

    // Duplicates: only the required number of smallest values.
    std::vector<int> v6 = {7, 7, 1, 7, 3};
    assert(kSmallestElements(v6, 2) == std::vector<int>({1, 3}));

    // Single element with k=1.
    std::vector<int> v7 = {42};
    assert(kSmallestElements(v7, 1) == std::vector<int>({42}));

    // Original vector is not modified.
    std::vector<int> original = {9, 1, 5, 3};
    kSmallestElements(original, 2);
    assert(original == std::vector<int>({9, 1, 5, 3}));

    return 0;
}

// The solution uses a min-heap to repeatedly extract the minimum element from the remaining elements. Start by inserting all elements of the vector into a min-heap. Then, while the heap is not empty and we have not yet collected `k` elements, extract the minimum from the heap and append it to the result vector. Since the heap always gives the smallest remaining element, extracting `k` times gives the `k` smallest elements in ascending order. Edge cases: if `k` is greater than or equal to the vector size, the loop will extract all elements, yielding the fully sorted vector. If `k` is zero or negative, the loop condition will not run, and an empty vector is returned. The heap operations (insert and extractMin) each take O(log n) time, so building the heap takes O(n log n) and extracting k elements takes O(k log n). Total time complexity is O((n + k) log n), which simplifies to O(n log n) in the worst case when k is close to n. Space complexity is O(n) for the heap storage plus O(k) for the result vector.
