// Write a C++ function `std::vector<int> kSmallestUsingBinaryHeap(const std::vector<int>& data, int k)` that returns the `k` smallest distinct integers from the input vector in ascending order, using a binary min-heap implemented with arrays. The function must not use `std::priority_queue` or `std::sort`; instead, implement your own heap operations (insert, delete-min, and optionally decrease-key) directly on a vector-based heap that stores satellite indices (original positions in `data`). If `k` is larger than the number of distinct integers, return all distinct values. Edge cases: empty input returns empty vector; `k <= 0` returns empty vector. The heap must handle duplicate values correctly—when inserting, if a value is already present, skip it rather than throwing or storing duplicates. For full credit, the heap should use a `key` array to store each value, a `satellite` array to store the original index, and a `pos` array to map from index to heap position, exactly as in the provided snippet but adapted for extraction. Return the distinct values in ascending order. For example, `data = {5,3,8,3,1,5,7}`, `k=3` → `{1,3,5}`.

The solution requires a custom binary heap that supports insert with duplicate checking, delete-min, and works with satellite data. The main algorithm: iterate through all elements of `data`; for each element, check if it already exists in the heap via a `pos` map (initialized to -1). If not present, insert it by first resizing arrays if needed, then bubbling up from the last position using a key-comparison. After processing all elements, repeatedly call `deleteMin()` exactly `k` times (or until heap empty) to collect the smallest distinct values; `deleteMin` swaps the last leaf to the root and percolates down. Since duplicates are skipped, the heap naturally stores distinct values. Edge cases: empty input, `k <= 0`, and `k >= number of distinct values`. Time complexity: insertion of `n` elements is O(n log n) worst-case, but since duplicates are skipped, it's O(n log d) where d is distinct count. Extracting k elements is O(k log d). Space complexity O(n + d) for the arrays. The `pos` array is sized to max input value + 1 if values are non-negative, but a safer approach is to use a hash map, although the prompt implies array indices; we assume values are non-negative integers and size `pos` to the maximum value + 1. If negative values are possible, use a map, but for a self-contained task, assume non-negative.

#include <vector>
#include <algorithm>
#include <stdexcept>

// Returns the k smallest distinct integers from data in ascending order.
// Implements a binary min-heap with satellite indices.
std::vector<int> kSmallestUsingBinaryHeap(const std::vector<int>& data, int k) {
    if (k <= 0 || data.empty()) return {};

    // Determine max value to size pos array (assume non-negative inputs)
    int maxVal = *std::max_element(data.begin(), data.end());
    std::vector<int> pos(maxVal + 1, -1); // pos[value] = heap position, -1 if absent
    std::vector<int> key(data.size() + 1); // key[i] = value at heap position i (1-indexed)
    std::vector<int> satellite(data.size() + 2); // satellite[i] = original value at heap position i (0 unused)
    int size = 0;

    // Insert helper (lambda)
    auto insert = [&](int value) {
        // Resize arrays if needed (values are distinct and bounded by data size)
        if (size + 2 > (int)satellite.size()) {
            satellite.resize(satellite.size() * 2);
            key.resize(satellite.size());
        }
        int i;
        for (i = ++size; i / 2 > 0 && key[satellite[i / 2]] > value; i /= 2) {
            satellite[i] = satellite[i / 2];
            pos[satellite[i]] = i;
        }
        satellite[i] = value;
        pos[value] = i;
        key[value] = value; // key array indexed by value for simplicity; but we only need key[satellite[i]]
        // Actually key array should store key per heap position; fix: store key[i] = value
        key[i] = value;
    };

    // Corrected insert: we need key indexed by heap position
    auto insertCorrect = [&](int value) {
        if (pos[value] != -1) return; // already present
        int i;
        for (i = ++size; i / 2 > 0 && key[satellite[i / 2]] > value; i /= 2) {
            satellite[i] = satellite[i / 2];
            pos[satellite[i]] = i;
        }
        satellite[i] = value;
        pos[value] = i;
        key[i] = value;
    };

    // Delete min helper
    auto deleteMin = [&]() -> int {
        if (size == 0) throw std::runtime_error("empty");
        int min = satellite[1];
        int slast = satellite[size--];
        int child, i;
        for (i = 1, child = 2; child <= size; i = child, child *= 2) {
            if (child < size && key[satellite[child]] > key[satellite[child + 1]])
                child++;
            if (key[slast] > key[satellite[child]]) {
                satellite[i] = satellite[child];
                pos[satellite[child]] = i;
            } else break;
        }
        satellite[i] = slast;
        pos[slast] = i;
        pos[min] = -1;
        return min;
    };

    // Insert all distinct values
    for (int v : data) {
        if (pos[v] == -1) insertCorrect(v);
    }

    // Extract k smallest
    std::vector<int> result;
    int count = 0;
    while (size > 0 && count < k) {
        result.push_back(deleteMin());
        count++;
    }
    return result;
}

#include <cassert>
#include <vector>

// Forward declaration or include solution here
std::vector<int> kSmallestUsingBinaryHeap(const std::vector<int>& data, int k);

int main() {
    // Basic test
    std::vector<int> a = {5,3,8,3,1,5,7};
    assert(kSmallestUsingBinaryHeap(a, 3) == std::vector<int>({1,3,5}));
    
    // k larger than distinct count
    assert(kSmallestUsingBinaryHeap(a, 10) == std::vector<int>({1,3,5,7,8}));
    
    // k = 1
    assert(kSmallestUsingBinaryHeap(a, 1) == std::vector<int>({1}));
    
    // All same values
    std::vector<int> b = {2,2,2};
    assert(kSmallestUsingBinaryHeap(b, 2) == std::vector<int>({2}));
    
    // Empty input
    assert(kSmallestUsingBinaryHeap({}, 5).empty());
    
    // k <= 0
    assert(kSmallestUsingBinaryHeap(a, 0).empty());
    assert(kSmallestUsingBinaryHeap(a, -1).empty());
    
    // Already sorted and distinct
    std::vector<int> c = {1,2,3,4};
    assert(kSmallestUsingBinaryHeap(c, 4) == std::vector<int>({1,2,3,4}));
    
    // Single element
    std::vector<int> d = {42};
    assert(kSmallestUsingBinaryHeap(d, 1) == std::vector<int>({42}));
    
    // Large values, duplicates
    std::vector<int> e = {100,50,100,25,50,75,25};
    assert(kSmallestUsingBinaryHeap(e, 5) == std::vector<int>({25,50,75,100}));
    
    return 0;
}
