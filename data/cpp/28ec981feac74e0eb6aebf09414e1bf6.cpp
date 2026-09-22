/*
Write a C++ function `bool hasCycle(const std::vector<int>& nodes)` that determines whether a static linked list, represented by a one-dimensional array where `nodes[i]` stores the index of the next node (with `0` conventionally used as a null/end marker), contains a cycle. The list starts at index `1` (since index `0` is unused, as in the snippet). The function must handle lists with a terminal node whose `nodes[i]` value is `0` (end marker) and also lists where a node points to itself, back to an earlier node, or forms a cycle of any length. The input vector may contain invalid indices or values outside the valid range (e.g., exceeding the vector size or negative), and the function must treat such cases as either no cycle (if an invalid index would be reached before a cycle forms) or report a cycle safely without infinite looping or out-of-bounds access. Your implementation must avoid modifying the input and must work in O(N) time even if a cycle is present, where N is the number of nodes with valid indices (i.e., indices from 1 to nodes.size()-1). Use Floyd’s cycle-detection algorithm (tortoise and hare) for efficiency, but be careful to handle the case where the hare or tortoise encounters a zero (end) or an out-of-range index.
*/

#include <vector>

/**
 * Determines if a static linked list (represented by nodes[i] = next index) contains a cycle.
 * Index 0 is treated as the null/end marker. The list starts at index 1.
 * Out-of-range indices are treated as the end of the list.
 */
bool hasCycle(const std::vector<int>& nodes) {
    const std::size_t size = nodes.size();
    if (size <= 1) {
        return false;
    }

    int slow = 1;
    int fast = 1;

    // Helper lambda to advance a pointer by one step, returning false if we can't (end or invalid).
    auto advance = [&](int& ptr) -> bool {
        if (ptr < 0 || static_cast<std::size_t>(ptr) >= size) {
            return false;
        }
        ptr = nodes[ptr];
        return ptr != 0;
    };

    // Do the first advance for both pointers separately.
    if (!advance(slow)) return false;
    if (!advance(fast)) return false;
    if (!advance(fast)) return false;

    while (slow != fast) {
        if (!advance(slow)) return false;
        if (!advance(fast)) return false;
        if (!advance(fast)) return false;
    }

    // If we exit the loop, slow and fast are equal; a cycle exists.
    return true;
}

#include <cassert>
#include <vector>

// Include the solution function here or link it.

int main() {
    // Empty vector
    assert(hasCycle(std::vector<int>{}) == false);
    // Only index 0, no nodes
    assert(hasCycle(std::vector<int>{0}) == false);
    // Single node pointing to 0 (end)
    assert(hasCycle(std::vector<int>{0, 0}) == false);
    // Single node pointing to itself (cycle)
    assert(hasCycle(std::vector<int>{0, 1}) == true);
    // Two nodes with cycle 1->2->1
    assert(hasCycle(std::vector<int>{0, 2, 1}) == true);
    // Three nodes, tail (1->2->3->0) no cycle
    assert(hasCycle(std::vector<int>{0, 2, 3, 0}) == false);
    // Cycle in middle: 1->2->3->2
    assert(hasCycle(std::vector<int>{0, 2, 3, 2}) == true);
    // Invalid index (5) from node 1
    assert(hasCycle(std::vector<int>{0, 5, 3}) == false);
    // Negative index from node 2
    assert(hasCycle(std::vector<int>{0, 2, -1}) == false);
    // Large cycle with tail: 1->2->3->4->5->3
    assert(hasCycle(std::vector<int>{0, 2, 3, 4, 5, 3}) == true);
    // Self-cycle at node 3
    assert(hasCycle(std::vector<int>{0, 2, 3, 3}) == true);
    return 0;
}

// The core idea is to simulate traversal using two pointers moving at different speeds: a slow pointer (tortoise) that advances one step per iteration, and a fast pointer (hare) that advances two steps. Initially, both point to index 1. At each step, we must safely advance the pointers. For the slow pointer, we read `nodes[slow]`; if that value is `0`, the list has ended and there is no cycle. If the value is out of bounds (i.e., `<0` or `>= nodes.size()`), treat as an end (no cycle) because an invalid pointer cannot continue. For the fast pointer, we advance twice; after the first advance, if it becomes `0` or invalid, then no cycle; otherwise, after the second advance, similarly check. If at any point slow equals fast (and they are not both at the start), a cycle exists. To avoid the initial condition where both start at 1, we advance both first before comparing. Edge cases: an empty list (nodes.size() <= 1) has no nodes to traverse, so return false. A list that is a single node pointing to itself: e.g., nodes = {0, 1} yields slow=1, fast=1 after first advance? Actually, after advancing slow from 1 to nodes[1]=1, slow stays 1; fast advances from 1 to nodes[1]=1, then again to nodes[1]=1; now slow==fast, so cycle detected. A list pointing to an invalid index like nodes[1]=5 but nodes.size()=3: the first advance of slow becomes 5, which is out of bounds, so we return false (no cycle) because the traversal ends. Time complexity is O(N) because in the worst case, the tortoise traverses at most N nodes before either hitting an end or meeting the hare (which may loop but the meeting occurs within O(N) steps because the distance between pointers reduces by 1 each step). Space is O(1).
