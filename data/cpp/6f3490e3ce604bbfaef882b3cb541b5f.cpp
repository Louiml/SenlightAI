// Given a string representing a node's name (e.g., `"node"`) and two strings representing the names of its next and previous neighbors in a doubly linked ring, write a C++ function `bool isValidTraversal(const std::string& start, const std::string& node1, const std::string& node2, const std::string& node3, const std::string& node4)` that checks whether a circular traversal starting at `start` and following the `next` links from each node returns to `start` after visiting all provided nodes exactly once, and that each node's `prev` link correctly points back to the node that came before it according to the traversal order. The function should return `true` only if the ring is consistent in both directions. The input will always contain exactly five distinct non-empty strings (the start node plus four other nodes), and each node’s `next` and `prev` relationship is implicitly defined by the order they are passed: `node1` follows `start`, `node2` follows `node1`, `node3` follows `node2`, `node4` follows `node3`, and `start` follows `node4`. Additionally, for each pair of adjacent nodes in this fixed order, the `prev` of the later node must equal the earlier node, and the `prev` of `start` must equal `node4`. The function must also verify that the `next` link of `start` is `node1`, and so on, in the fixed order.
The problem reduces to verifying a doubly linked circular list of exactly five nodes given in a specific order. The main algorithm is straightforward: first, check that the five input strings are all distinct (though the problem guarantees this, we can still verify defensively). Then, construct an array of the five nodes in traversal order: `[start, node1, node2, node3, node4]`. For each index `i` from 0 to 4, the `next` of the node at index `i` should be the node at index `(i+1)%5`, and the `prev` of the node at index `(i+1)%5` should be the node at index `i`. Equivalently, we can directly compare: `next` of `start` equals `node1`, `next` of `node1` equals `node2`, etc., and `prev` of `node1` equals `start`, `prev` of `node2` equals `node1`, etc., and `prev` of `start` equals `node4`. Edge cases: since the ring is circular and the order is fixed, the only possible failure is a mismatch in any of the ten comparisons (5 next links and 5 prev links). Also, if any of the strings are empty or equal to each other, the ring is invalid (though the problem constrains them to be distinct non-empty, we can still assert). Time complexity is O(1) because there are constant nodes to check, and space complexity is O(1) since we only store a few references.
#include <string>
#include <vector>
#include <algorithm>

// Check if the given five nodes form a consistent doubly linked circular ring.
// Traversal order: start -> node1 -> node2 -> node3 -> node4 -> start.
bool isValidTraversal(const std::string& start,
                      const std::string& node1,
                      const std::string& node2,
                      const std::string& node3,
                      const std::string& node4) {
    std::vector<std::string> order = {start, node1, node2, node3, node4};
    
    // Defensive: ensure all nodes are distinct and non-empty.
    for (const auto& s : order) {
        if (s.empty()) return false;
    }
    for (size_t i = 0; i < order.size(); ++i) {
        for (size_t j = i + 1; j < order.size(); ++j) {
            if (order[i] == order[j]) return false;
        }
    }
    
    // Check forward (next) and backward (prev) links.
    for (size_t i = 0; i < 5; ++i) {
        const std::string& current = order[i];
        const std::string& next = order[(i + 1) % 5];
        const std::string& prev = order[(i + 4) % 5];
        
        // The "next" of current must be the "next" node.
        if (current == start) {
            if (next != node1) return false;
        } else if (current == node1) {
            if (next != node2) return false;
        } else if (current == node2) {
            if (next != node3) return false;
        } else if (current == node3) {
            if (next != node4) return false;
        } else {
            if (next != start) return false;
        }
        
        // The "prev" of the next node must be the current node.
        if (next == start) {
            if (prev != node4) return false;
        } else if (next == node1) {
            if (prev != start) return false;
        } else if (next == node2) {
            if (prev != node1) return false;
        } else if (next == node3) {
            if (prev != node2) return false;
        } else {
            if (prev != node3) return false;
        }
    }
    
    return true;
}
#include <cassert>

int main() {
    // Correct consistent ring.
    assert(isValidTraversal("A", "B", "C", "D", "E") == true);
    // Start changed but still consistent order.
    assert(isValidTraversal("B", "C", "D", "E", "A") == true);
    // Swap two adjacent nodes breaks the fixed order.
    assert(isValidTraversal("A", "C", "B", "D", "E") == false);
    // Break backward link: prev of "B" should be "A", but we pass wrong order.
    // Here order is A,C,D,E,B so next of A is C, which is fine, but prev of C is A, but prev of B should be E, and prev of E should be D, etc.
    // Actually let's test a direct mismatch: next of "A" is "C" which is not "B".
    assert(isValidTraversal("A", "C", "D", "E", "B") == false);
    // Duplicate node should fail (defensive).
    assert(isValidTraversal("A", "A", "C", "D", "E") == false);
    // Empty string fails.
    assert(isValidTraversal("", "B", "C", "D", "E") == false);
    // Same as first but with different labels.
    assert(isValidTraversal("X", "Y", "Z", "W", "V") == true);
    // Reversal of order (A,E,D,C,B) fails because forward links don't match.
    assert(isValidTraversal("A", "E", "D", "C", "B") == false);
    // Correct but with unusual names.
    assert(isValidTraversal("alpha", "beta", "gamma", "delta", "epsilon") == true);
}
