// Write a C++ function that simulates a fixed-capacity array-based linked list where each node stores an integer value and an index to the "next" node. The function should take a positive integer `n` (the number of nodes) and return a `std::vector<std::pair<int,int>>` representing the nodes, where each pair contains the node's integer value (from 0 to n-1) and the index of the next node (with the last node pointing to -1 to indicate end of list). The list must be constructed such that the nodes are linked sequentially in order (node i points to node i+1, and the last node points to -1). This mimics a singly linked list using an array of nodes without dynamic allocation, exactly as the snippet intends but with explicit linking. The function must handle `n = 0` by returning an empty vector, and `n = 1` by returning a single pair with next = -1.
The main algorithm is straightforward: create a vector of `n` pairs. For each index `i` from 0 to n-1, set the first element of the pair (the integer value) to `i`. For the next pointer, if `i < n-1`, set it to `i+1`; otherwise (last node), set it to -1. This constructs a chain where each node's "next" field references the next array index, exactly mimicking linked-list traversal. Edge cases: when `n = 0`, return an empty vector (no nodes). When `n = 1`, the only node points to -1. The values and links are deterministic and require no dynamic memory allocation. Time complexity is O(n) because we iterate through n nodes once. Space complexity is O(n) for the output vector, plus O(1) auxiliary space.
#include <vector>
#include <utility>

// Builds a fixed-array simulated linked list of n nodes.
// Each node stores its index as value and points to the next node index (or -1 for last).
std::vector<std::pair<int,int>> buildLinkedList(int n) {
    std::vector<std::pair<int,int>> result;
    result.reserve(n);
    for (int i = 0; i < n; ++i) {
        int next = (i < n - 1) ? (i + 1) : -1;
        result.emplace_back(i, next);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// forward declaration or include the solution here (for self-contained test)
std::vector<std::pair<int,int>> buildLinkedList(int n);

int main() {
    // n = 0 -> empty
    assert(buildLinkedList(0).empty());

    // n = 1 -> single node pointing to -1
    auto one = buildLinkedList(1);
    assert(one.size() == 1);
    assert(one[0] == std::make_pair(0, -1));

    // n = 3 -> sequential links
    auto three = buildLinkedList(3);
    assert(three.size() == 3);
    assert(three[0] == std::make_pair(0, 1));
    assert(three[1] == std::make_pair(1, 2));
    assert(three[2] == std::make_pair(2, -1));

    // n = 5 -> check all nodes and last link
    auto five = buildLinkedList(5);
    for (int i = 0; i < 5; ++i) {
        assert(five[i].first == i);
        if (i < 4) {
            assert(five[i].second == i + 1);
        } else {
            assert(five[i].second == -1);
        }
    }
}
