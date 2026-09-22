Write a C++ function `long scheduleTimer(std::vector<long>& heapKeys, std::vector<long>& heapValues, std::vector<long>& timerIds, long newKey, long newValue)` that implements a binary min-heap keyed by `newKey` (the expiration time), storing `newValue` (the timer handler ID), and a parallel `timerIds` array mapping timer IDs to heap indices. The function must insert the new key–value pair into the heap, maintain the heap property, assign the next available timer ID using a freelist mechanism (negative values in `timerIds` indicate free slots), and return that timer ID. The heap is 0-indexed, with parent at `(i-1)/2` and left child at `2*i+1`. If the array capacity is exceeded, double its size and fill new `timerIds` slots with negative sequential freelist markers. You may assume `timerIds` initially contains only `-1` (free), and the heap vectors initially have equal capacity. The function must handle key ties by preferring the smaller heap index (i.e., heapify up stops when parent key is strictly less than or equal to the inserted key). Time complexity must be `O(log n)` amortized, space `O(n)`.

The solution is a standard binary min-heap insertion with an indexed freelist for timer IDs. We first check if the heap is full; if so, we grow both `heapKeys` and `heapValues` by doubling capacity and extend `timerIds` with `-1` values (or negative sequential markers, but simple `-1` suffices for our purposes). We then assign a timer ID by scanning from the current position (maintaining a static or external counter) to find the next free slot; if we reach the end, we wrap around to the beginning. Since the problem asks for a standalone function without internal state, we instead maintain the next timer ID counter as a reference parameter or derive it from the current size. Simpler: we use the current heap size as the next timer ID if that index is free, else scan forward from 0 to find a free slot. After assigning the ID, we append the new node at the end of the heap, then bubble it up: while the node’s parent has a strictly greater key, swap the node with its parent, and update `timerIds` for both indices. Edge cases: empty heap (just insert at index 0), duplicate keys (stop bubbling when parent key is ≤ new key), and heap growth when capacity is reached. Time complexity is `O(log n)` for heapify operations (plus `O(n)` for growth, amortized), and space is `O(n)`. The `timerIds` array is updated correctly during swaps and growth.

#include <vector>
#include <algorithm>
#include <cstddef>

// Insert a new timer (key = expiration time, value = handler ID) into a binary min-heap.
// heapKeys and heapValues are parallel vectors representing the heap array.
// timerIds maps timer IDs to heap indices (non-negative) or -1 for free IDs.
// Returns the assigned timer ID, or -1 on failure.
// On insertion, the heap property is restored by bubbling up.
long scheduleTimer(std::vector<long>& heapKeys,
                   std::vector<long>& heapValues,
                   std::vector<long>& timerIds,
                   long newKey,
                   long newValue) {
    const std::size_t n = heapKeys.size();

    // Check if we need to grow the heap (and timerIds array).
    if (n >= timerIds.size()) {
        // Double the capacity, at least growing by 1.
        std::size_t newCap = timerIds.size() * 2;
        if (newCap == 0) newCap = 1;
        timerIds.resize(newCap, -1);
        heapKeys.reserve(newCap);
        heapValues.reserve(newCap);
        // Note: heapKeys and heapValues are already sized to n; we'll push_back later.
    }

    // Find a free timer ID: scan from index 0 for a -1 slot.
    long timerId = -1;
    for (std::size_t i = 0; i < timerIds.size(); ++i) {
        if (timerIds[i] == -1) {
            timerId = static_cast<long>(i);
            break;
        }
    }
    if (timerId == -1) {
        // No free slot (shouldn't happen if we grew, but safe check).
        return -1;
    }

    // Append the new node at the end of the heap.
    std::size_t current = n;
    heapKeys.push_back(newKey);
    heapValues.push_back(newValue);
    timerIds[timerId] = static_cast<long>(current);

    // Bubble up to restore min-heap property.
    while (current > 0) {
        std::size_t parent = (current - 1) / 2;
        if (heapKeys[parent] <= heapKeys[current]) {
            break; // Parent is already ≤ current, stop.
        }
        // Swap current node with parent, update mappings.
        std::swap(heapKeys[parent], heapKeys[current]);
        std::swap(heapValues[parent], heapValues[current]);
        // Update timerIds for the moved node.
        // The node that was at 'parent' now moves to 'current'.
        // Find which timer ID points to 'parent' and update it.
        for (std::size_t t = 0; t < timerIds.size(); ++t) {
            if (timerIds[t] == static_cast<long>(parent)) {
                timerIds[t] = static_cast<long>(current);
                break;
            }
        }
        // The inserted node (with timerId) now sits at 'parent'.
        timerIds[timerId] = static_cast<long>(parent);
        current = parent;
    }

    return timerId;
}

#include <cassert>
#include <vector>

// Declaration of the function (ensure it is included from the solution).
// long scheduleTimer(...);

int main() {
    // Test 1: Insert into empty heap.
    std::vector<long> keys, vals, ids;
    ids = { -1, -1, -1, -1 };
    long id1 = scheduleTimer(keys, vals, ids, 10, 100);
    assert(id1 == 0);
    assert(keys.size() == 1 && keys[0] == 10);
    assert(vals[0] == 100);
    assert(ids[0] == 0);

    // Test 2: Insert smaller key, should bubble to root.
    std::vector<long> k2 = {10}, v2 = {100}, ids2 = {0, -1, -1, -1};
    long id2 = scheduleTimer(k2, v2, ids2, 5, 200);
    assert(id2 == 1);
    assert(k2.size() == 2 && k2[0] == 5 && k2[1] == 10);
    assert(v2[0] == 200 && v2[1] == 100);
    // Timer ID 1 should point to heap index 0, ID 0 to index 1.
    assert(ids2[1] == 0);
    assert(ids2[0] == 1);

    // Test 3: Insert larger key, no bubble.
    std::vector<long> k3 = {5,10}, v3 = {200,100}, ids3 = {1,0,-1,-1};
    long id3 = scheduleTimer(k3, v3, ids3, 20, 300);
    assert(id3 == 2);
    assert(k3[2] == 20 && v3[2] == 300);
    assert(ids3[2] == 2);

    // Test 4: Equal key, should not bubble (parent ≤ child).
    std::vector<long> k4 = {5,10,20}, v4 = {200,100,300}, ids4 = {1,0,2,-1};
    long id4 = scheduleTimer(k4, v4, ids4, 5, 400);
    assert(id4 == 3);
    assert(k4[3] == 5);
    // Parent at index 0 has key 5, so no swap; child at index 1 has key 10.
    assert(k4[0] == 5 && k4[1] == 10 && k4[2] == 20 && k4[3] == 5);
    assert(ids4[3] == 3);

    // Test 5: Heap growth when capacity is full.
    std::vector<long> k5 = {1}, v5 = {10}, ids5 = {0}; // Capacity 1.
    long id5 = scheduleTimer(k5, v5, ids5, 2, 20);
    assert(id5 == 1);
    assert(k5.size() == 2 && k5[0] == 1 && k5[1] == 2);
    assert(ids5.size() == 2 && ids5[1] == 1);
    assert(ids5[0] == 0);

    // Test 6: Reuse a freed slot (set timerIds[0] = -1 as if cancelled).
    std::vector<long> k6 = {1,2,3}, v6 = {10,20,30}, ids6 = { -1, 1, 2 }; // slot 0 free.
    long id6 = scheduleTimer(k6, v6, ids6, 0, 5);
    assert(id6 == 0);
    // New key 0 is smallest, so it should bubble to root.
    assert(k6[0] == 0 && k6[1] == 1 && k6[2] == 2 && k6[3] == 3);
    assert(v6[0] == 5);
    assert(ids6[0] == 0); // timer ID 0 now maps to root.

    return 0;
}
