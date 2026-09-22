// You are given a collection of location names that may repeat. Implement a C++ function `std::vector<std::pair<int, std::string>> rankLocations(const std::vector<std::string>& locations)` that returns a list of unique locations along with their occurrence counts, sorted in **descending order by count**. If two locations have the same count, order them by their **first appearance** in the input (i.e., stable order). The function must process each location by inserting it into a max-heap-like structure that maintains the invariant that higher‑count locations appear earlier, and when a count is incremented, the element moves upward until the heap property holds. You may use any standard containers internally, but the **final returned vector** must be sorted by count descending, with ties broken by earliest first occurrence. Handle an empty input by returning an empty vector. Do not use `std::sort` or other pre-built sorting algorithms on the final result—you must implement the heap operations manually (insert and sift‑up). The solution must be self‑contained and not rely on external libraries beyond the C++ standard library.
// The core idea is to simulate a max‑heap keyed by frequency, where each node stores a location string and its current count. We maintain a **stable** heap: when equal‑frequency elements exist, the one inserted first should remain first. To achieve this, we store an insertion order index (a monotonically increasing integer) with each node. When comparing two nodes, we first compare counts (higher count wins). If counts are equal, the smaller insertion index wins (i.e., earlier appearance). We process each input location sequentially: if the location is not yet in the heap, create a new node with count 1 and insert it at the end, then sift‑up. If the location already exists, increment its count and sift‑up to restore the heap property. After processing all locations, we simply extract the nodes from the heap by repeatedly popping the root (swap root with last, reduce size, sift‑down) and building the result vector in descending count order. Since we need stable ties, the heap’s comparison rule using insertion index ensures that after sift‑up and sift‑down operations, the order of equal‑count elements remains consistent with the original insertion order. Time complexity: each insertion or increment triggers a sift‑up of at most O(log n) steps, giving O(m log n) for m inputs and n unique locations. Extracting all nodes costs O(n log n). Space complexity: O(n) for the heap. Edge cases: empty input, repeated identical locations, and locations with equal counts.
#include <string>
#include <vector>
#include <utility>
#include <cassert>

// Represents a node in the heap: location, count, and insertion order key.
struct HeapNode {
    std::string location;
    int count;
    int order; // insertion order (smaller means earlier)
};

// Comparison function for the max-heap: higher count, then lower order (earlier).
bool heapOrderBetter(const HeapNode& a, const HeapNode& b) {
    if (a.count != b.count)
        return a.count > b.count;
    return a.order < b.order;
}

// Sift-up operation for the heap starting at index i (0-based).
void siftUp(std::vector<HeapNode>& heap, int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heapOrderBetter(heap[i], heap[parent])) {
            std::swap(heap[i], heap[parent]);
            i = parent;
        } else {
            break;
        }
    }
}

// Sift-down operation for the heap starting at index i.
void siftDown(std::vector<HeapNode>& heap, int n, int i) {
    while (true) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int best = i;
        if (left < n && heapOrderBetter(heap[left], heap[best]))
            best = left;
        if (right < n && heapOrderBetter(heap[right], heap[best]))
            best = right;
        if (best != i) {
            std::swap(heap[i], heap[best]);
            i = best;
        } else {
            break;
        }
    }
}

// Main function: returns unique locations sorted by count descending, ties by first appearance.
std::vector<std::pair<int, std::string>> rankLocations(const std::vector<std::string>& locations) {
    std::vector<HeapNode> heap;
    int currentOrder = 0;

    for (const auto& loc : locations) {
        // Search for existing location in the heap.
        int idx = -1;
        for (int i = 0; i < (int)heap.size(); ++i) {
            if (heap[i].location == loc) {
                idx = i;
                break;
            }
        }

        if (idx == -1) {
            // New location: insert with count 1.
            heap.push_back({loc, 1, currentOrder});
            currentOrder++;
            siftUp(heap, (int)heap.size() - 1);
        } else {
            // Existing location: increment count.
            heap[idx].count++;
            siftUp(heap, idx);
        }
    }

    // Extract from heap in descending count order.
    std::vector<std::pair<int, std::string>> result;
    for (int n = (int)heap.size(); n > 0; --n) {
        result.push_back({heap[0].count, heap[0].location});
        std::swap(heap[0], heap[n - 1]);
        siftDown(heap, n - 1, 0);
    }

    return result;
}
#include <cassert>
#include <string>
#include <vector>
#include <utility>

// Declare the function being tested.
std::vector<std::pair<int, std::string>> rankLocations(const std::vector<std::string>& locations);

int main() {
    // Basic test with repeated locations.
    std::vector<std::string> input1 = {"A", "B", "A", "C", "B", "A"};
    auto result1 = rankLocations(input1);
    assert(result1.size() == 3);
    assert(result1[0] == std::make_pair(3, std::string("A")));
    assert(result1[1] == std::make_pair(2, std::string("B")));
    assert(result1[2] == std::make_pair(1, std::string("C")));

    // Ties in count: first appearance order decides.
    std::vector<std::string> input2 = {"X", "Y", "X", "Z", "Y"};
    // Counts: X=2, Y=2, Z=1. First appearance: X (index0), Y(index1), Z(index3).
    auto result2 = rankLocations(input2);
    assert(result2.size() == 3);
    assert(result2[0] == std::make_pair(2, std::string("X")));
    assert(result2[1] == std::make_pair(2, std::string("Y")));
    assert(result2[2] == std::make_pair(1, std::string("Z")));

    // Empty input.
    std::vector<std::string> input3;
    auto result3 = rankLocations(input3);
    assert(result3.empty());

    // All same location.
    std::vector<std::string> input4 = {"P", "P", "P"};
    auto result4 = rankLocations(input4);
    assert(result4.size() == 1);
    assert(result4[0] == std::make_pair(3, std::string("P")));

    // Single unique element.
    std::vector<std::string> input5 = {"Q"};
    auto result5 = rankLocations(input5);
    assert(result5.size() == 1);
    assert(result5[0] == std::make_pair(1, std::string("Q")));

    // More complex case with many equal counts.
    std::vector<std::string> input6 = {"a", "b", "c", "d", "a", "b"};
    // Counts: a=2, b=2, c=1, d=1. Ties: a then b (first appearance), c then d.
    auto result6 = rankLocations(input6);
    assert(result6.size() == 4);
    assert(result6[0] == std::make_pair(2, std::string("a")));
    assert(result6[1] == std::make_pair(2, std::string("b")));
    assert(result6[2] == std::make_pair(1, std::string("c")));
    assert(result6[3] == std::make_pair(1, std::string("d")));

    // Test that repeated updates preserve stable order.
    std::vector<std::string> input7 = {"n", "m", "n", "m", "p"};
    // n=2, m=2, p=1. First appearance: n then m.
    auto result7 = rankLocations(input7);
    assert(result7[0] == std::make_pair(2, std::string("n")));
    assert(result7[1] == std::make_pair(2, std::string("m")));
    assert(result7[2] == std::make_pair(1, std::string("p")));

    return 0;
}
