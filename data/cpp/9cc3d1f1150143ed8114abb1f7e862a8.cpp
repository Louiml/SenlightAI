// Write a C++ function `vector<int> processQueries(int n, vector<vector<int>>& queries)` that processes a sequence of two types of queries on an initially empty min-heap and returns the values deleted in order. Each query is a vector of integers: a type-1 query has exactly two integers `[1, x]` and means "insert x into the heap"; a type-2 query has exactly one integer `[2]` and means "extract and record the current minimum, then remove it from the heap". The function must return a vector containing, in order, all values that were extracted during type-2 operations. If a type-2 query is performed when the heap is empty, no value is recorded for that query. You must implement the heap operations manually using a dynamic array; do not use `std::priority_queue` or any other container adaptors. The parameter `n` is the number of queries, but you may ignore it and use the size of `queries` directly.
// The core idea is to maintain a binary min-heap implemented on a `std::vector<int>`. For an insertion (type‑1), append the new value at the end and then "bubble up" by repeatedly comparing with its parent `(i-1)/2`; if the parent is larger, swap and move upward. This ensures the smallest element is always at index 0. For a deletion (type‑2), if the heap is empty, do nothing; otherwise, record the root (minimum), swap it with the last element, remove the last element, and then "heapify down" from the root: compare with the left and right children, select the smallest among the three, swap if needed, and recurse into the affected subtree. Important edge cases: heap of size 1 (deletion records and empties), heap of size 0 (deletion ignored), duplicate values (comparison uses strict `<` to preserve heap property). The algorithm performs each insertion in O(log k) time and each deletion in O(log k) time, where k is the current heap size. The total time complexity for m queries is O(m log m) in the worst case, and the auxiliary space is O(m) for the heap and answer vector, plus O(log m) recursion depth for heapify. The parameter `n` is redundant but kept for signature compatibility.
#include <vector>
#include <utility>

// Process min-heap queries: return extracted minimums in order.
std::vector<int> processQueries(int n, std::vector<std::vector<int>>& queries) {
    std::vector<int> heap;
    std::vector<int> result;

    // Bubble up the last element to restore min-heap property.
    auto heapifyUp = [&heap](int index) {
        while (index > 0) {
            int par = (index - 1) / 2;
            if (heap[par] <= heap[index]) break;
            std::swap(heap[par], heap[index]);
            index = par;
        }
    };

    // Heapify down from index to restore min-heap property.
    auto heapifyDown = [&heap](int index) {
        int size = static_cast<int>(heap.size());
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size && heap[left] < heap[smallest]) {
                smallest = left;
            }
            if (right < size && heap[right] < heap[smallest]) {
                smallest = right;
            }
            if (smallest == index) break;
            std::swap(heap[index], heap[smallest]);
            index = smallest;
        }
    };

    for (const auto& q : queries) {
        if (q.size() == 2) {
            // Insert operation
            int x = q[1];
            heap.push_back(x);
            heapifyUp(static_cast<int>(heap.size()) - 1);
        } else {
            // Extraction operation
            if (heap.empty()) continue;
            result.push_back(heap[0]);
            if (heap.size() == 1) {
                heap.pop_back();
            } else {
                std::swap(heap[0], heap.back());
                heap.pop_back();
                heapifyDown(0);
            }
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// Declaration of the function under test (assumed to be defined above).
std::vector<int> processQueries(int n, std::vector<std::vector<int>>& queries);

int main() {
    // Test 1: Basic insert and delete order
    std::vector<std::vector<int>> q1 = {{1,5}, {1,3}, {1,8}, {2}, {2}};
    assert(processQueries(5, q1) == std::vector<int>({3,5}));

    // Test 2: Deletion on empty heap should be ignored
    std::vector<std::vector<int>> q2 = {{2}, {1,10}, {2}, {2}};
    assert(processQueries(4, q2) == std::vector<int>({10}));

    // Test 3: Duplicate values and single element
    std::vector<std::vector<int>> q3 = {{1,7}, {1,7}, {2}, {2}};
    assert(processQueries(4, q3) == std::vector<int>({7,7}));

    // Test 4: Larger sequence with mixed operations
    std::vector<std::vector<int>> q4 = {{1,2}, {1,1}, {1,3}, {2}, {1,0}, {2}, {2}};
    assert(processQueries(7, q4) == std::vector<int>({1,0,2}));

    // Test 5: All insertions only, no deletions ⇒ empty result
    std::vector<std::vector<int>> q5 = {{1,4}, {1,9}, {1,2}};
    assert(processQueries(3, q5) == std::vector<int>({}));

    // Test 6: Single insertion then deletion
    std::vector<std::vector<int>> q6 = {{1,100}, {2}};
    assert(processQueries(2, q6) == std::vector<int>({100}));

    // Test 7: Many deletions on empty heap (no output)
    std::vector<std::vector<int>> q7 = {{2}, {2}, {2}};
    assert(processQueries(3, q7) == std::vector<int>({}));

    // Test 8: Check correct heap order with random-like pattern
    std::vector<std::vector<int>> q8 = {{1,50}, {1,20}, {2}, {1,10}, {1,30}, {2}, {2}};
    assert(processQueries(7, q8) == std::vector<int>({20,10,30}));

    // Test 9: Large heap, verify extraction sequence is sorted ascending
    std::vector<std::vector<int>> q9;
    for (int i = 20; i >= 1; --i) q9.push_back({1, i});
    for (int i = 0; i < 20; ++i) q9.push_back({2});
    std::vector<int> expected;
    for (int i = 1; i <= 20; ++i) expected.push_back(i);
    assert(processQueries(40, q9) == expected);

    return 0;
}
